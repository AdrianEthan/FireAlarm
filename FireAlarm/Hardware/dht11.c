#include "dht11.h"

static void DHT11_Mode_Out(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

static void DHT11_Mode_In(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

static uint8_t DHT11_ReadBit(void)
{
    uint8_t retry = 0;
    while (DHT11_DATA_IN() == Bit_SET && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    retry = 0;
    while (DHT11_DATA_IN() == Bit_RESET && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    Delay_us(40);
    if (DHT11_DATA_IN() == Bit_SET)
        return 1;
    else
        return 0;
}

static uint8_t DHT11_ReadByte(void)
{
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= DHT11_ReadBit();
    }
    return data;
}

void DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(DHT11_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);

    DHT11_DATA_OUT(1);
}

uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i;
    uint8_t retry;

    DHT11_Mode_Out();
    DHT11_DATA_OUT(0);
    Delay_ms(20);
    DHT11_DATA_OUT(1);
    Delay_us(30);

    DHT11_Mode_In();

    retry = 0;
    while (DHT11_DATA_IN() == Bit_SET && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    if (retry >= 100) return 1;

    retry = 0;
    while (DHT11_DATA_IN() == Bit_RESET && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    if (retry >= 100) return 1;

    retry = 0;
    while (DHT11_DATA_IN() == Bit_SET && retry < 100)
    {
        retry++;
        Delay_us(1);
    }
    if (retry >= 100) return 1;

    for (i = 0; i < 5; i++)
    {
        buf[i] = DHT11_ReadByte();
    }

    DHT11_Mode_Out();
    DHT11_DATA_OUT(1);

    if (buf[0] + buf[1] + buf[2] + buf[3] == buf[4])
    {
        *humi = buf[0];
        *temp = buf[2];
        return 0;
    }
    return 1;
}
