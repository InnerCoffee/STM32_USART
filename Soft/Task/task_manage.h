#ifndef __task_manage_H
#define __task_manage_H
#include "stm32f10x.h"
#include "oled.h"
#include "key.h"
#include "stm32f10x_it.h" 
#define OUT_KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//¶ÁÈ¡°´¼ü0

#define MIN_FRE	25000000
#define MAX_FRE	3000000000


extern u32 SysTimer;

u8 TaskCycleDelay(u32 delay_time, u8* Last_delay_ID, u8* Self_delay_ID);
void welcome_KW(void);
void Copybuf2dis(u8 source[5], u8 dis[10], u8  dispoint);

void Set_PointFre(u32 Key_Value);//µãÆµ

uint32_t AddCarry(uint64_t newNum, uint8_t pos, uint64_t MaxNum, uint64_t MinNum);
uint32_t SubbCarry(uint64_t newNum, uint8_t pos, uint64_t MaxNum, uint64_t MinNum);

#endif
