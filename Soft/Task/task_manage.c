#include "task_manage.h"
#include "HMC830.h"
#include "delay.h"

u8 init_display = 1;
u8 Task_Index = 0;

u8 TaskCycleDelay(u32 delay_time, u8* Last_delay_ID, u8* Self_delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
	
	if(!(*Last_delay_ID))
		return 0;
	if(Time_Get == 0)
	{
		Time_Triger = SysTimer + delay_time;
		Time_Get = 1;
	}
	if(SysTimer >= Time_Triger)
	{ 
		Time_Get = 0;
		*Last_delay_ID = 0;
		*Self_delay_ID = 1;		//	后续代码已被执行一遍
		return 1;
	}
	return 0;
}
void welcome_KW(void)
{
	//u8 delay_ID[11] = {0,0,0,0,0,0,0,0,0,0};
	//u8 delay_ID_Welcome[3] = {0,0,0};
	//u16 Wel_time0 = 10,Wel_time1 = 50;
	//delay_ID[10] = 1;
	//delay_ID_Welcome[2] = 1;
	
	LCD_Show_CEStr(0,6,"HMC830");
	LCD_Show_CEStr(56,6,"init");
	
	// each char is 8 in width
	LCD_Show_CEStr(32,0,"Chad HMC830");
	LCD_Show_CEStr(0,2,"Thundercock");
	LCD_Show_CEStr(0,4,"1234567890123456");
	LCD_Show_CEStr(0,6,"8===========D---");
	
	delay_ms(1000);
	
	/*
	while(1)
	{
	if(TaskCycleDelay(Wel_time0, &delay_ID[10], &delay_ID[0]))
		LCD_Show_CEStr(32,0,"w");
	if(TaskCycleDelay(Wel_time0, &delay_ID[0], &delay_ID[1]))
		LCD_Show_CEStr(48,0,"h");
	if(TaskCycleDelay(Wel_time0, &delay_ID[1], &delay_ID[2]))
		LCD_Show_CEStr(64,0,"a");
	if(TaskCycleDelay(Wel_time0, &delay_ID[2], &delay_ID[3]))
		LCD_Show_CEStr(80,0,"t");
	if(TaskCycleDelay(Wel_time0, &delay_ID[3], &delay_ID[4]))
		LCD_Show_CEStr(16,2," ");
	if(TaskCycleDelay(Wel_time0, &delay_ID[4], &delay_ID[5]))
		LCD_Show_CEStr(32,2,"t");
	if(TaskCycleDelay(Wel_time0, &delay_ID[5], &delay_ID[6]))
		LCD_Show_CEStr(48,2,"h");
	if(TaskCycleDelay(Wel_time0, &delay_ID[6], &delay_ID[7]))
		LCD_Show_CEStr(64,2,"e");
	if(TaskCycleDelay(Wel_time0, &delay_ID[7], &delay_ID[8]))
		LCD_Show_CEStr(80,2," ");
	if(TaskCycleDelay(Wel_time0, &delay_ID[8], &delay_ID[9]))
		LCD_Show_CEStr(96,2,"f");
	if(TaskCycleDelay(Wel_time0, &delay_ID[9], &delay_ID[0]))
	{
		delay_ms(100);
		break;
	}
	if(TaskCycleDelay(Wel_time1, &delay_ID_Welcome[2], &delay_ID_Welcome[0]))
		LCD_Show_CEStr(104,6,".  ");
	if(TaskCycleDelay(Wel_time1, &delay_ID_Welcome[0], &delay_ID_Welcome[1]))
		LCD_Show_CEStr(112,6,".");
	if(TaskCycleDelay(Wel_time1, &delay_ID_Welcome[1], &delay_ID_Welcome[2]))
		LCD_Show_CEStr(120,6,".");
	}
	*/
}

void Copybuf2dis(u8 *source, u8 dis[10], u8  dispoint)
{
	dis[0] = *source + '0';
	dis[1] = ',';
	dis[2] = *(source+1) + '0';
	dis[3] = *(source+2) + '0';
	dis[4] = *(source+3) + '0';
	dis[5] = ',';
	dis[6] = *(source+4) + '0';
	dis[7] = *(source+5) + '0';
	dis[8] = *(source+6) + '0';
	dis[9] = ',';
	dis[10] = *(source+7) + '0';
	dis[11] = *(source+8) + '0';
	dis[12] = *(source+9) + '0';
	dis[13] = 'H';
	dis[14] = 'z';
	dis[15] = 0;

	if(dispoint < 1) dis[dispoint] += 128;
	else if(dispoint < 4) dis[dispoint+1] += 128;
	else if(dispoint < 7) dis[dispoint+2] += 128;
	else if(dispoint < 10) dis[dispoint+3] += 128;
}

