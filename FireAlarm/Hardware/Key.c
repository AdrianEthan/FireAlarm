#include "Key.h"

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(KEY_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = KEY_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;

    if (GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == 0)
    {
        Delay_ms(30);
        while (GPIO_ReadInputDataBit(KEY_GPIO_PORT, KEY_GPIO_PIN) == 0);
        Delay_ms(30);
        KeyNum = 1;
    }

    return KeyNum;
}
