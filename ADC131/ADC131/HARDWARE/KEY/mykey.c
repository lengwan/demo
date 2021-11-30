#include "mykey.h"
#include "delay.h"


void KEY_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //??PORTA,PORTE ??
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
 GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

//0 duan   1 lian
u8 KEY_Scan(u8 mode)
{
 static u8 key_up = 1;
 if(mode)key_up = 1;
 if(key_up && (KEY0==0||KEY1==0))
 {
  delay_ms(20);
  key_up = 0;
  if(KEY0==0) return 1;
  else if(KEY1==0) return 2;
  delay_ms(500); //????
 }
 else if(KEY0==1&& KEY1==1)
  key_up = 1;
 return 0;
}