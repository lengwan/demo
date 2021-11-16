#ifndef __SPI_H
#define __SPI_H
#include "sys.h"


/* 定义GPIO端口 */


/*
SCK     ->   PA5
DIN     ->   PA7(MOSI)
CS      ->   PA4
DOUT    ->   PA6(MISO)
RESET   ->   PA3
DRDY    ->   PA1

*/
	
	
#define RCC_SCK 	RCC_APB2Periph_GPIOA
#define PORT_SCK	GPIOA
#define PIN_SCK		GPIO_Pin_5

#define RCC_DIN 	RCC_APB2Periph_GPIOA
#define PORT_DIN	GPIOA
#define PIN_DIN		GPIO_Pin_7

#define RCC_CS 		RCC_APB2Periph_GPIOA
#define PORT_CS		GPIOA
#define PIN_CS		GPIO_Pin_4


#define RCC_DOUT 	RCC_APB2Periph_GPIOA
#define PORT_DOUT	GPIOA
#define PIN_DOUT	GPIO_Pin_6


#define RCC_RESET 	RCC_APB2Periph_GPIOA
#define PORT_RESET	GPIOA
#define PIN_RESET		GPIO_Pin_3

#define RCC_DR   	RCC_APB2Periph_GPIOA
#define PORT_DR	  GPIOA
#define PIN_DR		GPIO_Pin_1



#define CS_0()		GPIO_ResetBits(PORT_CS, PIN_CS)
#define CS_1()		GPIO_SetBits(PORT_CS, PIN_CS)

#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

#define DI_0()		GPIO_ResetBits(PORT_DIN, PIN_DIN)
#define DI_1()		GPIO_SetBits(PORT_DIN, PIN_DIN)

#define DO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_DOUT, PIN_DOUT) == Bit_SET)

#define RESET_0		GPIO_ResetBits(PORT_RESET, PIN_RESETC)
#define RESET_1 	GPIO_SetBits(PORT_RESET, PIN_RESET)
	
#define DR_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_DR, PIN_DR) == Bit_SET)
	
	
// void SPI2_Init(void);			 //初始化SPI1口
// void SPI2_SetSpeed(u8 SpeedSet); //设置SPI1速度   
// u8 SPI2_ReadWriteByte(u8 TxData);

void ADS131A0X_soft_spi(void);
uint8_t ADS131A0X_SendRecive_8Bit(uint8_t _data);

#endif

