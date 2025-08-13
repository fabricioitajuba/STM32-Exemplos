https://www.micropeta.com/video29#google_vignette

SD Card with Blue Pill using STM32CubeIDE

# Prerequisites

This project assumes you have already installed STM32CubeIDE. 
You need to have previously done a basic blink sketch with blue-pill using STM32CubeIDE. 
I have made a complete video from installing STM32CubeIDE to LED blink program. 
You can watch it by clicking this link. 

https://www.youtube.com/watch?v=kXg467nVd_A

# STM32CubeIDE Settings

Click connectivity → Click SPI1

For Mode select Full Duplex Master

Set PA4 to GPIO_Output

Click Middleware → Click FATFS

Tick User-defined

Configuration → Set Defines

For USE LFN (Use Long Filename) select Enabled with static working buffer on the BSS

For Max SS (Maximum Sector Size) select 4096

Libraries
Taken from https://blog.naver.com/eziya76/221188701172

Inside Core/Inc Folder

fatfs_sd.h
Inside Core/Src Folder

fatfs_sd.c

# Code modification

Inside Core/Src Folder
stm32f1xx_it.c

Inside FATFS/Target Folder
user_diskio.c

Additional code on top of STM32CubeIDE generated code (main.c)

/* USER CODE BEGIN PD */
FATFS fs;
FIL fil;
/* USER CODE END PD */

  /* USER CODE BEGIN 2 */
  HAL_Delay(500);
  f_mount(&fs, "", 0);
  f_open(&fil, "write.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
  f_lseek(&fil, fil.fsize);
  f_puts("Hello from Nizar\n", &fil);
  f_close(&fil);
  /* USER CODE END 2 */

# To check SD card capacity and free space

Additional code on top of STM32CubeIDE generated code (main.c)

/* USER CODE BEGIN PD */
FATFS *pfs;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
/* USER CODE END PD */

  /* USER CODE BEGIN 2 */
  f_getfree("", &fre_clust, &pfs);
  totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
  freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);
  /* USER CODE END 2 */

# To read from the SD card

Additional code on top of STM32CubeIDE generated code (main.c)

/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */
/* USER CODE BEGIN PD */
char buffer[100];
/* USER CODE END PD */

  /* USER CODE BEGIN 2 */
  f_open(&fil, "read.txt", FA_READ);
  // Reads line by line until the end
  while(f_gets(buffer, sizeof(buffer), &fil))
  {
    // Can use the buffer for something useful
    // memset(buffer,0,sizeof(buffer));
  }
  f_close(&fil);
  /* USER CODE END 2 */