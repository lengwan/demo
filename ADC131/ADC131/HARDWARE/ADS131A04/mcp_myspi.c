#include "mcp_myspi.h"
#include "delay.h"
#include "sys.h"
#include "stm32f10x_spi.h"



void ADS131A0X_WaitDRDY(void);
void ADS131A0X_DelaySCLK(void);
void ADS131A0X_DelayDATA(void);

void ADS131A0X_soft_spi(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // ʹ��PA�˿�ʱ�� 

	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������ģʽ */

	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN;
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RESET;
	GPIO_Init(PORT_RESET, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* PIN_DOUT ����Ϊ�������� */
	GPIO_InitStructure.GPIO_Pin = PIN_DOUT;
	GPIO_Init(PORT_DOUT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_DR;
	GPIO_Init(PORT_DR, &GPIO_InitStructure);

	CS_1();
	SCK_0();		/* SPI���߿���ʱ�������ǵ͵�ƽ */
	RESET_1;
}


void ADS131A0X_DelaySCLK(void)
{
	uint16_t i;
	for (i = 0; i < 3; i++);
}


void ADS131A0X_DelayDATA(void)
{
	delay_us(6);	/* ��?D??��3�� 6.5uS, ��?��|��?10us */
}


uint8_t ADS131A0X_SendRecive_8Bit(uint8_t _data)
{
	
	uint8_t i;
	uint8_t read = 0;
	ADS131A0X_DelayDATA();
	for (i = 0; i < 8; i++)
	{
		SCK_1();
		if (_data & 0x80)
		{
			DI_1();
		}
		else
		{
			DI_0();
		}
		_data=_data<< 1;
		read = read<<1;
		ADS131A0X_DelaySCLK();
		SCK_0();
		read= read | GPIO_ReadInputDataBit(PORT_DOUT, PIN_DOUT);
		ADS131A0X_DelaySCLK();
	}
	return read;
	
}
