#include "mqtt.h"

void MQTT_PacketInit(MQTT_Packet *packet, uint8_t *buf, uint16_t size)
{
    packet->buf = buf;
    packet->len = size;
    packet->pos = 0;
}

void MQTT_PacketWriteByte(MQTT_Packet *packet, uint8_t byte)
{
    if(packet->pos < packet->len)
    {
        packet->buf[packet->pos++] = byte;
    }
}

void MQTT_PacketWriteString(MQTT_Packet *packet, char *str)
{
    uint16_t len = strlen(str);
    MQTT_PacketWriteByte(packet, len >> 8);
    MQTT_PacketWriteByte(packet, len & 0xFF);
    while(*str)
    {
        MQTT_PacketWriteByte(packet, *str++);
    }
}

void MQTT_PacketWriteRemainLen(MQTT_Packet *packet, uint16_t len)
{
    do {
        uint8_t byte = len & 0x7F;
        len >>= 7;
        if(len > 0) byte |= 0x80;
        MQTT_PacketWriteByte(packet, byte);
    } while(len > 0);
}

uint16_t MQTT_PacketConnect(uint8_t *buf, char *clientid, char *username, char *password)
{
    MQTT_Packet packet;
    uint16_t remaining_len;
    uint16_t payload_len;
    uint8_t connect_flags = 0x02;

    payload_len = 2 + strlen(clientid);
    if(username != NULL) {
        payload_len += 2 + strlen(username);
        connect_flags |= 0x80;
    }
    if(password != NULL) {
        payload_len += 2 + strlen(password);
        connect_flags |= 0x40;
    }

    remaining_len = 10 + payload_len;

    MQTT_PacketInit(&packet, buf, 512);

    MQTT_PacketWriteByte(&packet, MQTT_CONNECT);
    MQTT_PacketWriteRemainLen(&packet, remaining_len);

    MQTT_PacketWriteString(&packet, "MQTT");
    MQTT_PacketWriteByte(&packet, 0x04);
    MQTT_PacketWriteByte(&packet, connect_flags);
    MQTT_PacketWriteByte(&packet, 0x00);
    MQTT_PacketWriteByte(&packet, 0x3C);

    MQTT_PacketWriteString(&packet, clientid);
    if(username != NULL)
        MQTT_PacketWriteString(&packet, username);
    if(password != NULL)
        MQTT_PacketWriteString(&packet, password);

    return packet.pos;
}

uint16_t MQTT_PacketPublish(uint8_t *buf, char *topic, char *msg)
{
    MQTT_Packet packet;
    uint16_t remaining_len;
    uint16_t i;

    remaining_len = 2 + strlen(topic) + strlen(msg);

    MQTT_PacketInit(&packet, buf, 512);

    MQTT_PacketWriteByte(&packet, MQTT_PUBLISH);
    MQTT_PacketWriteRemainLen(&packet, remaining_len);

    MQTT_PacketWriteString(&packet, topic);

    for(i = 0; i < strlen(msg); i++)
    {
        MQTT_PacketWriteByte(&packet, msg[i]);
    }

    return packet.pos;
}

uint16_t MQTT_PacketSubscribe(uint8_t *buf, char *topic)
{
    MQTT_Packet packet;
    uint16_t remaining_len;

    remaining_len = 2 + 2 + strlen(topic) + 1;

    MQTT_PacketInit(&packet, buf, 512);

    MQTT_PacketWriteByte(&packet, MQTT_SUBSCRIBE);
    MQTT_PacketWriteRemainLen(&packet, remaining_len);

    MQTT_PacketWriteByte(&packet, 0x00);
    MQTT_PacketWriteByte(&packet, 0x01);

    MQTT_PacketWriteString(&packet, topic);
    MQTT_PacketWriteByte(&packet, 0x00);

    return packet.pos;
}

uint16_t MQTT_PacketPing(uint8_t *buf)
{
    buf[0] = MQTT_PINGREQ;
    buf[1] = 0x00;
    return 2;
}
