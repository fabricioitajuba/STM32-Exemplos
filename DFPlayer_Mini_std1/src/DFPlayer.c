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
//	USART_GetStr(void)
//  Return length of input string
// ***********************************************************************
void USART_GetStr(void)
{
	int i=0;
	char k = 0;

	while(k != 0xEF)
	{
		// Is a byte available from UART?
		if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
		{
			k = USART_ReceiveData(USART1);  // get input char
			msgBuf[i]=k;                       // store input char

			if(++i == 10)  // Buffer Full = EXIT
				break;
		}
    }

	msgBuf[i]=0;
}

// ***********************************************************************
// Envia comandos para o DFPlayer
// ***********************************************************************
void send_comand_DFPlayer(uint8_t cmd, uint8_t feedback, uint8_t para1, uint8_t para2)
{
	int i;
	uint8_t buffer[10] = {0x7E, 0xFF, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEF};
	uint16_t sum = 0;

	//Comandos
	buffer[3] = cmd;
	buffer[4] = feedback;
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

	USART_PutStr(buffer);

}

// ***********************************************************************
// Retorno de parâmetros do módulo
// ***********************************************************************
uint16_t query_system(uint8_t query)
{
	uint16_t dados = 0;

	send_comand_DFPlayer(query, 0, 0, 0);
	USART_GetStr();

	dados = msgBuf[5] << 8;
	dados += msgBuf[6];

	return dados;
}

// ***********************************************************************
// Inicializa o DFPlayer
// ***********************************************************************
void ini_DFPlayer(uint8_t ini)
{
	send_comand_DFPlayer(0x3f, 0x00, 0x00, ini);
}

// ***********************************************************************
// Envia comando next
// ***********************************************************************
void next(void)
{
	send_comand_DFPlayer(0x01, 0x00, 0x00, 0x00);
}

// ***********************************************************************
// Envia comando previous
// ***********************************************************************
void previous(void)
{
	send_comand_DFPlayer(0x02, 0x00, 0x00, 0x00);
}

// ***********************************************************************
// Especifica qual música
// ***********************************************************************
void specify_tracking(uint16_t num)
{
	uint8_t num_H, num_L;

	num_H = (num >> 8) & 0x00FF;
	num_L = (num & 0x00FF);

	send_comand_DFPlayer(0x03, 0x00, num_H, num_L);
}

// ***********************************************************************
// Aumenta o volume
// ***********************************************************************
void increase_volume(void)
{
	send_comand_DFPlayer(0x04, 0x00, 0x00, 0x00);
}

// ***********************************************************************
// Decrementa o volume
// ***********************************************************************
void decrease_volume(void)
{
	send_comand_DFPlayer(0x05, 0x00, 0x00, 0x00);
}

// ***********************************************************************
// Envia comando pause vol 0-30
// ***********************************************************************
void volume(uint8_t vol)
{
	send_comand_DFPlayer(0x06, 0x00, 0x00, vol);
}

// ***********************************************************************
// Especifica o tipo de equalização
// ***********************************************************************
void specify_eq(uint8_t eq)
{
	send_comand_DFPlayer(0x07, 0x00, 0x00, eq);
}

// ***********************************************************************
// Especifica o modo de playback
// ***********************************************************************
void specify_playback_mode(uint8_t mode)
{
	send_comand_DFPlayer(0x08, 0x00, 0x00, mode);
}

// ***********************************************************************
// Especifica a fonte
// ***********************************************************************
void specify_playback_source(uint8_t source)
{
	send_comand_DFPlayer(0x09, 0x00, 0x00, source);
}

// ***********************************************************************
// Envia comando pause
// ***********************************************************************
void pause(void)
{
	send_comand_DFPlayer(0x0E, 0x00, 0x00, 0x00);
}

// ***********************************************************************
// Envia comando loop
// ***********************************************************************
void normal_working(void)
{
	send_comand_DFPlayer(0x11, 0x00, 0x00, 0x01);
}

// ***********************************************************************
// Envia comando play
// ***********************************************************************
void playback(void)
{
	send_comand_DFPlayer(0x0D, 0x00, 0x00, 0x00);
}
