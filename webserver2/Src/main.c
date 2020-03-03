/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "memory.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/*
 * Programa para trabalhar com o ENC28J60 utilizando o STM32F103C8 com a placa BluePill
 * Data: 18/02/2020
 * Adaptação: Eng. Fabrício de Lima Ribeiro
 */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MYWWWPORT 80
#define BUFFER_SIZE 550
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

static uint8_t mymac[6] = { 0x54, 0x55, 0x58, 0x10, 0x00, 0x25 };
//static uint8_t myip[4] = {192,168,0,15};
static uint8_t buf[BUFFER_SIZE + 1];
uint16_t plen, dat_p;

/* For DHCP */
static uint8_t myip[4] = { 0, 0, 0, 0 };
static uint8_t mynetmask[4] = { 0, 0, 0, 0 };

// IP address of the host being queried to contact (IP of the first portion of the URL):
static uint8_t websrvip[4] = { 0, 0, 0, 0 };

// Default gateway. The ip address of your DSL router. It can be set to the same as
// websrvip the case where there is no default GW to access the
// web server (=web server is on the same lan as this host)
static uint8_t gwip[4] = { 192, 168, 0, 1 };
static uint8_t dnsip[4] = { 0, 0, 0, 0 };
static uint8_t dhcpsvrip[4] = { 0, 0, 0, 0 };

