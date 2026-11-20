# 基于 STM32F103C8T6 的智能火灾报警系统

这是一个基于 **STM32F103C8T6 + ESP8266 + OneNET** 的物联网火灾报警项目。系统实时采集环境温度、湿度、烟雾浓度和明火状态，在本地 OLED 上显示数据，并在检测到异常时通过蜂鸣器报警；同时通过 ESP8266 以 MQTT 协议将数据上传到 OneNET 云平台。

## 功能特性

- 使用 DHT11 采集环境温度与湿度
- 使用 MQ-2 采集烟雾浓度，并将 ADC 原始值换算为 0%～100% 的显示值
- 使用火焰传感器检测明火，支持低电平触发的数字量输入
- 使用 0.96 英寸 OLED 实时显示测量值和报警状态
- 温度、烟雾或明火任一条件满足时，蜂鸣器持续报警
- 按键可临时静音，静音持续约 10 秒
- ESP8266 连接 Wi-Fi，通过 MQTT 将数据上传到 OneNET
- 默认每 4 秒上传一次温度、湿度、烟雾、火焰和报警状态

## 系统结构

```text
DHT11 温湿度传感器 ─┐
MQ-2 烟雾传感器   ─┼──> STM32F103C8T6 ──> OLED 显示屏
火焰传感器         ─┘          │
                               ├──> 有源蜂鸣器
                               └──> ESP8266 ──> Wi-Fi ──> OneNET
```

## 硬件清单

| 器件 | 说明 |
| --- | --- |
| 主控板 | STM32F103C8T6 最小系统板，Cortex-M3，72 MHz |
| 温湿度传感器 | DHT11，单总线数字输出 |
| 烟雾传感器 | MQ-2，模拟输出 |
| 火焰传感器 | 数字输出，低电平表示检测到火焰 |
| Wi-Fi 模块 | ESP8266，AT 固件，UART 通信 |
| 显示屏 | 0.96 英寸 128×64 OLED，SPI 接口 |
| 报警器 | 3.3 V/5 V 有源蜂鸣器，低电平触发 |
| 按键 | 一个轻触按键，低电平触发 |
| 调试/烧录 | ST-Link，SWD 接口 |

> MQ-2 的加热丝功耗较高，建议使用独立、足量的 5 V 电源。ESP8266 瞬时电流也可能超过 200 mA，不要仅依赖 STM32 板上的小电流 3.3 V 稳压器。所有进入 STM32 的信号都必须保持在 3.3 V 逻辑电平范围内。

## 引脚连接

| STM32 引脚 | 外设 | 功能/方向 |
| --- | --- | --- |
| PA1 | MQ-2 AO | ADC1_IN1，模拟输入 |
| PA2 | ESP8266 RX | USART2_TX，复用推挽输出 |
| PA3 | ESP8266 TX | USART2_RX，浮空输入 |
| PA4 | 火焰传感器 DO | GPIO 输入，上拉；低电平有效 |
| PB3 | 蜂鸣器 | GPIO 推挽输出，低电平触发 |
| PB5 | DHT11 DATA | 单总线，双向 GPIO |
| PB6 | OLED RES | GPIO 输出 |
| PB7 | 按键 | GPIO 输入，上拉；低电平按下 |
| PB8 | OLED SCLK | GPIO 输出 |
| PB9 | OLED SDIN | GPIO 输出 |
| PB12 | OLED DC | GPIO 输出 |
| PB13 | OLED CS | GPIO 输出 |
| PA13/PA14 | ST-Link SWDIO/SWCLK | 下载与调试 |

ESP8266 串口参数为 **115200 bps、8 数据位、1 停止位、无校验、无硬件流控**。

> ESP8266 的 TX 应连接 STM32 的 PA3，ESP8266 的 RX 应连接 STM32 的 PA2。供电和地线必须共地。

## 报警逻辑

系统满足以下任一条件时进入报警状态：

1. MQ-2 的 ADC 采样值大于 `2900`；
2. 火焰传感器输出低电平；
3. DHT11 温度大于 `28 ℃`。

报警时蜂鸣器持续鸣响，OLED 按优先级显示 `Flame alarm`、`Smoke alarm` 或 `Temperature alarm`。正常状态下显示 `Normal operation`。

按下 PB7 按键后，程序会关闭蜂鸣器并暂停约 10 秒。10 秒结束后重新检测传感器；如果火情或异常仍然存在，报警会再次触发。

相关参数可在以下位置修改：

