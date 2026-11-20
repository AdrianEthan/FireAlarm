#ifndef __DHT11_H
#define __DHT11_H

#include "system.h"

// DHT11连接引脚 PB5
#define DHT11_GPIO_PORT GPIOB
#define DHT11_GPIO_PIN GPIO_Pin_5
#define DHT11_GPIO_CLK RCC_APB2Periph_GPIOB

// DHT11引脚操作
#define DHT11_DATA_OUT(x) GPIO_WriteBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN, (BitAction)(x))
#define DHT11_DATA_IN() GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

// 函数声明
void DHT11_Init(void);
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi);

#endif
