#include "onenet.h"

uint8_t OneNet_Init(char *wifi_ssid, char *wifi_password)
{
    uint8_t mqtt_buf[256];
    uint16_t mqtt_len;

    ESP8266_Init();
    Delay_ms(1000);

    ESP8266_SendCmd("AT+RST", "ready", 500);
    Delay_ms(2000);

    if(ESP8266_ConnectWiFi(wifi_ssid, wifi_password) != 0)
    {
        return 1;
    }

    Delay_ms(1000);

    if(ESP8266_ConnectServer("TCP", ONENET_SERVER, ONENET_PORT) != 0)
    {
        return 2;
    }

    Delay_ms(500);

    mqtt_len = MQTT_PacketConnect(mqtt_buf, ONENET_DEVICE_ID, ONENET_PRODUCT_ID, ONENET_TOKEN);
    ESP8266_SendData(mqtt_buf, mqtt_len);

    Delay_ms(1500);

    if(strstr(ESP8266_RxBuffer, "+IPD") != NULL)
    {
        return 0;
    }

    return 3;
}

void OneNet_Publish(char *topic, char *msg)
{
    uint8_t mqtt_buf[512];
    uint16_t mqtt_len;

    mqtt_len = MQTT_PacketPublish(mqtt_buf, topic, msg);
    ESP8266_SendData(mqtt_buf, mqtt_len);
}

void OneNet_SendData(uint8_t temp, uint8_t humi, uint8_t smoke, uint8_t flame, uint8_t alarm)
{
    char json_buf[256];

    sprintf(json_buf, "{\"id\":\"123\",\"version\":\"1.0\",\"params\":{\"temperature\":{\"value\":%d},\"humidity\":{\"value\":%d},\"smoke\":{\"value\":%d},\"flame\":{\"value\":%d},\"alarm\":{\"value\":%d}}}",
            temp, humi, smoke, flame, alarm);

    OneNet_Publish(ONENET_TOPIC_PUBLISH, json_buf);
}
