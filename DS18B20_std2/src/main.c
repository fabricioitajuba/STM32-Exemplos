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
#include "ds18b20.h"
#include "lcd.h"

simple_float temperature;
int temp;
char buffer[10];

int main(void)
{
	Delay_ini();

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
			sprintf(buffer, "%d.%d [*C]", temperature.integer, temperature.fractional);
			lcd_gotoxy(0, 1);
			lcd_puts(buffer);
		}
		Delay_ms(300);
	}
}

