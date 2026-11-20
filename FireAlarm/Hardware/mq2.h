#ifndef __MQ2_H
#define __MQ2_H

#include "system.h"

#define MQ2_ADC_CHANNEL     ADC_Channel_1
#define MQ2_ADC_PORT        GPIOA
#define MQ2_ADC_PIN         GPIO_Pin_1
#define MQ2_ADC_RCC         RCC_APB2Periph_GPIOA

#define MQ2_THRESHOLD       2900

void MQ2_Init(void);
uint16_t MQ2_GetValue(void);
uint8_t MQ2_DetectSmoke(void);
uint8_t MQ2_GetPercent(void);

#endif
