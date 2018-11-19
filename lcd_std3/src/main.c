/******************************************************************************
STM32 - Exemplo de programação StdPeriph utilizando display LCD 16x2 e o led
da placa Blue Pill
STM32F103C8T6
Autor: Eng. Fabrício de Lima Ribeiro
26/10/2018
******************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "delays.h"
#include "lcd.h"

//Definições para configurar o led de saída
#define LED_PIN  GPIO_Pin_13
#define LED_PORT GPIOC

int cont=0, i;
char msg[]="teste", buffer[13];

int main(void)
{
	Delay_ini();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);// Clock PORTB Enable

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = LED_PIN; // Configure Led pin
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// Set Output Push-Pull
	GPIO_Init(LED_PORT, &GPIO_InitStruct);

	GPIO_SetBits(LED_PORT, LED_PIN);

	Delay_ms(100);
	lcd_init();
	lcd_clear();
	lcd_gotoxy(0, 0);
	lcd_puts("Contagem:       ");

	while(1)
	{
		lcd_gotoxy(0, 1);
		//lcd_puts(msg);

		sprintf(buffer, "%3d", cont);
		lcd_puts(buffer);

		cont ++;
		if(cont > 999)
		{
			cont = 0;
		}

		GPIO_ResetBits(LED_PORT, LED_PIN);
		Delay_ms(300);
		GPIO_SetBits(LED_PORT, LED_PIN);
		Delay_ms(700);
	}
}
