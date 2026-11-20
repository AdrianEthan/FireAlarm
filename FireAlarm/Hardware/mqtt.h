#ifndef __MQTT_H
#define __MQTT_H

#include "system.h"

#define MQTT_CONNECT 0x10
#define MQTT_CONNACK 0x20
#define MQTT_PUBLISH 0x30
#define MQTT_PUBACK 0x40
#define MQTT_SUBSCRIBE 0x82
#define MQTT_SUBACK 0x90
#define MQTT_PINGREQ 0xC0
#define MQTT_PINGRESP 0xD0

typedef struct {
    uint8_t *buf;
    uint16_t len;
    uint16_t pos;
} MQTT_Packet;

void MQTT_PacketInit(MQTT_Packet *packet, uint8_t *buf, uint16_t size);
void MQTT_PacketWriteByte(MQTT_Packet *packet, uint8_t byte);
void MQTT_PacketWriteString(MQTT_Packet *packet, char *str);
void MQTT_PacketWriteRemainLen(MQTT_Packet *packet, uint16_t len);
uint16_t MQTT_PacketConnect(uint8_t *buf, char *clientid, char *username, char *password);
uint16_t MQTT_PacketPublish(uint8_t *buf, char *topic, char *msg);
uint16_t MQTT_PacketSubscribe(uint8_t *buf, char *topic);
uint16_t MQTT_PacketPing(uint8_t *buf);

#endif
