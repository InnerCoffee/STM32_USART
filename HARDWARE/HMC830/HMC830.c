#include "HMC830.h"
#include "delay.h"

unsigned char dat_reg[4]={0};
unsigned char rfDividReg[3] = {0X00, 0XE0, 0X90};

void Hmc830Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PC端口时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
////	HMC830_SCK = 0;
	HMC830_SCK = 1;
//	HMC830_CEN = 0;
//	HMC830_SEN = 0;
////	HMC830_SCK = 0;
////	HMC830_SCK = 1;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE
	
	HMC830_CEN = 1;
	delay_ms(20);
	dat_reg[0]=0x00;         
	dat_reg[1]=0x00;         
	dat_reg[2]=0x20;//2;     
	spi_wr(0x00,dat_reg); //复位
	delay_ms(20);
} 
void _delay(unsigned int num)
{
	unsigned char i;
	unsigned int j;
	for(i=0;i<100;i++)
		for(j=0;j<num;j++);
}
void nop(unsigned int num)
{
	while(num)
  		num--;
}
 void spi_wr(unsigned char dat,unsigned char *p)
{
	unsigned char i,j;
	unsigned char dat_reg;

	HMC830_SEN = 1;
	dat=dat<<3;	

	
	for(j=0;j<3;j++)
	{
		dat_reg=*p;
		for(i=0;i<8;i++)
		{
			if(dat_reg & 0X80)
				HMC830_SDI = 1;
			else
				HMC830_SDI = 0;
			dat_reg <<= 1;
			nop(50);
			HMC830_SCK=0;
			nop(50);
			HMC830_SCK=1;
			nop(50);
			HMC830_SCK=0;
			nop(50);
		}
		p++;
	}
	HMC830_SEN = 0;
	nop(100);
	for(i=0;i<5;i++)
	{
		if(dat & 0X80)
				HMC830_SDI = 1;
			else
				HMC830_SDI = 0;
		dat<<=1;		
		HMC830_SCK=0;
		nop(50);
		HMC830_SCK=1;
		nop(50);
		HMC830_SCK=0;
		nop(50);
	}

	for(i=0;i<3;i++)
	{
		HMC830_SDI=0;		
		HMC830_SCK=0;
		nop(50);
		HMC830_SCK=1;
		nop(50);
		HMC830_SCK=0;
		nop(50);
	}
	HMC830_SEN=0;
	nop(50);
	HMC830_SEN=1;
	nop(50);
	HMC830_SEN=0;
	nop(50);
}


