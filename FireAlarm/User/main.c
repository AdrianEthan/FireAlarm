#include "system.h"
#include "oled.h"
#include "dht11.h"
#include "mq2.h"
#include "beep.h"
#include "flame.h"
#include "Key.h"
#include "onenet.h"

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

uint8_t temperature = 0;
uint8_t humidity = 0;
uint8_t smoke_percent = 0;
uint8_t flame_detected = 0;
uint8_t alarm_status = 0;
uint32_t upload_timer = 0;

void Hardware_Init(void);
void Display_Update(void);
void Alarm_Check(void);

int main(void)
{

    uint8_t onenet_status = 0;

    System_Init();
    Hardware_Init();

    OLED_Clear();
    OLED_ShowCH(0, 0, "");
    Delay_ms(1000);

    OLED_Clear();
    OLED_ShowCH(0, 0, "wifi...");
    onenet_status = OneNet_Init(WIFI_SSID, WIFI_PASSWORD);

    if(onenet_status == 0)
    {
        OLED_Clear();
        OLED_ShowCH(0, 0, "success");
        Delay_ms(1000);
    }
    else
    {
        OLED_Clear();
        OLED_ShowCH(0, 0, "lose");
        Delay_ms(2000);
    }

    OLED_Clear();
    OLED_ShowCH(0, 0, "smoke");
    OLED_ShowChar(32, 0, ':');
    OLED_ShowChar(64, 0, '%');

    OLED_ShowCH(0, 2, "temp");
    OLED_ShowChar(32, 2, ':');
    OLED_ShowChar(64, 2, 'C');

    OLED_ShowCH(0, 4, "humi");
    OLED_ShowChar(32, 4, ':');
    OLED_ShowChar(64, 4, '%');

    while (1)
    {


        if(DHT11_Read_Data(&temperature, &humidity) == 0)
        {
            OLED_ShowNum(40, 2, temperature, 2, 1);
            OLED_ShowNum(40, 4, humidity, 2, 1);
        }

        smoke_percent = MQ2_GetPercent();
        OLED_ShowNum(40, 0, smoke_percent, 2, 1);

        flame_detected = FLAME_Detect();

        Alarm_Check();
        Display_Update();

        upload_timer++;
        if(upload_timer >= 4 && onenet_status == 0)
        {
            upload_timer = 0;
            OneNet_SendData(temperature, humidity, smoke_percent, flame_detected, alarm_status);
        }

        Delay_ms(1000);
    }
}

void Hardware_Init(void)
{
    OLED_Init();
    DHT11_Init();
    MQ2_Init();
    BEEP_Init();
    FLAME_Init();
    Key_Init();
}

void Display_Update(void)
{
    if(alarm_status)
    {
        if(flame_detected)
        {
            OLED_ShowCH(0, 6, "Flame alarm");
        }
        else if(MQ2_DetectSmoke())
        {
            OLED_ShowCH(0, 6, " Smoke alarm");
        }
        else if(temperature > 28)
        {
            OLED_ShowCH(0, 6, "Temperature alarm");
        }
    }
    else
    {
        OLED_ShowCH(0, 6, "Normal operation");
    }
}

void Alarm_Check(void)
{
	uint8_t key_num = 0;
	key_num = Key_GetNum();
    if(key_num == 1)
        {
            alarm_status = 0;
            BEEP_Stop();
			Delay_ms(10000);
        }
	else if(flame_detected || MQ2_DetectSmoke() || temperature > 28)
		{
			alarm_status = 1;
			BEEP_Alarm();
		}
	else
		{
        alarm_status = 0;
        BEEP_Stop();
		}
}
