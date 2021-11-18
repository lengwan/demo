#include "mytim.h"
#include "led.h"

extern u16 mai_con;
u16 now_mai=0;

void TIM4_Init(u16 per,u16 psc)
 {
		TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

		TIM_TimeBaseInitStructure.TIM_Period=per;
		TIM_TimeBaseInitStructure.TIM_Prescaler=psc;
		TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);

		TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); 
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;      
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);    

		TIM_Cmd(TIM4,ENABLE);     
 }




void TIM4_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM4,TIM_IT_Update))
		{
			now_mai++;
			if(now_mai>mai_con)
			{
				now_mai=0;
				MAI=!MAI;
			}
		}
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);    
}

 
