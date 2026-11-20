#ifndef __FLAME_H
#define __FLAME_H

#include "system.h"

#define FLAME_GPIO_PORT GPIOA
#define FLAME_GPIO_PIN GPIO_Pin_4
#define FLAME_GPIO_CLK RCC_APB2Periph_GPIOA

#define FLAME_DETECTED() (GPIO_ReadInputDataBit(FLAME_GPIO_PORT, FLAME_GPIO_PIN) == Bit_RESET)

void FLAME_Init(void);
uint8_t FLAME_Detect(void);

#endif
