
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */
#include "hd44780.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void imitationPrinting(char *ptr);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint32_t SystemFreq = HAL_RCC_GetHCLKFreq();
  char str[17] = {};
  float pi = 3.14;
  int t = 123;
  unsigned char men1[FONT_HEIGHT] = {0x0e,0x0e,0x04,0x1f,0x04,0x1a,0x01,0x00};
  unsigned char men2[FONT_HEIGHT] = {0x07,0x07,0x0a,0x07,0x02,0x05,0x09,0x00};
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  lcdInit();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  lcdClrScr();
	  lcdGoto(LCD_1st_LINE,0);
	  lcdPuts("System Workbench\nFreq MCU ");
	  lcdItos(SystemFreq/1000000);
	  lcdPuts(" Mhz");
	  HAL_Delay(3000);

      for (int i = 0; i < 16; ++i)
      {
    	  lcdScroll(RIGHT);
	      HAL_Delay(250);
	  }

	  lcdClrScr();
	  sprintf(str,"STM32F103C8 and");
	  imitationPrinting(str);
	  sprintf(str,"LCD WH1602A Demo");
	  lcdGoto(LCD_2nd_LINE,0);
	  imitationPrinting(str);
	  HAL_Delay(3000);

	  for (int i = 0; i < 16; ++i)
	  {
		  lcdScroll(LEFT);
	      HAL_Delay(250);
	  }

	  HAL_Delay(1000);

	  for (int i = 0; i < 16; ++i)
	  {
		  lcdScroll(RIGHT);
	      HAL_Delay(250);
	  }

      HAL_Delay(2000);
      lcdClrScr();
      lcdPuts("Float = ");
      lcdFtos(pi,2);
      lcdGoto(LCD_2nd_LINE,0);
      lcdPuts("Integer = ");
      lcdItos(t);
      HAL_Delay(3000);
      lcdSetMode(VIEW_MODE_DispOn_BlkOff_CrsOn);
      lcdGoto(LCD_2nd_LINE,16);

      for (int i = 0; i < 16; ++i)
      {
    	  lcdBackSpace();
	      HAL_Delay(100);
	  }

      lcdGoto(LCD_1st_LINE,16);

      for (int i = 0; i < 16; ++i)
      {
	      lcdBackSpace();
	      HAL_Delay(100);
      }

      lcdSetMode(VIEW_MODE_DispOn_BlkOff_CrsOff);
      lcdReturn();
      lcdPuts("\tLoading...");
      lcdGoto(PROGRESS_BAR_LINE, PROGRESS_BAR_WIDTH);
      lcdPuts("[   %]");

      for(uint8_t i = 0; i <= PROGRESS_BAR_MAX_LOAD; ++i)
      {
    	  lcdGoto(PROGRESS_BAR_LINE, PROGRESS_BAR_WIDTH + 1);
	      lcdNtos((i*100)/PROGRESS_BAR_MAX_LOAD, 3);
	      lcdDrawBar(i);
	      HAL_Delay(100);
      }

      HAL_Delay(3000);
      lcdClrScr();
      lcdLoadChar(men1,6);
      lcdLoadChar(men2,7);

      lcdPuts("\tRunning men\n");

      for(uint8_t i = 0; i < 8; i++)
      {
	      lcdPutc(6);
	      HAL_Delay(250);
	      cursorShift(LEFT);
	      lcdPutc(' ');
	      lcdPutc(7);
	      HAL_Delay(250);
	      cursorShift(LEFT);
	      lcdPutc(' ');
	  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin 
                          |LCD_RS_Pin|LCD_E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_D4_Pin LCD_D5_Pin LCD_D6_Pin LCD_D7_Pin 
                           LCD_RS_Pin LCD_E_Pin */
  GPIO_InitStruct.Pin = LCD_D4_Pin|LCD_D5_Pin|LCD_D6_Pin|LCD_D7_Pin 
                          |LCD_RS_Pin|LCD_E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void imitationPrinting(char *ptr)
{

 lcdSetMode(VIEW_MODE_DispOn_BlkOff_CrsOn);

 while(*ptr != 0)
 {

  lcdPutc(*ptr++);

  HAL_Delay(100);
 }

 lcdSetMode(VIEW_MODE_DispOn_BlkOff_CrsOff);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