//Reservando área da flah interna
MemoryMap map;
static const uint32_t memoryMapSize = sizeof(map) / sizeof(uint32_t);

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* Private function prototypes */
uint16_t http200ok(void);
uint16_t print_webpage(uint8_t *buf);
uint16_t print_webpage2(uint8_t *buf);

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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  // Initialise SPI interface
  ES_enc28j60SpiInit(&hspi1);

  // initialize enc28j60
  ES_enc28j60Init(mymac);

  // init the ethernet/ip layer:
  //ES_init_ip_arp_udp_tcp(mymac,myip, MYWWWPORT);

  if(ES_enc28j60Revision() <= 0)
  {
	// Failed to access ENC28J60
	while (1)
		;    // Just loop here
  }

  // Get IP Address details
  if (allocateIPAddress(buf, BUFFER_SIZE, mymac, 80, myip, mynetmask, gwip, dhcpsvrip, dnsip) > 0)
  {
		// Display the results:
  }
  else
  {
	// Failed to get IP address"
	while (1)
		;
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		// read packet, handle ping and wait for a tcp packet:
		dat_p = ES_packetloop_icmp_tcp(buf, ES_enc28j60PacketReceive(BUFFER_SIZE, buf));

		/* dat_p will be unequal to zero if there is a valid
		 * http get */
		if (dat_p == 0)
		{
			// no http request
			continue;
		}

		// tcp port 80 begin
		if (strncmp("GET ", (char *) &(buf[dat_p]), 4) != 0)
		{
			// head, post and other methods:
			dat_p = http200ok();
			dat_p = ES_fill_tcp_data(buf, dat_p, "<h1>200 OK</h1>");
			goto SENDTCP;
		}

		// Página principal
		if (strncmp("/ ", (char *) &(buf[dat_p + 4]), 2) == 0)
		{
			dat_p = print_webpage(buf);
			goto SENDTCP;
		}
		else if (strncmp("/test ", (char *) &(buf[dat_p + 4]), 2) == 0)
		{
			dat_p = print_webpage2(buf);
			goto SENDTCP;
		}
		else if (strncmp("/?", (char *) &(buf[dat_p + 4]), 2) == 0)
		{
			//Salva valores na flash
			char mat[3];
			int cont=0, i=4, offset=0, n1=0, n2=1;

			while(n1 != n2)
			{
				if(buf[dat_p + i] == '=')
				{
					i++;

					if(cont < 2)
					{
						while(buf[dat_p + i] != '&')
						{
							mat[offset] = buf[dat_p + i];
							offset++;
							i++;
						}
					}
					else
					{
						while(buf[dat_p + i] != ' ')
						{
							mat[offset] = buf[dat_p + i];
							offset++;
							i++;
						}
					}

					offset = 0;

					if(cont == 0)
					{
						map.data0=atoi(mat);
						cont++;
					}
					else if(cont == 1)
					{
						map.data1=atoi(mat);
						cont++;
					}
					else if(cont == 2)
					{
						map.data2=atoi(mat);
						cont++;
						n1++;
					}
				}
				i++;
			}

			/*Store map to flash*/
			MemMap_WriteAllEntriesToFlash(&map, memoryMapSize);

			dat_p = print_webpage2(buf);
			goto SENDTCP;
		}
		else
		{
			dat_p = ES_fill_tcp_data(buf, 0, "HTTP/1.0 401 Unauthorized\r\nContent-Type: text/html\r\n\r\n<h1>401 Unauthorized</h1>");
			goto SENDTCP;
		}
		SENDTCP: ES_www_server_reply(buf, dat_p); // send web page data
		// tcp port 80 end
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

  /** Initializes the CPU, AHB and APB busses clocks 
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
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
uint16_t http200ok(void)
{
	return (ES_fill_tcp_data(buf, 0, "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage(uint8_t *buf)
{
	uint16_t plen;
	char writeValue[50];
	int var1, var2, var3;

	plen = http200ok();
	plen = ES_fill_tcp_data(buf, plen, "<!DOCTYPE html><head><title>STM32F1 ENC28J60 Ethernet Demo</title></head>");

	plen = ES_fill_tcp_data(buf, plen, "<body><h2>Dados na memoria:</h2>");

	MemMap_ReadAllEntriesFromFlash(&map, memoryMapSize);

	var1 = (int) map.data0;
	sprintf(writeValue,"<br>O valor da variavel 1 = %d", var1);
	plen = ES_fill_tcp_data(buf, plen, writeValue);

	var2 = (int) map.data1;
	sprintf(writeValue,"<br>O valor da variavel 2 = %d",var2);
	plen = ES_fill_tcp_data(buf, plen, writeValue);

	var3 = (int) map.data2;
	sprintf(writeValue,"<br>O valor da variavel 3 = %d",var3);
	plen = ES_fill_tcp_data(buf, plen, writeValue);

	plen = ES_fill_tcp_data(buf, plen, "<br> ");

	plen = ES_fill_tcp_data(buf, plen, "<br> <a href='/test'><button>Alterar dados</button></a>");
	plen = ES_fill_tcp_data(buf, plen, "</body></html>");

	return (plen);
}

// prepare the webpage by writing the data to the tcp send buffer
uint16_t print_webpage2(uint8_t *buf)
{
	uint16_t plen;

	plen = http200ok();
	plen = ES_fill_tcp_data(buf, plen, "<!DOCTYPE html><head><title>Test page</title></head><body>");
	plen = ES_fill_tcp_data(buf, plen, "<h1>Alterar dados: </h1>");

	plen = ES_fill_tcp_data(buf, plen, "<form method='GET' action='/ '>");
	plen = ES_fill_tcp_data(buf, plen, "<br>Variavel 1: <input type='text' name='var1'/><br/>");
	plen = ES_fill_tcp_data(buf, plen, "<br>Variavel 2: <input type='text' name='var2'/><br/>");
	plen = ES_fill_tcp_data(buf, plen, "<br>Variavel 3: <input type='text' name='var3'/><br/>");
	plen = ES_fill_tcp_data(buf, plen, "<br><input type='submit' value='Insere dados'/>");
	plen = ES_fill_tcp_data(buf, plen, "</form>");

	plen = ES_fill_tcp_data(buf, plen, "<br> <a href='/'><button>Voltar</button></a>");

	plen = ES_fill_tcp_data(buf, plen, "</body></html>");

	return (plen);
}

void ES_PingCallback(void)
{
	//STM_EVAL_LEDToggle(LED4);
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
