#include "ADS131A04_EVB.h"
#include "delay.h"
#include "stm32f10x.h"
#include "stdio.h"  
unsigned long Value[4];
  
/**��ʼ��ADS131**/  
void ADS131A0X_Init(void)   
{  
    ADS131A0X_soft_spi();
}  


/***************************************************************************************** 
* Function Name: ADS13_SPI 
* Description  : ͨ��SPI������ADS131ͨ�� 
* Arguments    : com:��д������� 
* Return Value : ���ض�ȡ������ 
******************************************************************************************/  
unsigned char ADS13_SPI(unsigned char com)  
{
   return(ADS131A0X_SendRecive_8Bit(com));  
}  


/* write a command to ADS131a02 */  
u16 ADS13_WRITE_CMD(u16 command)  
{  
    u16 receive=0x0000;  
    CS_0();  
		delay_us(10);  
    receive = ADS13_SPI((u8)(command>>8)); 
    receive <<= 8;  
    receive |= ADS13_SPI((u8)(command)); 
    ADS13_SPI(0X00);//����3���ֽ�   24bit
    ADS13_SPI(0X00);//����4���ֽ�   32bit
    CS_1();   
		delay_us(10);  
    return receive; //���ؽ��յ�����  
}

/***************************************************************************************** 
* Function Name: ADS13_REG 
* Description  : ��ADS131�ڲ��Ĵ������в���  
* Arguments    : com:��д�Ĵ�����ַ��data����д������� 
* Return Value : ���ض�ȡ������ 
******************************************************************************************/  
u16 ADS13_REG(unsigned char com, unsigned data)  
{  
    u16 temp=0X0000;   
    if((com&0x20)==0x20)//�ж��Ƿ�Ϊ���Ĵ���ָ�� if����Ϊ��  
    {
        ADS13_WRITE_CMD(com);  
			  temp= ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
    }  
    else //д�Ĵ���  
    {
        CS_0();
				delay_us(10);  
        ADS13_SPI(com);  
        ADS13_SPI(data);  
        ADS13_SPI(0X00); // ����0  24bit
        ADS13_SPI(0X00); // ����0  32bit
        delay_us(10);   
        CS_1();  
    }  
    delay_us(10);   
    return temp;  
}    
  
/***************************************************************************************** 
* Function Name: ADS13_PowerOnInit 
* Description  : ADS13�ϵ縴λ 
* Arguments    : NONE 
* Return Value : NONE 
******************************************************************************************/  
int adcenable_flag=0;  
void ADS13_PowerOnInit(void)  
{
		u8 RREG=0X20;//��
		u8 WREG=0X40;//д
    u16 RECEIVE=0X0000; //����spi���ص��ַ�  
		u16 ID_M,ID_L;
    ADS131A0X_Init(); 	
    delay_ms(20); 

    do  
    {  
				ADS13_WRITE_CMD(ADS131A04_RESET_COMMAND);  //RESET ADS131
        RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND);  
        printf("receive data is : %X...\n", RECEIVE);  
        if(RECEIVE == ADS131A04_READY) //ADS131A02 is ready.break loop  
            printf("ADS131A02 is ready!\n  Configuring Registers...\n");  
        delay_ms(5);  
    }while(RECEIVE != ADS131A04_READY); //��ʼ���ɹ�������ѭ��
		
		ADS13_WRITE_CMD(ADS131A04_UNCLOCK_COMMAND);//�����Ĵ���
		RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND);  
		
		ADS13_WRITE_CMD(0x2000);//read id
		RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
		
		ADS13_WRITE_CMD(0x2100);//read id
		RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
		
//    ADS13_REG(WREG|A_SYS_CFG, 0X78); //enable internal reference volatage (4.0v)  
		ADS13_REG(WREG|A_SYS_CFG, 0X30);
		ADS13_REG(WREG|D_SYS_CFG, 0X3E); //Fixed six device words per frame for the ADS131A04 
    ADS13_REG(WREG|CLK1, 0X08); //ADC CLK1  fICLK = fCLKIN(16.384mhz) / 8 

		
    ADS13_REG(WREG|CLK2, 0X20); //����Ƶ��
		
//		ADS13_REG(WREG|CLK2, 0XE0);//37HZ
		
//				ADS13_REG(WREG|CLK2, 0X40);//125HZ
		
    ADS13_REG(WREG|ADC_ENA, 0X0F); //ADC CHANNEL ENABLE ALL  
		
		ADS13_WRITE_CMD(0x2B00);//read A_SYS_CFG
		RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
		
		ADS13_WRITE_CMD(0x2C00);//read D_SYS_CFG
		RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
		
    ADS13_WRITE_CMD(ADS131A04_WAKEUP_COMMAND);//WAKEUP ADS131  
		RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND);  
      
    adcenable_flag=1; //��ʼ����ϱ�־λ  
}  
  
u8 ADC_READY(void)
{
	u16 ret=0;
	u16 RECEIVE;
	ADS13_WRITE_CMD(0x2200);//read D_SYS_CFG
	RECEIVE = ADS13_WRITE_CMD(ADS131A04_NULL_COMMAND); 
	RECEIVE = RECEIVE & 0x0002;
	ret = (u8) RECEIVE;
	return ret;
	
}

u8 dummy_send[24]={0};
u8 recBuffer[24];
void Read_ADS131A0X_BUFFER_DATA(void)
{
	u8 i=0;
	CS_0(); 
	for(i = 0; i < 24; i++)
	{
		recBuffer[i]=ADS13_SPI(dummy_send[i]);
	}
	CS_1();
}


				
void Read_ADS131A0X_Value(void)//��ȡADC
{
	unsigned long  value =0;
	union
	{
		unsigned long voltage;
		struct
		{
			unsigned char LSB ;
			unsigned char NSB ;
			unsigned char MSB ;
			unsigned char d;
		}byte;
	} volt0,volt1,volt2,volt3;
	
	
	while(ADC_READY());//�ȴ�ת�����
	
	// ��ȡ�������� 24bit
	Read_ADS131A0X_BUFFER_DATA();
	
	
	
	volt0.byte.MSB=recBuffer[4];
	volt0.byte.NSB=recBuffer[5];
	volt0.byte.LSB=recBuffer[6];
	volt0.byte.d=recBuffer[7];
	
	volt1.byte.MSB=recBuffer[8];
	volt1.byte.NSB=recBuffer[9];
	volt1.byte.LSB=recBuffer[10];
	volt1.byte.d=recBuffer[11];
	
	volt2.byte.MSB=recBuffer[12];
	volt2.byte.NSB=recBuffer[13];
	volt2.byte.LSB=recBuffer[14];
	volt2.byte.d=recBuffer[15];
	
	volt3.byte.MSB=recBuffer[16];
	volt3.byte.NSB=recBuffer[17];
	volt3.byte.LSB=recBuffer[18];
	volt3.byte.d=recBuffer[19];
	Value[0]=volt0.voltage;
	Value[1]=volt1.voltage;
	Value[2]=volt2.voltage;
	Value[3]=volt3.voltage;
//	switch(channel)
//	{
//		case 0:value = volt0.voltage;break;
//		case 1:value = volt1.voltage;break;
//		case 2:value = volt2.voltage;break;		
//		case 3:value = volt3.voltage;break;		
//	}

//	return value;

}

  
