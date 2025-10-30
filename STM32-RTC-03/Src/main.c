/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * RTC com STM32 e ESP8266 no modo LSE
  *
  * Autor: Eng. Fabrício Ribeiro
  * Data: 29/10/2025
  *
  * Status: Concluído mas o dia da semana não bate
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h> // For uint8_t
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INTERVAL1 1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
char dadotosend[100];
int len;
uint32_t last_time1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void atualiza(char *msg, RTC_TimeTypeDef _sTime, RTC_DateTypeDef _sDate);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  RTC_TimeTypeDef sTime;
  //WeekDay 0-Sun .. 6-Sat
  RTC_DateTypeDef sDate;

  //char *msg = "+CIPSNTPTIME:Wed Oct 29 10:23:00 2025\r\n";
  char *msg = "+CIPSNTPTIME:Sun Oct 30 15:56:00 2025\r\n";

  atualiza(msg, sTime, sDate);

  last_time1 = HAL_GetTick();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  	  if(HAL_GetTick()-last_time1 > INTERVAL1){

		last_time1 = HAL_GetTick();

		HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

		len = sprintf(dadotosend, "Time: %02d:%02d:%02d, Date: %d - %02d/%02d/%02d\r\n",
				sTime.Hours, sTime.Minutes, sTime.Seconds,
				sDate.WeekDay, sDate.Date, sDate.Month, sDate.Year+2000);

		HAL_UART_Transmit(&huart1, (uint8_t*)dadotosend, len, HAL_MAX_DELAY);
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JANUARY;
  DateToUpdate.Date = 1;
  DateToUpdate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void atualiza(char *msg, RTC_TimeTypeDef _sTime, RTC_DateTypeDef _sDate){

	//char *msg = "+CIPSNTPTIME:Wed Oct 29 10:23:00 2025\r\n";

	char dado[5];

	//Dia da semana
	dado[0] = msg[13];
	dado[1] = msg[14];
	dado[2] = msg[15];
	dado[3] = 0;

	if(!strcmp(dado, "Mon")){
		_sDate.WeekDay = 1;
	}
	else if(!strcmp(dado, "Tue")){
		_sDate.WeekDay = 2;
	}
	else if(!strcmp(dado, "Wed")){
		_sDate.WeekDay = 3;
	}
	else if(!strcmp(dado, "Thu")){
		_sDate.WeekDay = 4;
	}
	else if(!strcmp(dado, "Fri")){
		_sDate.WeekDay = 5;
	}
	else if(!strcmp(dado, "Sat")){
		_sDate.WeekDay = 6;
	}
	else if(!strcmp(dado, "Sun")){
		_sDate.WeekDay = 7;
	}
	//HAL_UART_Transmit(&huart1, (uint8_t*)dado, strlen(dado), HAL_MAX_DELAY);

	//Meês
	dado[0] = msg[17];
	dado[1] = msg[18];
	dado[2] = msg[19];
	dado[3] = 0;

	if(!strcmp(dado, "Jan")){
		_sDate.Month = 1;
	}
	else if(!strcmp(dado, "Feb")){
		_sDate.Month = 2;
	}
	else if(!strcmp(dado, "Mar")){
		_sDate.Month = 3;
	}
	else if(!strcmp(dado, "Apr")){
		_sDate.Month = 4;
	}
	else if(!strcmp(dado, "May")){
		_sDate.Month = 5;
	}
	else if(!strcmp(dado, "Jun")){
		_sDate.Month = 6;
	}
	else if(!strcmp(dado, "Jul")){
		_sDate.Month = 7;
	}
	else if(!strcmp(dado, "Aug")){
		_sDate.Month = 8;
	}
	else if(!strcmp(dado, "Sep")){
		_sDate.Month = 9;
	}
	else if(!strcmp(dado, "Oct")){
		_sDate.Month = 10;
	}
	else if(!strcmp(dado, "Nov")){
		_sDate.Month = 11;
	}
	else if(!strcmp(dado, "Dec")){
		_sDate.Month = 12;
	}

	HAL_UART_Transmit(&huart1, (uint8_t*)dado, strlen(dado), HAL_MAX_DELAY);

	//Dia
	dado[0] = msg[21];
	dado[1] = msg[22];
	dado[2] = 0;

	_sDate.Date = strtoul(dado, NULL, 10);

	HAL_UART_Transmit(&huart1, (uint8_t*)dado, strlen(dado), HAL_MAX_DELAY);

	//ano
	dado[0] = msg[35];
	dado[1] = msg[36];
	dado[2] = 0;

	_sDate.Year = strtoul(dado, NULL, 10);

	HAL_UART_Transmit(&huart1, (uint8_t*)dado, strlen(dado), HAL_MAX_DELAY);

	//Hora
	dado[0] = msg[24];
	dado[1] = msg[25];
	dado[2] = 0;

	_sTime.Hours = strtoul(dado, NULL, 10);

	HAL_UART_Transmit(&huart1, (uint8_t*)dado, strlen(dado), HAL_MAX_DELAY);

	//Minuto
	dado[0] = msg[27];
	dado[1] = msg[28];
	dado[2] = 0;

	_sTime.Minutes = strtoul(dado, NULL, 10);

	HAL_UART_Transmit(&huart1, (uint8_t*)dado, strlen(dado), HAL_MAX_DELAY);

	//Segundos
	dado[0] = msg[30];
	dado[1] = msg[31];
	dado[2] = 0;

	_sTime.Seconds = strtoul(dado, NULL, 10);

	HAL_UART_Transmit(&huart1, (uint8_t*)dado, strlen(dado), HAL_MAX_DELAY);

	//### Acerto
	//_sTime.Hours = 11;
	//_sTime.Minutes = 20;
	//_sTime.Seconds = 00;

	if(HAL_RTC_SetTime(&hrtc, &_sTime, RTC_FORMAT_BIN) != HAL_OK){
	    Error_Handler();
	}

	//_sDate.Date = 30;
	//_sDate.Month = 10;
	//_sDate.Year = 25;
	//_sDate.WeekDay = 4;

	if(HAL_RTC_SetDate(&hrtc, &_sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
