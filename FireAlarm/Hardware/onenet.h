#ifndef __ONENET_H
#define __ONENET_H

#include "system.h"
#include "esp8266.h"
#include "mqtt.h"

#define ONENET_DEVICE_ID "FireAlarm"
#define ONENET_PRODUCT_ID "fYvwbXeM62"
#define ONENET_TOKEN "YOUR_ONENET_TOKEN"
#define ONENET_SERVER "mqtts.heclouds.com"
#define ONENET_PORT 1883

#define ONENET_TOPIC_PUBLISH "$sys/" ONENET_PRODUCT_ID "/" ONENET_DEVICE_ID "/thing/property/post"

uint8_t OneNet_Init(char *wifi_ssid, char *wifi_password);
void OneNet_SendData(uint8_t temp, uint8_t humi, uint8_t smoke, uint8_t flame, uint8_t alarm);
void OneNet_Publish(char *topic, char *msg);

#endif
