#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "demo131a04.h" 
*********



 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	 
	uart_init(115200);	 
	LED_Init();		  	//初始化与LED连接的硬件接口
	while(1)
	{
		demo_ads131a0x();
		LED0=!LED0;
		delay_ms(1000);	//延时300ms
		LED1=!LED1;
	}
 }

