#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	  
#include "demo131a04.h" 
*********



 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	 
	uart_init(115200);	 
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	while(1)
	{
		demo_ads131a0x();
		LED0=!LED0;
		delay_ms(1000);	//��ʱ300ms
		LED1=!LED1;
	}
 }

