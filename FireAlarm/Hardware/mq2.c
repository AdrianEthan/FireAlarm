#include "mq2.h"

void MQ2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(MQ2_ADC_RCC | RCC_APB2Periph_ADC1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = MQ2_ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(MQ2_ADC_PORT, &GPIO_InitStructure);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

uint16_t MQ2_GetValue(void)
{
    ADC_RegularChannelConfig(ADC1, MQ2_ADC_CHANNEL, 1, ADC_SampleTime_55Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    return ADC_GetConversionValue(ADC1);
}

uint8_t MQ2_DetectSmoke(void)
{
    uint16_t value = MQ2_GetValue();
    if(value > MQ2_THRESHOLD)
    {
        return 1;
    }
    return 0;
}

uint8_t MQ2_GetPercent(void)
{
    uint16_t value = MQ2_GetValue();
    uint32_t percent = (uint32_t)value * 100 / 4095;
    if(percent > 100) percent = 100;
    return (uint8_t)percent;
}
