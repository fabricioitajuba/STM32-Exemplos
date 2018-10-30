/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  * fontes: https://www.embarcados.com.br/system-workbench-com-stm32f429i-disco/
  *         http://www.l3oc.com/2015/05/stm-arm-cortex-m3-cmsis-delay.html
  ******************************************************************************
*/

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

//Definições para configurar o led de saída
#define LED_PIN  GPIO_Pin_13
#define LED_PORT GPIOC
			
//Variavel uint32 para contar milissegundos.
//volatile determina que pode ser acessada e modificada
//em diversas partes do codigo.
volatile uint32_t msTicks;

//A cada ocorrencia de SysTick, o sistema faz uma chama a essa rotina
void SysTick_Handler (void)
{
  //E nessa rotina, incrementamos +1 a variavel msTicks.
  msTicks++;
}

//Funcao de delay - recebe atraso em ms
void Delay(uint32_t dlyTicks)
{
  //Variavel uint32 para ponto de partida de contagem
  uint32_t curTicks;
  //Ponto de partida recebe msTicks original neste ponto
  curTicks = msTicks;
  //Segura a execucao do codigo enquanto a diferenca
  //entre msTicks (modificado pelo sistema) e curTicks (do ponto de chamada)
  //for menor que o delay passado por parametro pra funcao.
  while ((msTicks - curTicks) < dlyTicks);
}

int main(void)
{
	//Configura a interrupcao de SysTick a cada milissegundo
	SysTick_Config(SystemCoreClock / 1000);
	//Habilita clock no GPIOC - Leds
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);// Clock PORTB Enable

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = LED_PIN; // Configure Led pin
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;// Set Output Push-Pull
	GPIO_Init(LED_PORT, &GPIO_InitStruct);

    while(1)
    {
        // Essa rotina faz com que os sinais sejam invertidos.
        // Ou seja, se forem 0, viram 1. Se forem 1, viram 0.
        // De modo a causar o efeito de alternancia do pisca LED
        // a cada 1 segundo do delay
        GPIO_ResetBits(LED_PORT, LED_PIN);
        Delay(50); // Dispara o delay de 50 ms.
        GPIO_SetBits(LED_PORT, LED_PIN);
        Delay(50); // Dispara o delay de 50 ms.
        GPIO_ResetBits(LED_PORT, LED_PIN);
        Delay(50); // Dispara o delay de 50 ms.
        GPIO_SetBits(LED_PORT, LED_PIN);
        Delay(50); // Dispara o delay de 50 ms.
        GPIO_ResetBits(LED_PORT, LED_PIN);
        Delay(50); // Dispara o delay de 50 ms.
        GPIO_SetBits(LED_PORT, LED_PIN);
        Delay(2000); // Dispara o delay de 1 segundo = 1000 ms.
    }
}
