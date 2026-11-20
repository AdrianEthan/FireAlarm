# 火灾报警监控

基于 STM32F103C8T6 的火灾监控与可视化系统，使用 DHT11、MQ-2、火焰传感器、OLED、ESP8266 和 OneNET 实现本地报警与云端数据上报。

## 项目文档

完整的硬件连接、编译烧录、OneNET 配置和故障排查说明请查看：

- [项目说明文档](FireAlarm/README.md)
- [Keil 工程](FireAlarm/Project.uvprojx)

## 主要功能

- 温度和湿度采集
- 烟雾浓度检测
- 明火检测
- OLED 实时显示
- 蜂鸣器声光报警
- ESP8266 连接 Wi-Fi
- MQTT 上传数据到 OneNET

## 开发环境

- Keil uVision 5
- ARM Compiler 5
- STM32F1xx Device Family Pack
- STM32F10x Standard Peripheral Library
