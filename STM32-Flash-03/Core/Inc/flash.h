/******************************************************************************
 * Biblioteca para gravar e ler a flash
 * STM32F103C6xx FLASH INTERNA -> 0x0800.0000 - 0x0800.7FFF - 32k páginas de 1k
 * STM32F103C8xx FLASH INTERNA -> 0x0800.0000 - 0x0800.FFFF - 64k páginas de 1k
 ******************************************************************************/

#ifndef FLASH_H_
#define FLASH_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include <string.h>

#ifndef FLASH_STORAGE
#define FLASH_STORAGE 0x08007C00
#endif

#ifndef page_size
#define page_size 0x400
#endif

void save_to_flash(uint8_t *data);
void read_flash(uint8_t* data);
void SaveStructToFlash(MyData_t *data);
void LoadStructFromFlash(MyData_t *data);

#endif /* FLASH_H_ */
