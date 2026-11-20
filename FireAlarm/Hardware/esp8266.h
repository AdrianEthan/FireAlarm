#ifndef __ESP8266_H
#define __ESP8266_H

#include "system.h"

#define ESP8266_USART USART2
#define ESP8266_USART_CLK RCC_APB1Periph_USART2
#define ESP8266_GPIO_CLK RCC_APB2Periph_GPIOA
#define ESP8266_GPIO_PORT GPIOA
#define ESP8266_TX_PIN GPIO_Pin_2
#define ESP8266_RX_PIN GPIO_Pin_3

#define ESP8266_RX_BUF_SIZE 1024

void ESP8266_Init(void);
void ESP8266_SendString(char *str);
void ESP8266_SendCmd(char *cmd, char *ack, uint16_t timeout);
uint8_t ESP8266_ConnectWiFi(char *ssid, char *password);
uint8_t ESP8266_ConnectServer(char *protocol, char *ip, uint16_t port);
void ESP8266_SendData(uint8_t *data, uint16_t len);
void ESP8266_Clear(void);

extern char ESP8266_RxBuffer[ESP8266_RX_BUF_SIZE];
extern uint16_t ESP8266_RxCounter;

#endif
