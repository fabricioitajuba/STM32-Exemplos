```
https://www.youtube.com/watch?v=EYrHRSeAUUc
https://www.micropeta.com/video61
```

LCD 1602 I2C Display with Nucleo STM32F446RE using STM32CubeIDE

Prerequisites

This project assumes you have already installed STM32CubeIDE. You need to have previously done a basic blink sketch with Nucleo STM32F446RE using STM32CubeIDE. I have made a complete video from installing STM32CubeIDE to LED blink program. You can watch it by clicking this link. https://www.youtube.com/watch?v=oAwZ0cjlmN8

Wiring Diagram

STM32CubeIDE Settings

Click connectivity --> Click I2C1

For I2C select I2C

Set PB8 to I2C1_SCL

Set PB9 to I2C1_SDA

Libraries

Inside Core/Inc Folder

liquidcrystal_i2c.h
Inside Core/Src Folder

liquidcrystal_i2c.c
Line no 58 of liquidcrystal_i2c.h, change to #define DEVICE_ADDR (0x27 << 1) // 0x4E

Additional code on top of STM32CubeIDE generated code

```
/* USER CODE BEGIN Includes */
#include "liquidcrystal_i2c.h"
/* USER CODE END Includes */

  /* USER CODE BEGIN 2 */
  HD44780_Init(2);
  HD44780_Clear();
  HD44780_SetCursor(0,0);
  HD44780_PrintStr("HELLO");
  HD44780_SetCursor(10,1);
  HD44780_PrintStr("WORLD");
  HAL_Delay(2000);

  HD44780_Clear();
  HD44780_SetCursor(0,0);
  HD44780_PrintStr("HELLO");
  HAL_Delay(2000);
  HD44780_NoBacklight();
  HAL_Delay(2000);
  HD44780_Backlight();

  HAL_Delay(2000);
  HD44780_Cursor();
  HAL_Delay(2000);
  HD44780_Blink();
  HAL_Delay(5000);
  HD44780_NoBlink();
  HAL_Delay(2000);
  HD44780_NoCursor();
  HAL_Delay(2000);

  HD44780_NoDisplay();
  HAL_Delay(2000);
  HD44780_Display();

  HD44780_Clear();
  HD44780_SetCursor(0,0);
  HD44780_PrintStr("Learning STM32 with LCD is fun :-)");
  int x;
  for(int x=0; x<40; x=x+1)
  {
    HD44780_ScrollDisplayLeft();  //HD44780_ScrollDisplayRight();
    HAL_Delay(500);
  }

  char snum[5];
  for ( int x = 1; x <= 200 ; x++ )
  {
    itoa(x, snum, 10);
    HD44780_Clear();
    HD44780_SetCursor(0,0);
    HD44780_PrintStr(snum);
    HAL_Delay (1000);
  }
  /* USER CODE END 2 */


```
