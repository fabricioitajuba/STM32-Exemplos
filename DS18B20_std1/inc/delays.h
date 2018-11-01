/*
 * delays.h
 *
 *  Created on: 27 de out de 2018
 *      Author: Eng. Fabrício de Lima Ribeiro
 */

#ifndef DELAYS_H_
#define DELAYS_H_

void SysTick_Handler(void);
void Delay_us(const int usec);
void Delay_ms(const int msec);

unsigned int TimingDelay, ctMs;
unsigned int usCounter, msCounter;

#endif /* DELAYS_H_ */
