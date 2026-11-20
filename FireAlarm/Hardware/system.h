#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 系统时钟频率
#define SYSTEM_CLOCK 72000000

// 延时函数声明
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

// 系统初始化
void System_Init(void);

#endif
