#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "demo131a04.h" 
#include "mytim.h"
#include "mykey.h"
#define JIZHUN 50000
u16 mai_priod=100;

u16 mai_con=JIZHUN/100;

 int main(void)
 {	
	u8 key_temp=0;
	delay_init();	    	 //延时函数初始化	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);	 
	TIM4_Init(144-1,10-1);//
	LED_Init();		  	//初始化与LED连接的硬件接口
	KEY_Init();
	 LED1=0;
	while(1)
	{
//		demo_ads131a0x();
		LED0=!LED0;
		key_temp=KEY_Scan(0);
		if(key_temp==1)
		{
			mai_priod-=100;
			if(mai_priod<100)
				mai_priod=100;
			mai_con=JIZHUN/mai_priod;
		}
		else if(key_temp==2)
		{
			mai_priod+=100;
			if(mai_priod>2000)
				mai_priod=2000;
			mai_con=JIZHUN/mai_priod;
		}

	}
 }

