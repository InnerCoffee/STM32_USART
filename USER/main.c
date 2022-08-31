/**********************************************************
										 康威电子					 
功能：利用ADF4351锁相环模块输出35~4400M Hz频率
接口：
时间：2017/1/6
版本：2.1.5
作者：康威电子
其他：

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/

**********************************************************/

#include "delay.h"
#include "key.h"
#include "oled.h"
#include "HMC830.h"
#include "task_manage.h"
int main(void)
{		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	key_init();
	OLED_Init();			 //初始化OLED 
	OLED_Clear();
	welcome_KW();
	Hmc830Config();
	Hmc830FreWrite(25000000);
	while(1)
	{
		//KeyRead();		

		//delay_ms(100);
		
		
		//Set_PointFre(Keycode);//点频设置						

		
		
		
		//KEY_EXIT();
	}
}





















