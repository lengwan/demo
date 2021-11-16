#include "sys.h"
#include "delay.h"
#include "demo131a04.h" 
#include "ADS131A04_EVB.h"
#include "usart.h"
#include "led.h"
union float_temp
{
	float f_data;
	u8 u_temp[4]; 
};
union float_temp send_buf;
union float_temp send_buf;
void Send_char(char temp);
extern unsigned long Value[4];
void ADS131swap(int *a, int *b)  
{  
    int     c;  
     c = *a;  
    *a = *b;  
    *b =  c;  
} 
int ADS131GetAverage(int *dat,u16 leng)//中值滤波
{
	int Average=0;
	u16 i,j;
    //排序，从dat[0]开始排序，从小到大
    for (i = 0; i < leng; i++)  
    {  
        for (j = i + 1; j < leng; j++)  
        {  
            if (dat[i] > dat[j])  
            {  
                ADS131swap(&dat[i], &dat[j]);  
            }
        }
    }
		
		Average=dat[leng/2];

		return Average;
}


/****************************************
//功能:把读数转化成电压值,输入分别为 ： 读回的二进制值   参考电压   内置增益
*****************************************/
double DataFormatting(u32 Data , double Vref ,u8 PGA)
{
	/*
	电压计算公式；
			设：AD采样的电压为Vin ,AD采样二进制值为X，参考电压为 Vr ,内部集成运放增益为G
			Vin = ( (Vr) / G ) * ( x / (2^23 -1))
	*/
	double ReadVoltage;
	if(Data & 0x00800000)
	{
		Data = (~Data) & 0x00FFFFFF;
		ReadVoltage = -(((double)Data) / 8388607) * ((Vref) / ((double)PGA));
	}
	else
		ReadVoltage =  (((double)Data) / 8388607) * ((Vref) / ((double)PGA));
	
	return(ReadVoltage);
}


int demo_ads131a0x(void)
{ 
	static unsigned long iTemp;
	u8 shuju[4]={0x03,0xFC,0xFC,0x03};
	float value;
	float buchang[4]={0.00004,-0.0006,-0.001,-0.00075};//数据纠偏
  char buf[16]=" ";
	char i=0,j=0;	
	/* Initialize MCP3911 device. */
	ADS13_PowerOnInit();
	delay_ms(100);//初始化延时
  while(1) 
	{

		Read_ADS131A0X_Value();
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR =shuju[0];
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR =shuju[1]; 		
	  for(i=0;i<4;i++)
		{
			value = DataFormatting(Value[i] , 4.97 ,1)+buchang[i];
			send_buf.f_data=value;
			for(j=0;j<4;j++)
			Send_char(send_buf.u_temp[j]);
		
		}
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR =shuju[2];
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR =shuju[3]; 
	
	}
	
}


void Send_char(char temp)
{
		while((USART1->SR&0X40)==0);//????,??????   
    USART1->DR = (u8)temp;  
}