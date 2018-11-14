/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "delays.h"

#define LEDS GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3

#define LED_5  GPIO_Pin_4
#define LED_6  GPIO_Pin_5
#define LED_7  GPIO_Pin_6
#define LED_8  GPIO_Pin_7

#define LED_PORT GPIOC

int main(void)
{

	 uint16_t dataVal;
	 GPIO_InitTypeDef GPIO_InitStruct;

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);// Clock PORTA Enable
	 GPIO_InitStruct.GPIO_Pin = LEDS; // Configure LCD Data pins
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// Set Output Push-Pull
	 GPIO_Init(LED_PORT, &GPIO_InitStruct);

	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);// Clock PORTA Enable
	 GPIO_InitStruct.GPIO_Pin = LED_5 | LED_6 | LED_7 | LED_8; // Configure LCD Ctrl pins
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// Set Output Push-Pull
	 GPIO_Init(LED_PORT, &GPIO_InitStruct);

	 GPIO_ResetBits(LED_PORT, LED_5 | LED_6 | LED_7 | LED_8);

	 Delay_ini();

	while(1)
	{
		 dataVal = GPIO_ReadOutputData(LED_PORT);
		 dataVal |= 0x000F;
		 GPIO_Write(LED_PORT, dataVal);
		 Delay_ms(2000);

		 dataVal = GPIO_ReadOutputData(LED_PORT);
		 dataVal &= 0x0000;
		 GPIO_Write(LED_PORT, dataVal);
		 Delay_ms(2000);
	}
}
