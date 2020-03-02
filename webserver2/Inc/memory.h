/*
 * memory.h
 *
 *  Created on: 20 de fev de 2020
 *      Author: fabricio
 *      Fonte: http://www.pstadler.de/2018/04/04/storing-data-on-the-non-volatile-flash-of-a-stm32f303/
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#ifndef _MEMORY_H
#define _MEMORY_H

#include "main.h"
//#include <inttypes.h>

#define FLASH_UNLOCK_KEY1	0x45670123
#define FLASH_UNLOCK_KEY2	0xCDEF89AB
#define FLASH_TIMEOUT		1000

#define FLASH_MEMORYMAP_BASE_ADDR	0x08010000		/*is also the beginning of the used page xx*/

/*Important: Do NOT change the data types of the map or add one that is different!*/
typedef struct
{
	__IO	uint32_t 		data0;
  	__IO	uint32_t 		data1;
  	__IO	uint32_t 		data2;
  	__IO	uint32_t 		data3;
	/*TextBlock */
	__IO	uint32_t		Textblock[16];
} MemoryMap;

static inline void MemMap_Flash_Unlock(void);
static inline void MemMap_Flash_Lock(void);
static int8_t MemMap_Flash_WriteWord(uint32_t address, uint32_t word);
static uint32_t MemMap_Flash_ReadWord(uint32_t address);
static int8_t MemMap_Flash_ErasePage(uint32_t pageAddress);
void MemMap_WriteAllEntriesToFlash(MemoryMap* map, uint32_t size);
void MemMap_ReadAllEntriesFromFlash(MemoryMap* map, uint32_t size);

#endif

#endif /* MEMORY_H_ */