- 温度阈值：`User/main.c` 中的 `temperature > 28`
- 烟雾阈值：`Hardware/mq2.h` 中的 `MQ2_THRESHOLD`
- 按键静音时间：`User/main.c` 中的 `Delay_ms(10000)`
- 数据上传周期：`User/main.c` 中的 `upload_timer >= 4`

> `MQ2_GetPercent()` 只是将 ADC 值线性换算为百分比，并未进行 ppm 标定。不同传感器模块的数值可能存在明显差异，实际使用前应在目标环境中预热并重新标定阈值。

## 开发环境

- Keil uVision 5
- ARM Compiler 5（工程使用 ARMCC V5.06 update 6）
- Keil.STM32F1xx_DFP 2.3.0
- STM32F10x Standard Peripheral Library
- ST-Link 驱动与调试工具

工程使用 `USE_STDPERIPH_DRIVER` 宏和标准外设库，不使用 HAL 库。工程目前配置为 ARMCC 5；如果使用 ARM Compiler 6，可能出现兼容性警告，建议在 Keil 中切换到 ARM Compiler 5。

## 编译与烧录

1. 使用 Keil uVision 5 打开根目录下的 `Project.uvprojx`。
2. 确认已经安装 STM32F1 器件支持包和 ARM Compiler 5。
3. 按 `F7` 或选择 `Project -> Build Target` 编译工程。
4. 编译成功后，固件输出到 `Objects/Project.hex`。
5. 使用 ST-Link 通过 SWD 接口连接开发板，在 Keil 中选择下载并运行。
6. 将 ESP8266、传感器和 OLED 按引脚表连接后重新上电。

如果只需要使用已经生成的固件，可以直接烧录 `Objects/Project.hex`。

## Wi-Fi 与 OneNET 配置

### 1. 配置 Wi-Fi

打开 `User/main.c`，将以下内容替换为可用的 2.4 GHz Wi-Fi 信息：

```c
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"
```

ESP8266 通常不支持 5 GHz Wi-Fi，请使用 2.4 GHz 网络。修改后需要重新编译并下载固件。

### 2. 配置 OneNET

打开 `Hardware/onenet.h`，核对以下参数：

```c
#define ONENET_DEVICE_ID  "设备名称"
#define ONENET_PRODUCT_ID "产品 ID"
#define ONENET_TOKEN      "设备鉴权 Token"
#define ONENET_SERVER     "mqtts.heclouds.com"
#define ONENET_PORT       1883
```

当前工程使用 OneNET 物模型的属性上报主题：

```text
$sys/{PRODUCT_ID}/{DEVICE_ID}/thing/property/post
```

上报周期约 4 秒，JSON 数据格式如下：

```json
{
  "id": "123",
  "version": "1.0",
  "params": {
    "temperature": { "value": 25 },
    "humidity": { "value": 60 },
    "smoke": { "value": 12 },
    "flame": { "value": 0 },
    "alarm": { "value": 0 }
  }
}
```

OneNET 物模型中的数据标识应保持为：

| 标识符 | 类型 | 含义 |
| --- | --- | --- |
| `temperature` | 整数 | 温度，单位 ℃ |
| `humidity` | 整数 | 湿度，单位 % |
| `smoke` | 整数 | 烟雾 ADC 换算百分比 |
| `flame` | 整数 | `1` 表示检测到火焰，`0` 表示未检测到 |
| `alarm` | 整数 | `1` 表示报警，`0` 表示正常 |

> 工程中的 OneNET 设备 ID、产品 ID 和 Token 目前直接写在源码里。公开仓库前请删除真实凭据，并改用示例值或个人私有配置文件。Token 具有有效期，过期后需要重新生成。

## 运行流程

1. 初始化系统时钟、SysTick、ADC、GPIO、OLED、DHT11、MQ-2、蜂鸣器、火焰传感器和按键。
2. 初始化 ESP8266，配置为 Station 模式并连接 Wi-Fi。
3. 连接 OneNET MQTT 服务器，发送 MQTT CONNECT 报文。
4. OLED 显示 Wi-Fi/云平台连接结果。
5. 主循环每秒读取 DHT11、MQ-2 和火焰传感器。
6. 根据传感器状态更新蜂鸣器和 OLED。
7. 每经过 4 个主循环周期，向 OneNET 上传一次属性数据。

启动时如果云端连接失败，本地采集和报警功能仍会继续运行，但不会上传数据，主程序也不会自动重连云端。

## 目录结构

