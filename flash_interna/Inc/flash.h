/*! @file   flash.h
Biblioteca para gravar e ler a flash
*/

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f1xx_hal.h"
#include <string.h>

/*
 * STM32F103C8
 * FLASH INTERNA -> 0x08000000 - 0x0800FFFF
 * Valor escolhido aleatoriamente: 0x08010000
 */
#define FLASH_STORAGE 0x08010000
#define page_size 0x800

void save_to_flash(uint8_t *data);
void read_flash(uint8_t* data);

#endif /* FLASH_H_ */
