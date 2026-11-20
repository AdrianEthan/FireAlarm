#ifndef __KEY_H
#define __KEY_H

#include "system.h"

#define KEY_GPIO_PORT GPIOB
#define KEY_GPIO_PIN GPIO_Pin_7
#define KEY_GPIO_CLK RCC_APB2Periph_GPIOB

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
