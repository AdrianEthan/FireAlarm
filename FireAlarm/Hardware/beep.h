#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"

#define BEEP_PORT           GPIOB
#define BEEP_PIN            GPIO_Pin_12
#define BEEP_RCC            RCC_APB2Periph_GPIOB

#define BEEP_ON()           GPIO_ResetBits(BEEP_PORT, BEEP_PIN)
#define BEEP_OFF()          GPIO_SetBits(BEEP_PORT, BEEP_PIN)

void BEEP_Init(void);
void BEEP_Alarm(void);
void BEEP_Stop(void);

#endif
