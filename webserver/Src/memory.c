/*
 * memory.c
 *
 *  Created on: 20 de fev de 2020
 *      Author: fabricio
 */


#include "memory.h"

static inline void MemMap_Flash_Unlock(void)
{
	if(FLASH->CR & FLASH_CR_LOCK)
	{
		FLASH->KEYR = FLASH_UNLOCK_KEY1;
		FLASH->KEYR = FLASH_UNLOCK_KEY2;
	}
}


static inline void MemMap_Flash_Lock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}


static int8_t MemMap_Flash_WriteWord(uint32_t address, uint32_t word)
{
	uint32_t timeout = FLASH_TIMEOUT;

	while(FLASH->SR & FLASH_SR_BSY)
	{
		if(--timeout == 0)
		{
			return -1;
		}
	}

	FLASH->CR |= FLASH_CR_PG;

	*(__IO uint16_t*)address = (uint16_t)word;

	timeout = FLASH_TIMEOUT;
	while(FLASH->SR & FLASH_SR_BSY)
	{
		if(--timeout == 0)
		{
			return -1;
		}
	}

	address += 2;
	*(__IO uint16_t*)address = (uint16_t)(word >> 16);

	timeout = FLASH_TIMEOUT;
	while(FLASH->SR & FLASH_SR_BSY)
	{
		if(--timeout == 0)
		{
			return -1;
		}
	}

	FLASH->CR &= ~FLASH_CR_PG;

	return 1;
}


static uint32_t MemMap_Flash_ReadWord(uint32_t address)
{
	return *(uint32_t*)address;
}


static int8_t MemMap_Flash_ErasePage(uint32_t pageAddress)
{
	uint32_t timeout = FLASH_TIMEOUT;

	while(FLASH->SR & FLASH_SR_BSY)
	{
		if(--timeout == 0)
		{
			return -1;
		}
	}

	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = pageAddress;
	FLASH->CR |= FLASH_CR_STRT;

	timeout = FLASH_TIMEOUT;
	while(FLASH->SR & FLASH_SR_BSY)
	{
		if(--timeout == 0)
		{
			return -1;
		}
	}

	if(FLASH->SR & FLASH_SR_EOP)
	{
		FLASH->SR &= ~FLASH_SR_EOP;
		FLASH->CR &= ~FLASH_CR_PER;
		return 1;
	}
	else
		return -1;
}

///*
void MemMap_WriteAllEntriesToFlash(MemoryMap* map, uint32_t size)
{
	uint32_t sizeOfUint32 = sizeof(uint32_t);
	MemMap_Flash_Unlock();

	if(MemMap_Flash_ErasePage(FLASH_MEMORYMAP_BASE_ADDR))
	{

		uint32_t* elemPtr = (uint32_t*)map;
		for(uint32_t elemCnt = 0; elemCnt < size; elemCnt++)
		{
			uint32_t word = *(elemPtr + elemCnt);
			uint32_t address = (uint32_t)(FLASH_MEMORYMAP_BASE_ADDR + (elemCnt * sizeOfUint32));
			MemMap_Flash_WriteWord(address, word);
		}
	}

	MemMap_Flash_Lock();
}


void MemMap_ReadAllEntriesFromFlash(MemoryMap* map, uint32_t size)
{
	uint32_t sizeOfUint32 = sizeof(uint32_t);
	uint32_t* elemPtr = (uint32_t*)map;
	for(uint32_t elemCnt = 0; elemCnt < size; elemCnt++)
	{
		uint32_t address = (uint32_t)(FLASH_MEMORYMAP_BASE_ADDR + (elemCnt * sizeOfUint32));
		uint32_t elemVal = MemMap_Flash_ReadWord(address);
		*(elemPtr + elemCnt) = elemVal;
	}
}

