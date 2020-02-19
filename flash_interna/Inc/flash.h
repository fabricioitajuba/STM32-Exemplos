/*
 * flash.h
 *
 *  Created on: Feb 19, 2020
 *      Author: fabricio
 */

#ifndef FLASH_H_
#define FLASH_H_

// Include the appropriate HAL Library here
#include "stm32f1xx_hal.h"
#include <string.h>

#define FLASH_STORAGE 0x08000000
#define page_size 0x800

void save_to_flash(uint8_t *data);
void read_flash(uint8_t* data);

#endif /* FLASH_H_ */
