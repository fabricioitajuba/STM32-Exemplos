/**
  ******************************************************************************
  * @file    main.c
  * @author  Eng.Fabrício de Lima Ribeiro
  * @version V1.0
  * @date    25-Novembro-2018
  * @brief   Programa teste do DFPlayer - blue pill
  ******************************************************************************
  USAR1 = TX1-PA9, RX1-PA10 -> DFPlayer
  Display LCD - ver as pinagens no arquivo lcd.h
*/

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "delays.h"
#include "lcd.h"
#include "DFPlayer.h"

mostra_query(uint16_t i);

int main(void)
{
	SetupUSART();
	Delay_ini();
	lcd_init();
	lcd_clear();

	//inicializa o DFPlayer
	ini_DFPlayer(INI_FLASH);
	specify_playback_source(_FLASH);
	Delay_ms(3000);

	volume(0x14);

	//normal_working(); //Executa uma por uma
	specify_tracking(0x0001);
	specify_eq(ROCK);

	while(1)
	{
		lcd_clear();
		lcd_puts("Current VOL: ");
		lcd_gotoxy(0, 1);
		mostra_query(query_system(0x43));
		Delay_ms(3000);

		//Current Equalização
		lcd_clear();
		lcd_puts("Current EQ: ");
		lcd_gotoxy(0, 1);
		mostra_query(query_system(0x44));
		Delay_ms(3000);

		//Current playback mode
		lcd_clear();
		lcd_puts("Current mode:");
		lcd_gotoxy(0, 1);
		mostra_query(query_system(0x45));
		Delay_ms(3000);

		//Current number
		lcd_clear();
		lcd_puts("Number files:");
		lcd_gotoxy(0, 1);
		mostra_query(query_system(0x48));
		Delay_ms(3000);

		//Current track
		lcd_clear();
		lcd_puts("Current track:");
		lcd_gotoxy(0, 1);
		mostra_query(query_system(0x4C));
		Delay_ms(3000);

		next();
	}
}

mostra_query(uint16_t i)
{
	LCD_PutHexByte((i >> 8) & 0x00FF);
	LCD_PutHexByte(i & 0x00FF);
}