void hmc830_ini(unsigned char *p)
{
	unsigned char dat_reg[3]={0};
	unsigned char dat0;
	unsigned char dat1;
	unsigned char dat2;
	unsigned char dat3;																																						  
	
	dat0 = *p;

	p++;
	dat1 = *p;

	p++;
	dat2 = *p;

	p++;
	dat3 = *p;

	dat_reg[0]=0x00;         
	dat_reg[1]=0x00;         
	dat_reg[2]=0x02;//2;     
	spi_wr(0x01,dat_reg); 
	
	dat_reg[0]=0x00;
	dat_reg[1]=0x00;
	dat_reg[2]=0x01;//		  R分频设置100M鉴相0x01；50M 0x02； 25M0x04
	spi_wr(0x02,dat_reg);//
		
    
//	dat_reg[0]=0x00;//0x50;            
//	dat_reg[1]=0xF3;                //38分频  
//	dat_reg[2]=0x10; //e090                  
//	spi_wr(0x05,dat_reg);//输出1分频：E090	2分频 E110  4分频E210


	dat_reg[0]=rfDividReg[0];//0x50;            
	dat_reg[1]=rfDividReg[1];                //1分频  
	dat_reg[2]=rfDividReg[2]; //e090                  
	spi_wr(0x05,dat_reg);//输出1分频：E090	2分频 E110  4分频E210
	
//	dat_reg[0]=0x00;//0x50;            
//	dat_reg[1]=0xFF;                //62分频  
//	dat_reg[2]=0x10; //e090                  
//	spi_wr(0x05,dat_reg);//输出1分频：E090	2分频 E110  4分频E210
	
	dat_reg[0]=0x00;          
	dat_reg[1]=0x28;                  
	dat_reg[2]=0x98;                  
	spi_wr(0x05,dat_reg);//vco04 fo: 0x2a95; 2f0:0x2295;
	
	
	dat_reg[0]=0x00;//0x50;            
	dat_reg[1]=0x00;                  
	dat_reg[2]=0x00;                  
	spi_wr(0x05,dat_reg);//vco00
	
	dat_reg[0]=0x20;//20
	dat_reg[1]=0x0B;//0b
	dat_reg[2]=0x4A;//整数模式 0xcA;小数模式 0x4a
	spi_wr(0x06,dat_reg);//设置片子模式
	
	
	dat_reg[0]=0x00;                  
	dat_reg[1]=0x0D;//0X01;       
	dat_reg[2]=0xCD;
	spi_wr(0x07,dat_reg);

	dat_reg[0]=0x54;//0x54;
	dat_reg[1]=0x3F;//0x7f;
	dat_reg[2]=0xFF;//0xff;
	spi_wr(0x09,dat_reg);//初始电流400FFF--5FFFFF
	
	
	dat_reg[0]=0x00;  
	dat_reg[1]=0x20;  
	dat_reg[2]=0x46; //2005 
	spi_wr(0x0A,dat_reg);//Reg 0Ah CP Op Amp Register [2]
	
	dat_reg[0]=0x00;                   
	dat_reg[1]=0x00;                   
	dat_reg[2]=0x81;//                  
	spi_wr(0x0F,dat_reg);//

	dat_reg[0]=0x00;
	dat_reg[1]=0x00;
	dat_reg[2]=dat0;
	spi_wr(0x03,dat_reg); //数据正数部分

	dat_reg[0]=dat3;
	dat_reg[1]=dat2;
	dat_reg[2]=dat1;
	spi_wr(0x04,dat_reg); //数据正数部分
	
}
void WriteAFre(unsigned long int fre)
{
	unsigned int reg;
	unsigned char lock_dat;
	unsigned char dds_dat;

	unsigned long int fsysclk = 25000000;//50000000;	//鉴相频率50M/25M/100M
	unsigned long int data_24 = 0xFFFFFF;
	unsigned long int fdout;
	unsigned long int fdout1;

	unsigned long int res0,res1,sum0,sum,k;
	unsigned char j,i;
	
//	Hmc830Config();
//	_delay(50);
	
	fdout = fre;	
	fdout1 = fdout*1;	//4分频 *4 //2分频 *2


	dat_reg[0] = fdout1/fsysclk; //	   fsysclk=鉴相频率50M/25M/100M
	fdout = fdout1%fsysclk;

	sum=data_24/fsysclk;
	res0=data_24%fsysclk;
	sum=fdout*sum;

	for(i=0;i<7;i++)
	{
		j=i+1;
		k=1;
		while(j>0)
		{
			k=10*k;
			j--;
		}

		res1=res0%(fsysclk/k);
		res0=res0/(fsysclk/k);
		sum0=(res0*fdout)/k;
		res0=res1;
		if((res0>4)&&(res0<10))
			sum=sum+1;
		sum=sum+sum0;
	}
	
	dat_reg[1] = sum;
	sum>>=8;
	dat_reg[2] = sum;
	sum>>=8;
	dat_reg[3] = sum;

	hmc830_ini(dat_reg);
	_delay(50);
}
/*
*********************************************************************************************************
*	函 数 名: Hmc830FreWrite
*	功能说明: 直接输出25M~3G点频
*	形    参：wfre，输出频率值，单位：Hz
*	返 回 值: 无
*	作		者: 康威电子，https://kvdz.taobao.com/
*	时		间: 2016/04/18
*	备    注: 计算需要的输出分频系数，并将其倍频（范围1~62，即VCO分频系数），从而实现任意点频输出。
*********************************************************************************************************
*/
void Hmc830FreWrite(u32 wfre)
{
	u16 vco02reg = 0X6010;
	u16 rfdivid = 1;
	u8 noise_con = 0;
	u32 vco_fre;
	if(wfre > 3000000000)
		wfre = 3000000000;
	if(wfre < 25000000)
		wfre = 25000000;
	if(wfre > 1500000000) 
	{
		rfdivid = 1;
	}
	
	rfdivid = 3000000000/wfre;
	if(rfdivid < 2)
	{
		rfdivid = 1;
		noise_con = 1;
	}
	else if(rfdivid < 3)
	{
		rfdivid = 2;
		noise_con = 1;
	}
	else if(rfdivid > 62)
	{
		rfdivid = 62;
	}
	else
	{
		rfdivid = ((rfdivid%2)==0)?(rfdivid):(rfdivid-1);
	}
	if(rfdivid < 3)
	{
		noise_con = 1;
	}
	else
	{
		noise_con = 0;
	}
	if(noise_con) vco02reg |= (1<<15);
	else vco02reg &= ~(1<<15);
	vco02reg |= (rfdivid<<7);
	vco_fre = wfre*rfdivid;
	
	rfDividReg[1] = vco02reg>>8; 
	rfDividReg[2] = vco02reg&0xff;
	WriteAFre(vco_fre);
}


