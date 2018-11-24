/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
  USAR1 = TX1-PA9, RX1-PA10
*/

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "delays.h"
#include "DFPlayer.h"

int main(void)
{
	SetupUSART();
	Delay_ini();

	//inicializa o DFPlayer
	ini_DFPlayer(INI_FLASH);
	specify_playback_source(FLASH);
	specify_eq(ROCK);
	Delay_ms(3000);

	volume(20);

	normal_working(); //Executa uma por uma
	//previous();
	//previous();

	while(1)
	{

	}
}

