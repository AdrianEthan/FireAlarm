#include "esp8266.h"

char ESP8266_RxBuffer[ESP8266_RX_BUF_SIZE];
uint16_t ESP8266_RxCounter = 0;

void ESP8266_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(ESP8266_GPIO_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(ESP8266_USART_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = ESP8266_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ESP8266_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ESP8266_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ESP8266_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(ESP8266_USART, &USART_InitStructure);

    USART_ITConfig(ESP8266_USART, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(ESP8266_USART, ENABLE);

    ESP8266_Clear();
}

void ESP8266_SendString(char *str)
{
    while(*str)
    {
        while(USART_GetFlagStatus(ESP8266_USART, USART_FLAG_TC) == RESET);
        USART_SendData(ESP8266_USART, *str++);
    }
}

void ESP8266_Clear(void)
{
    memset(ESP8266_RxBuffer, 0, ESP8266_RX_BUF_SIZE);
    ESP8266_RxCounter = 0;
}

void ESP8266_SendCmd(char *cmd, char *ack, uint16_t timeout)
{
    ESP8266_Clear();
    ESP8266_SendString(cmd);
    ESP8266_SendString("\r\n");

    if(ack == NULL) {
        Delay_ms(timeout);
        return;
    }

    while(timeout--)
    {
        Delay_ms(10);
        if(strstr(ESP8266_RxBuffer, ack) != NULL)
            break;
    }
}

uint8_t ESP8266_ConnectWiFi(char *ssid, char *password)
{
    char cmd[128];

    ESP8266_SendCmd("AT", "OK", 100);
    ESP8266_SendCmd("AT+CWMODE=1", "OK", 100);

    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
    ESP8266_SendCmd(cmd, "OK", 1000);

    if(strstr(ESP8266_RxBuffer, "OK") != NULL)
        return 0;
    return 1;
}

uint8_t ESP8266_ConnectServer(char *protocol, char *ip, uint16_t port)
{
    char cmd[128];

    sprintf(cmd, "AT+CIPSTART=\"%s\",\"%s\",%d", protocol, ip, port);
    ESP8266_SendCmd(cmd, "CONNECT", 500);

    if(strstr(ESP8266_RxBuffer, "CONNECT") != NULL)
        return 0;
    return 1;
}

void ESP8266_SendData(uint8_t *data, uint16_t len)
{
    char cmd[32];
    uint16_t i;

    sprintf(cmd, "AT+CIPSEND=%d", len);
    ESP8266_SendCmd(cmd, ">", 100);

    for(i = 0; i < len; i++)
    {
        while(USART_GetFlagStatus(ESP8266_USART, USART_FLAG_TC) == RESET);
        USART_SendData(ESP8266_USART, data[i]);
    }

    Delay_ms(100);
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(ESP8266_USART, USART_IT_RXNE) != RESET)
    {
        if(ESP8266_RxCounter < ESP8266_RX_BUF_SIZE)
        {
            ESP8266_RxBuffer[ESP8266_RxCounter++] = USART_ReceiveData(ESP8266_USART);
        }
        else
        {
            ESP8266_RxCounter = 0;
        }
        USART_ClearITPendingBit(ESP8266_USART, USART_IT_RXNE);
    }
}