void Set_PointFre(u32 Key_Value)//设置点频
{ 
	static u32 Fre = 25000000;
	static u8 P_Index = 0;
	u8 fre_buf[10];
	u8 display[10];
	if(init_display) 
	{
		Key_Value = K_4_L;
		OLED_Clear();
		
		
		LCD_Show_CEStr(32,0,"Freq set");
		// left up down right arrows
		LCD_Show_CEStr(0,6," ←  ↑  ↓  → ");
		init_display = 0;
	}
	
	switch(Key_Value)
	{
		case K_1_S: P_Index++;break;
		case K_1_L: P_Index++;break;
		case K_3_S: P_Index--;break;
		case K_3_L: P_Index--;break;
		case K_2_S: Fre=AddCarry(Fre,  9-P_Index, MAX_FRE, MIN_FRE);break;
		case K_2_L: Fre=AddCarry(Fre,  9-P_Index, MAX_FRE, MIN_FRE);break;
		case K_5_S: Fre=SubbCarry(Fre, 9- P_Index, MAX_FRE, MIN_FRE);break;
		case K_5_L: Fre=SubbCarry(Fre,  9-P_Index, MAX_FRE, MIN_FRE);break;
	}
	if(P_Index==255)
		P_Index=9;
	else if(P_Index==10) P_Index=0;
	
	if(Key_Value != K_NO)
	{
		fre_buf[0] = (u32)Fre%10000000000/1000000000;
		fre_buf[1] = (u32)Fre%1000000000/100000000;
		fre_buf[2] = (u32)Fre%100000000/10000000;
		fre_buf[3] = (u32)Fre%10000000/1000000;
		fre_buf[4] = (u32)Fre%1000000/100000;
		fre_buf[5] = (u32)Fre%100000/10000;
		fre_buf[6] = (u32)Fre%10000/1000;
		fre_buf[7] = (u32)Fre%1000/100;
		fre_buf[8] = (u32)Fre%100/10;
		fre_buf[9] = (u32)Fre%10;
		Copybuf2dis(fre_buf, display, P_Index);
		OLED_ShowString(4, 2, display);	
		Hmc830FreWrite(Fre);
	}
}
//m^n函数
u32 mypow(u8 m,u8 n)
{
  u32 result=1;	 
  while(n--)result*=m;    
  return result;
}			 
/*
*********************************************************************************************************
*	函 数 名: AddCarry
*	功能说明: 加法进位操作
*	形    参: newNum， 被操作数
						pos，造作位置位于操作数的第pos位
									pos=0为最高位
						MaxNum， 被操作数的最大值
						MinNum， 被操作数的最小值
*	返 回 值: 进位，借位或无进位借位后的真实值
*********************************************************************************************************
*/
uint32_t AddCarry(uint64_t newNum, uint8_t pos, uint64_t MaxNum, uint64_t MinNum)
{
//	uint64_t numDiff;
//	uint64_t newNumAfCarry;
	
	newNum+=mypow(10,pos);
	if(newNum>=MaxNum)
		newNum=MaxNum;
	if(newNum<=MinNum)
		newNum=MinNum;

	return newNum;
}

/*
*********************************************************************************************************
*	函 数 名: SubbCarry
*	功能说明: 减法借位操作
*	形    参: newNum， 被操作数
						pos，造作位置位于操作数的第pos位
								pos=0为最高位
						MaxNum， 被操作数的最大值
						MinNum， 被操作数的最小值
*	返 回 值: 进位，借位或无进位借位后的真实值
*********************************************************************************************************
*/
uint32_t SubbCarry(uint64_t newNum, uint8_t pos, uint64_t MaxNum, uint64_t MinNum)
{
	newNum-=mypow(10,pos);
	if(newNum>=MaxNum)
		newNum=MinNum;
	if(newNum<=MinNum)
		newNum=MinNum;

	return newNum;
}










