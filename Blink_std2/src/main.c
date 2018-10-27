/******************************************************************************
STM32 - Exemplo de programação StdPeriph utilizando o ledcda placa Blue Pill
STM32F103C8T6
Autor: Eng. Fabrício de Lima Ribeiro
26/10/2018
******************************************************************************/


#include "stm32f10x.h"
#include "delays.h"
			
#define LED_PIN    GPIO_Pin_13
#define LED_PORT   GPIOC

GPIO_InitTypeDef GPIO_InitStruct;

int main(void)
{
	 SysTick_Config(SystemCoreClock / 100000);

	 // Configure the GPIO pin for the LED (PC13)
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // Clock PORTC Enable

	 GPIO_InitStruct.GPIO_Pin = LED_PIN;            // Configure Led pin
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;   // Set Output Push-Pull
	 GPIO_Init(LED_PORT, &GPIO_InitStruct);

	 GPIO_SetBits(LED_PORT, LED_PIN); // LED1 OFF

	while(1)
	{
		GPIO_ResetBits(LED_PORT, LED_PIN); // LED1 ON
		Delay_ms(50);
		GPIO_SetBits(LED_PORT, LED_PIN); // LED1 OFF
		Delay_ms(50);
		GPIO_ResetBits(LED_PORT, LED_PIN); // LED1 ON
		Delay_ms(50);
		GPIO_SetBits(LED_PORT, LED_PIN); // LED1 OFF
		Delay_ms(50);
		GPIO_ResetBits(LED_PORT, LED_PIN); // LED1 ON
		Delay_ms(50);
		GPIO_SetBits(LED_PORT, LED_PIN); // LED1 OFF
		Delay_ms(1000);
	}
}
