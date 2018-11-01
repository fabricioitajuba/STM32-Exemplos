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
#include "utils.h"
#include "ds18b20.h"
#include "delays.h"
#include "lcd.h"

simple_float temperature;
int temp;

int main(void)
{
	SysTick_Config(SystemCoreClock / 100000);
	setup_delay_timer (TIM2);

	GPIO_InitTypeDef gpioInitStruct;
	GPIO_StructInit(&gpioInitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioInitStruct.GPIO_Pin = GPIO_Pin_13;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &gpioInitStruct);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	gpioInitStruct.GPIO_Pin = GPIO_Pin_11;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &gpioInitStruct);

	Delay_ms(100);
	lcd_init();
	lcd_clear();
	lcd_puts("Temperatura:");

	// Configure DS18B20 and 1-Wire on pin B12
	// Please remember about adding pull-up resistor :)
	ds18b20_init(GPIOB, GPIO_Pin_12, TIM2);

	while(1)
	{
		temperature = ds18b20_get_temperature_simple();
		if (temperature.is_valid)
		{
			//sprintf(buff, "%d\t%s\t%d.%03d\n", entry, timestamp, temperature.integer, temperature.fractional);
			//msb_temp = temperature.integer;
			//lsb_temp = temperature.fractional;
			temp = temperature.integer;
			lcd_gotoxy(0, 1);
			//LCD_PutIntByte(temp);
			LCD_PutHexByte(temp);

			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
			Delay_ms(50);
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			Delay_ms(1000);
		}
	}
}
