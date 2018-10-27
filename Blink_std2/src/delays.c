/*
 * delays.c
 *
 *  Created on: 27 de out de 2018
 *      Author: Eng. Fabrício de Lima Ribeiro
 */
#include "delays.h"

// ************************************
//  Rotina de delay em micro-segundos
// ************************************
void Delay_us(const int usec)
{
  TimingDelay = usec/10;
  while(TimingDelay != 0);
}

// ************************************
//  Rotina de delay em mili-segundos
// ************************************
void Delay_ms(const int msec)
{
  Delay_us(msec * 1000);
}

// ***********************************************************************
//  SystemTick IRQ handler
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