```text
FireAlarm/
├── Hardware/                 # 传感器、OLED、ESP8266、MQTT 和 OneNET 驱动
│   ├── beep.c/.h             # 蜂鸣器
│   ├── dht11.c/.h            # DHT11 温湿度传感器
│   ├── esp8266.c/.h          # ESP8266 AT 指令与串口接收
│   ├── flame.c/.h            # 火焰传感器
│   ├── Key.c/.h              # 按键
│   ├── mq2.c/.h              # MQ-2 烟雾传感器与 ADC
│   ├── mqtt.c/.h             # 精简 MQTT 报文封装
│   ├── oled.c/.h             # OLED 驱动
│   ├── onenet.c/.h           # OneNET 初始化与属性上报
│   └── system.c/.h           # 系统初始化与延时
├── Library/                  # STM32F10x 标准外设库
├── Start/                    # 启动文件、CMSIS 和芯片头文件
├── System/                   # 系统辅助代码
├── User/                     # 主程序和中断文件
├── Objects/                  # 编译输出，包含 Project.hex
├── Project.uvprojx           # Keil 工程文件
└── README.md
```

`原理图/` 目录中保存了开发板原理图和接线截图，可作为硬件连接参考。

## 常见问题

### OLED 无显示

- 检查 OLED 是否使用 3.3 V 供电，并确认 GND 共地。
- 检查 RES、DC、CS、SCLK、SDIN 是否与引脚表一致。
- 部分 OLED 模块的 CS 可直接接地；当前工程使用 PB13 控制 CS。
- 如果显示方向相反，可调用 `OLED_DisplayTurn()` 调整。

### DHT11 读数始终为 0 或读取失败

- DHT11 的数据线通常需要 4.7 kΩ～10 kΩ 上拉电阻，带模块的版本一般已经集成。
- 检查供电电压和引脚 PB5。
- DHT11 采样间隔不宜过短，当前程序每秒读取一次。
- 启动后第一秒内的数据可能无效，应以稳定后的读数为准。

### MQ-2 数值异常或一直报警

- MQ-2 首次使用需要预热，建议预热 2～3 分钟后再进行判断。
- 检查模块 AO 是否连接到 PA1，且电压没有超过 3.3 V。
- 根据实际环境重新设置 `MQ2_THRESHOLD`。
- 线性百分比不等于精确烟雾浓度，不能直接当作 ppm 或安全标准使用。

### 火焰传感器误报

- 当前代码按低电平有效判断火焰。
- 如果模块逻辑相反，需要修改 `Hardware/flame.h` 中的 `FLAME_DETECTED()`。
- 避免强光直射和电源干扰，必要时增加滤波或多次采样确认。

### Wi-Fi 或 OneNET 初始化失败

- 确认使用的是 2.4 GHz Wi-Fi，SSID 和密码正确。
- 确认 ESP8266 已烧录支持 `AT+CWMODE`、`AT+CWJAP`、`AT+CIPSTART` 和 `AT+CIPSEND` 的 AT 固件。
- 检查 PA2/PA3 是否交叉连接、波特率是否为 115200、ESP8266 供电是否稳定。
- 检查 OneNET 产品 ID、设备 ID、Token、服务器地址和端口。
- Token 过期、设备被删除或物模型标识符不匹配都会导致连接或数据上传失败。

### 无法下载程序

- 确认 ST-Link 已连接 SWDIO、SWCLK、GND 和 3.3 V。
- 工程关闭了 JTAG，但保留了 SWD，因此 PA13/PA14 仍可用于 ST-Link 下载。
- 检查 Keil 中是否选择了正确的调试器和 STM32F103C8 器件。

## 当前限制与改进方向

- AT 指令、Wi-Fi 连接和传感器读取均采用阻塞方式，网络异常时会增加主循环延迟。
- 云端初始化失败后没有自动重连机制，可以增加断线检测、MQTT 心跳和重连状态机。
- MQ-2 尚未做温度补偿和 ppm 标定，只适合作为相对烟雾趋势判断。
- DHT11 精度和分辨率有限，如有需要可更换 DHT22、SHT30 等传感器。
- 目前仅支持 OneNET 属性上报，尚未实现云端远程配置或命令下发。
- Wi-Fi 凭据和 Token 直接存放在源码中，量产项目应改为安全配置或加密存储。
- 蜂鸣器静音依赖阻塞式延时，后续可以改为定时器计时，避免影响传感器采样。

## 安全提示

本项目用于学习、课程设计和原型验证，不能替代符合国家或行业标准的消防报警设备。实际消防场景应使用经过认证的探测器、冗余电源、故障监测和可靠的报警联动方案。

## 许可证

仓库中暂未包含开源许可证文件。如需公开发布，请根据使用需求补充 MIT、Apache-2.0 或其他合适的许可证。
