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
	send_comand_DFPlayer(9, 0, 4);

	Delay_ms(100);
	volume(20);
	play();

	while(1)
	{

	}
}

