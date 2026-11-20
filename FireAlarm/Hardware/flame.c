#include "flame.h"

void FLAME_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(FLAME_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = FLAME_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(FLAME_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t FLAME_Detect(void)
{
    if(FLAME_DETECTED())
    {
        return 1;
    }
    return 0;
}
