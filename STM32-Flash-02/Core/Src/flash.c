/*! @file   flash.c
Biblioteca para gravar e ler a flash
*/

#include "flash.h"

void save_to_flash(uint8_t *data)
{
	volatile uint32_t data_to_FLASH[(strlen((char*)data)/4)	+ (int)((strlen((char*)data) % 4) != 0)];
	memset((uint8_t*)data_to_FLASH, 0, strlen((char*)data_to_FLASH));
	strcpy((char*)data_to_FLASH, (char*)data);

	volatile uint32_t data_length = (strlen((char*)data_to_FLASH) / 4)
									+ (int)((strlen((char*)data_to_FLASH) % 4) != 0);
	volatile uint16_t pages = (strlen((char*)data)/page_size)
									+ (int)((strlen((char*)data)%page_size) != 0);
	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();

	/* Allow Access to option bytes sector */
	HAL_FLASH_OB_Unlock();

	/* Fill EraseInit structure*/
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = FLASH_STORAGE;
	EraseInitStruct.NbPages = pages;
	uint32_t PageError;

	volatile uint32_t write_cnt=0, index=0;

	volatile HAL_StatusTypeDef status;
	status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
	while(index < data_length)
	{
		if (status == HAL_OK)
		{
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_STORAGE+write_cnt, data_to_FLASH[index]);
			if(status == HAL_OK)
			{
				write_cnt += 4;
				index++;
			 }
		 }
	  }

	  HAL_FLASH_OB_Lock();
	  HAL_FLASH_Lock();
}

void read_flash(uint8_t* data)
{
	volatile uint32_t read_data;
	volatile uint32_t read_cnt=0;
	do
	{
		read_data = *(uint32_t*)(FLASH_STORAGE + read_cnt);
		if(read_data != 0xFFFFFFFF)
		{
			data[read_cnt] = (uint8_t)read_data;
			data[read_cnt + 1] = (uint8_t)(read_data >> 8);
			data[read_cnt + 2] = (uint8_t)(read_data >> 16);
			data[read_cnt + 3] = (uint8_t)(read_data >> 24);
			read_cnt += 4;
		}
	}while(read_data != 0xFFFFFFFF);
}

