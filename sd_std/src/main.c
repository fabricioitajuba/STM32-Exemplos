/**
  ******************************************************************************
  * @file    main.c
  * @author  Eng. Fabrício de Lima Ribeiro
  * @version V1.0
  * @date    03-Novembro-2018
  * @brief   Exemplo de uso do cartão sd
  ******************************************************************************
 STM32 - SD
 PA4   - CS
 PA5   - SCK
 PA6   - MISO
 PA7   - MOSI
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "ff.h"

FRESULT fr;
FATFS fs;
FIL fil;

char buff[32];
char logName[64];
char timestamp[32];

uint8_t enable = 1;
uint16_t numToAppend = 1;
uint32_t var, dado;


FRESULT open_append(FIL* fp, /* [OUT] File object to create */
const char* path /* [IN]  File name to be opened */
) {

	/* Opens an existing file. If not exist, creates a new file. */
	fr = f_open(fp, path, FA_WRITE | FA_OPEN_ALWAYS);
	if (fr == 0) {
		/* Seek to end of the file to append data */
		fr = f_lseek(fp, f_size(fp));
		if (fr != 0)
			f_close(fp);
	}
	return fr;
}

int main(void)
{
	//Inicializa o pino PC13
	GPIO_InitTypeDef gpioInitStruct;
	GPIO_StructInit(&gpioInitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioInitStruct.GPIO_Pin = GPIO_Pin_13;
	gpioInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &gpioInitStruct);
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, 1);

	//Cria o arquivo com o nome teste.txt
	var = 1;
	dado = 73;
	sprintf(timestamp, "%d", var);

	sprintf(logName, "teste.txt");
	f_mount(&fs, "", 0);
	fr = FR_OK;
	do
	{
		fr = f_open(&fil, logName, FA_OPEN_EXISTING);
		if (fr == FR_OK)
		{
			f_close(&fil);
			sprintf(logName, "teste_%d.txt", numToAppend);
			numToAppend++;
		}
	}while (fr == FR_OK);


	f_mount(&fs, "", 0);
	/* Open or create a log file and ready to append */
	fr = open_append(&fil, logName);
	if (fr == FR_OK)
	{
		/* Append a line */
		f_puts("Trabalhando com Arquivos\n", &fil);
		sprintf(buff, "Teste: \t%s\n\n", timestamp);
		f_puts(buff, &fil);
		f_close(&fil);
	}

/*
	sprintf(buff, "Dado =%d\n", dado);
	f_mount(&fs, "", 0);
	fr = open_append(&fil, logName);
	if (fr == FR_OK)
	{
		f_puts(buff, &fil);
		f_close(&fil);
	}
*/

	while(1)
	{
		if(enable == 1)
		{
			for(int i=1; i<=10; i++)
			{
				sprintf(buff, "Dado%d = %d\n",i, dado);
				f_mount(&fs, "", 0);
				fr = open_append(&fil, logName);
				if (fr == FR_OK)
				{
					f_puts(buff, &fil);
					f_close(&fil);
				}
			}
		}
		enable = 0;
	}
}
