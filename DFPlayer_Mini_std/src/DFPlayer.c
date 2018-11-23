/*
 * DFPlayer.c
 *
 *  Created on: 22 de nov de 2018
 *      Author: Fabricio
 */
#include "stm32f10x.h"
#include"DFPlayer.h"

// ***********************************************************************
//  SetupUSART()
//
// USART1 configured as follow:
// BaudRate = 9600 baud, Word Length = 8 Bits, One Stop Bit, No parity
// Hardware flow control disabled (RTS and CTS signals)
// Receive and transmit enabled
// USART Clock disabled
// USART CPOL: Clock is active low
// USART CPHA: Data is captured on the middle
// USART LastBit: The clock pulse of the last data bit is not output to
//                           the SCLK pin
// ***********************************************************************
void SetupUSART()
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 USART_InitTypeDef USART_InitStructure;

 // Enable USART1 and GPIOA clock
 RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

 // Configure USART1 Rx (PA10) as input floating
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 // Configure USART1 Tx (PA9) as alternate function push-pull            */
 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 USART_InitStructure.USART_BaudRate            = 9600;
 USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
 USART_InitStructure.USART_StopBits            = USART_StopBits_1;
 USART_InitStructure.USART_Parity              = USART_Parity_No ;
 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
 USART_Init(USART1, &USART_InitStructure);
 USART_Cmd(USART1, ENABLE);
}

// ***********************************************************************
// ******** USART Utility functions **************************************
// ***********************************************************************
// ***********************************************************************
// USART_PutChar(char *c)
// ***********************************************************************
void USART_PutChar(char c)
{
  // write a character to the USART
  USART_SendData(USART1, (uint8_t) c);

  ///Loop until the end of transmission
  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

// ***********************************************************************
// USART_PutStr(char *str)
// ***********************************************************************
void USART_PutStr(char *str)
{
	int i;
	for(i=0; i<10; i++)
	{
		USART_PutChar(*str);
		str++;
	}
}

// ***********************************************************************
// USART_PutHexByte(char byte)
// ***********************************************************************
void USART_PutHexByte(unsigned char byte)
{
  char n = (byte >> 4) & 0x0F;
  // Write high order digit
  if(n < 10)
 	USART_PutChar(n + '0');
  else
	USART_PutChar(n - 10 + 'A');

  // Write low order digit
  n = (byte & 0x0F);
  if(n < 10)
 	USART_PutChar(n + '0');
  else
	USART_PutChar(n - 10 + 'A');
}

// ***********************************************************************
// USART_PutStrHex(char *str)
// ***********************************************************************
void USART_PutStrHex(char *str)
{
	int i;
	for(i=0; i<10; i++)
	{
		USART_PutHexByte(*str);
		str++;
	}
	USART_PutChar(0x0D);
	USART_PutChar(0x0A);
}


// ***********************************************************************
// Envia comandos para o DFPlayer
// ***********************************************************************
void send_comand_DFPlayer(uint8_t cmd, uint8_t para1, uint8_t para2)
{
	int i;
	uint8_t buffer[10] = {0x7E, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF};
	uint16_t sum = 0;

	//Comandos
	buffer[3] = cmd;
	buffer[5] = para1;
	buffer[6] = para2;
	//Checksum
	for(i=1; i<7; i++)
	{
		sum += buffer[i];
	}
	sum = - sum;

	buffer[7] = (sum >> 8) & 0x00FF;
	buffer[8] = (sum & 0x00FF);

	//USART_PutStrHex(buffer);
	USART_PutStr(buffer);

}

// ***********************************************************************
// Envia comando play
// ***********************************************************************
void play(void)
{
	send_comand_DFPlayer(3, 0, 1);
}

// ***********************************************************************
// Envia comando pause
// ***********************************************************************
void pause(void)
{
	send_comand_DFPlayer(0x0E, 0, 0);
}

// ***********************************************************************
// Envia comando pause vol 0-30
// ***********************************************************************
void volume(uint8_t vol)
{
	send_comand_DFPlayer(6, 0, vol);
}

// ***********************************************************************
// Envia comando loop
// ***********************************************************************
void Loop()
{
	send_comand_DFPlayer(0x11, 0, 0x01);
}
