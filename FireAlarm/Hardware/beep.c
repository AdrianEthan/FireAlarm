#include "beep.h"
#include "delay.h"

void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(BEEP_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);

    BEEP_OFF();
}

void BEEP_Alarm(void)
{
    BEEP_ON();
}

void BEEP_Stop(void)
{
    BEEP_OFF();
}
