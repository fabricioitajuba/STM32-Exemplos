/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  *
  *STM32 com W5500
  *www.micropeta.com/video109
  *https://www.youtube.com/watch?v=uTeMOO7paQs
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "Ethernet/socket.h"
#include "Internet/dhcp.h"
#include "Internet/httpServer.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#define DHCP_SOCKET     0
#define DNS_SOCKET      1
#define HTTP_SOCKET     2
#define SOCK_TCPS       0
#define SOCK_UDPS       1
#define PORT_TCPS       5000
#define PORT_UDPS       3000
#define MAX_HTTPSOCK    6
#define index_page "<!DOCTYPE html>"\
  "<html>"\
    "<head>"\
      "<title>W5500-STM32 Web Server</title>"\
      "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"\
      "<link href=\"data:image/x-icon;base64,A\" rel=\"icon\" type=\"image/x-icon\">"\
      "<style>"\
        "html {display: inline-block; margin: 0px auto; text-align: center;}"\
        "body{margin-top: 50px;}"\
        ".button {display: block;"\
          "width: 70px;"\
          "background-color: #008000;"\
          "border: none;"\
          "color: white;"\
          "padding: 14px 28px;"\
          "text-decoration: none;"\
          "font-size: 24px;"\
          "margin: 0px auto 36px;"\
          "border-radius: 5px;}"\
        ".button-on {background-color: #008000;}"\
        ".button-on:active{background-color: #008000;}"\
        ".button-off {background-color: #808080;}"\
        ".button-off:active {background-color: #808080;}"\
        "p {font-size: 20px;color: #808080;margin-bottom: 20px;}"\
      "</style>"\
    "</head>"\
    "<body>"\
      "<h1>STM32 - W5500</h1>"\
      "<p>Control the light via Ethernet</p>"\
      "<a class=\"button button-on\" href=\"/ledon.html\">ON</a>"\
      "<a class=\"button button-off\" href=\"/ledoff.html\">OFF</a>"\
    "</body>"\
  "</html>"

#define ledon_page" <!DOCTYPE html>"\
  "<html>"\
    "<head>"\
      "<title>W5500-STM32 Web Server</title>"\
      "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"\
      "<link href=\"data:image/x-icon;base64,A\" rel=\"icon\" type=\"image/x-icon\">"\
      "<style>"\
        "html {display: inline-block; margin: 0px auto; text-align: center;}"\
        "body{margin-top: 50px;}"\
        ".button {display: block;"\
          "width: 70px;"\
          "background-color: #008000;"\
          "border: none;"\
          "color: white;"\
          "padding: 14px 28px;"\
          "text-decoration: none;"\
          "font-size: 24px;"\
          "margin: 0px auto 36px;"\
          "border-radius: 5px;}"\
        ".button-on {background-color: #008000;}"\
        ".button-on:active{background-color: #008000;}"\
        ".button-off {background-color: #808080;}"\
        ".button-off:active {background-color: #808080;}"\
        "p {font-size: 20px;color: #808080;margin-bottom: 20px;}"\
      "</style>"\
    "</head>"\
    "<body>"\
      "<h1>STM32 - W5500</h1>"\
      "<p>Light is currently on</p>"\
      "<a class=\"button button-off\" href=\"/ledoff.html\">OFF</a>"\
    "</body>"\
  "</html>"

#define ledoff_page"<!DOCTYPE html>"\
  "<html>"\
    "<head>"\
      "<title>W5500-STM32 Web Server</title>"\
      "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>"\
      "<link href=\"data:image/x-icon;base64,A\" rel=\"icon\" type=\"image/x-icon\">"\
      "<style>"\
        "html {display: inline-block; margin: 0px auto; text-align: center;}"\
        "body{margin-top: 50px;}"\
        ".button {display: block;"\
          "width: 70px;"\
          "background-color: #008000;"\
          "border: none;"\
          "color: white;"\
          "padding: 14px 28px;"\
          "text-decoration: none;"\
          "font-size: 24px;"\
          "margin: 0px auto 36px;"\
          "border-radius: 5px;}"\
        ".button-on {background-color: #008000;}"\
        ".button-on:active{background-color: #008000;}"\
        ".button-off {background-color: #808080;}"\
        ".button-off:active {background-color: #808080;}"\
        "p {font-size: 20px;color: #808080;margin-bottom: 20px;}"\
      "</style>"\
    "</head>"\
    "<body>"\
      "<h1>STM32 - W5500</h1>"\
      "<p>Light is currently off</p>"\
      "<a class=\"button button-on\" href=\"/ledon.html\">ON</a>"\
    "</body>"\
  "</html>"

uint8_t socknumlist[] = {2, 3, 4, 5, 6, 7};
uint8_t RX_BUF[1024];
uint8_t TX_BUF[1024];
wiz_NetInfo net_info = {
    .mac  = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED },
    .dhcp = NETINFO_DHCP
};



uint8_t dhcp_buffer[1024];
uint8_t dns_buffer[1024];

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

  W5500Init();
  httpServer_init(TX_BUF, RX_BUF, MAX_HTTPSOCK, socknumlist);
  reg_httpServer_cbfunc(NVIC_SystemReset, NULL);
  /* Web content registration */
  reg_httpServer_webContent((uint8_t *)"index.html", (uint8_t *)index_page);
  reg_httpServer_webContent((uint8_t *)"ledon.html", (uint8_t *)ledon_page);
  reg_httpServer_webContent((uint8_t *)"ledoff.html", (uint8_t *)ledoff_page);

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 for(int i = 0; i < MAX_HTTPSOCK; i++) httpServer_run(i); // HTTP Server handler
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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
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
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void wizchipSelect(void) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
}

void wizchipUnselect(void) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

void wizchipReadBurst(uint8_t* buff, uint16_t len) {
    HAL_SPI_Receive(&hspi1, buff, len, HAL_MAX_DELAY);
}

void wizchipWriteBurst(uint8_t* buff, uint16_t len) {
    HAL_SPI_Transmit(&hspi1, buff, len, HAL_MAX_DELAY);
}

uint8_t wizchipReadByte(void) {
    uint8_t byte;
    wizchipReadBurst(&byte, sizeof(byte));
    return byte;
}

void wizchipWriteByte(uint8_t byte) {
    wizchipWriteBurst(&byte, sizeof(byte));
}

volatile bool ip_assigned = false;

void Callback_IPAssigned(void) {
    ip_assigned = true;
}

void Callback_IPConflict(void) {
    ip_assigned = false;
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
