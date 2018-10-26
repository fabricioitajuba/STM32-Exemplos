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
#include "lcd.h"

//Definições para configurar o led de saída
#define LED_PIN  GPIO_Pin_13
#define LED_PORT GPIOC

//Declaração das funções utilizadas no programa
void SysTick_Handler(void);
void Delay_us(const uint32_t usec);
void Delay_ms(const uint32_t msec);

static __IO uint32_t TimingDelay, ctMs;
static __IO uint32_t usCounter, msCounter;

GPIO_InitTypeDef GPIO_InitStruct;

int led1_val=0;  // state of LED

int main(void)
{

	SysTick_Config(SystemCoreClock / 100000);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);// Clock PORTB Enable
	GPIO_InitStruct.GPIO_Pin = LED_PIN; // Configure Led pin
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// Set Output Push-Pull
	GPIO_Init(LED_PORT, &GPIO_InitStruct);

	lcd_init();
	lcd_clear();
	lcd_goto(0);
	lcd_puts("lcd_teste:");

	//GPIO_ResetBits(LED_PORT, LED_PIN);  //LED ON
	//GPIO_SetBits(LED_PORT, LED_PIN);  //LED OFF

	TimingDelay = 100000;

	while(1)
	{
		lcd_goto(15);

		if(led1_val == 0)
		{
			lcd_putch('0');
			GPIO_SetBits(LED_PORT, LED_PIN);  //LED OFF
			led1_val = 1;
		}
		else if(led1_val == 1)
		{
			lcd_putch('1');
			GPIO_ResetBits(LED_PORT, LED_PIN);  //LED ON
			led1_val = 0;
		}

		Delay_ms(1000);
	}
}

// ***********************************************************************
//  Simple microsecond delay routine (* 10us resolution)
// ***********************************************************************
void Delay_us(const uint32_t usec)
{
	TimingDelay = usec/10;
	while(TimingDelay != 0);
}

// ***********************************************************************
//  Simple millisecond delay routine
// ***********************************************************************
void Delay_ms(const uint32_t msec)
{
	Delay_us(msec * 1000);
}

// ***********************************************************************
//  SystemTick IRQ handler (invoked every 10us)
//  Decrements TimingDelay, increments usCounter & msCounter;
// ***********************************************************************
void SysTick_Handler(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}

	usCounter+=10;

	if(++ctMs > 99)
	{
		msCounter++;
		ctMs=0;
	}
}
