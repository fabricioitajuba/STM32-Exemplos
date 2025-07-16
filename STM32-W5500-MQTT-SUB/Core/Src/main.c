/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <Internet/MQTT_interface.h>
#include <Internet/MQTTClient.h>
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

//w5500 related
#include "W5500/W5500_spi.h"
#include "../Inc/Ethernet/wizchip_conf.h"
#include "../Inc/Ethernet/socket.h"

//MQTT Related
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

wiz_NetInfo gWIZNETINFO = {
		.mac = { 0x80, 0x34, 0x28, 0x74, 0xA5, 0xCB },//MSB - LSB
		.ip ={ 192, 168, 1, 112 },
		.sn = { 255, 255, 255, 0 },
		.gw ={ 192, 168, 1, 1 },
		.dns = { 8, 8, 8, 8 },
		.dhcp = NETINFO_STATIC };

//IP Address of the MQTT broker
uint8_t destination_ip[]={192,168,1,36};
uint16_t destination_port = 1883;

MQTTClient mqtt_client;
Network network;
MQTTPacket_connectData connect_data=MQTTPacket_connectData_initializer;

uint8_t sendbuff[256],receivebuff[256];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

static void UWriteData(const char data);
static void PHYStatusCheck(void);
static void PrintPHYConf(void);
static void PrintBrokerIP(void);

//Topic handlers, Paho will call them when new message arrives
void OnTopicTemperature(MessageData*);
void OnTopicHumidity(MessageData*);

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

  //code below disables buffering of printf and sends output immediately to USART
  setbuf(stdout, NULL);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  printf("A Simple MQTT Client Subscription Application using W5500!\r\n");

  W5500Init();

  ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);

  //Configure PHY by software
  wiz_PhyConf phyconf;

  phyconf.by=PHY_CONFBY_SW;
  phyconf.duplex=PHY_DUPLEX_FULL;
  phyconf.speed=PHY_SPEED_10;
  phyconf.mode=PHY_MODE_AUTONEGO;//best to go with auto-negotiation

  ctlwizchip(CW_SET_PHYCONF, (void*) &phyconf);
  //*** End Phy Configuration

  PHYStatusCheck();
  PrintPHYConf();

  //MQTT Client Part
  connect_data.willFlag = 0;
  connect_data.MQTTVersion = 3;
  connect_data.clientID.cstring = "iotencew55";
  //connect_data.username.cstring = opts.username;
  //connect_data.password.cstring = opts.password;

  connect_data.keepAliveInterval = 60;//seconds
  connect_data.cleansession = 1;

  NewNetwork(&network, 1);//1 is the socket number to use
  PrintBrokerIP();
  printf("Connecting to MQTT Broker ...");

  if(ConnectNetwork(&network, destination_ip, destination_port)!=SOCK_OK){

	  printf("ERROR: Cannot connect with broker!\r\n");
  	  //Broker (server) not reachable
  	  while(1);
    }

  printf("SUCCESS\r\n");

  MQTTClientInit(&mqtt_client, &network, 1000, sendbuff, 256, receivebuff, 256);

  printf("Sending connect packet ...");

  if(MQTTConnect(&mqtt_client, &connect_data)!=MQTT_SUCCESS){

  	  printf("ERROR!");
  	  while(1);
  }

  printf("SUCCESS\r\n");

  //Subscribe to temperature sensor
  MQTTSubscribe(&mqtt_client, "room/temp",QOS0 , OnTopicTemperature);
  printf("Subscribed to topic room/temp\r\n");

    //Subscribe to humidity sensor
  MQTTSubscribe(&mqtt_client, "room/humidity",QOS0 , OnTopicHumidity);
  printf("Subscribed to topic room/humidity\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	//Transfer conton to Paho for 250 milliseconds
	MQTTYield(&mqtt_client, 250);

	//Do other application specific things like read sensors,inputs and update displays
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RST_Pin|SCS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RST_Pin SCS_Pin */
  GPIO_InitStruct.Pin = RST_Pin|SCS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void OnTopicTemperature(MessageData* msg_data){
	printf("New message on topic room/temp\r\n");
}

void OnTopicHumidity(MessageData* msg_data){
	printf("New message on topic room/humidity\r\n");
}

void UWriteData(const char data){

	while(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TXE)==RESET);
	//huart2.Instance->TDR=data;
	huart2.Instance->DR=data;
}

int __io_putchar(int ch){
	UWriteData(ch);
	return ch;
}

void PHYStatusCheck(void){

	uint8_t tmp;

	do{
		printf("\r\nChecking Ethernet Cable Presence ...");
		ctlwizchip(CW_GET_PHYLINK, (void*) &tmp);

		if(tmp == PHY_LINK_OFF){
			printf("NO Cable Connected!");
			HAL_Delay(1500);
		}
	}while(tmp == PHY_LINK_OFF);

	printf("Good! Cable got connected!");
}

void PrintPHYConf(void){

	wiz_PhyConf phyconf;

	ctlwizchip(CW_GET_PHYCONF, (void*) &phyconf);

	if(phyconf.by==PHY_CONFBY_HW){
		printf("\r\nPHY Configured by Hardware Pins");
	}
	else{
		printf("\r\nPHY Configured by Registers");
	}

	if(phyconf.mode==PHY_MODE_AUTONEGO){
		printf("\r\nAutonegotiation Enabled");
	}
	else{
		printf("\r\nAutonegotiation NOT Enabled");
	}

	if(phyconf.duplex==PHY_DUPLEX_FULL){
		printf("\r\nDuplex Mode: Full");
	}
	else{
		printf("\r\nDuplex Mode: Half");
	}

	if(phyconf.speed==PHY_SPEED_10){
		printf("\r\nSpeed: 10Mbps");
	}
	else{
		printf("\r\nSpeed: 100Mbps");
	}
}

static void PrintBrokerIP(void){
	printf("Broker IP: %d.%d.%d.%d\r\n",destination_ip[0],destination_ip[1],destination_ip[2],destination_ip[3]);
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
