/**********************************************************
										 ��������					 
���ܣ�����ADF4351���໷ģ�����35~4400M HzƵ��
�ӿڣ�
ʱ�䣺2017/1/6
�汾��2.1.5
���ߣ���������
������

������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com/

**********************************************************/

#include "delay.h"
#include "key.h"
#include "oled.h"
#include "HMC830.h"
#include "task_manage.h"
int main(void)
{		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	key_init();
	OLED_Init();			 //��ʼ��OLED 
	OLED_Clear();
	welcome_KW();
	Hmc830Config();
	Hmc830FreWrite(25000000);
	while(1)
	{
		//KeyRead();		

		//delay_ms(100);
		
		
		//Set_PointFre(Keycode);//��Ƶ����						

		
		
		
		//KEY_EXIT();
	}
}





















