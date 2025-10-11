# Utilizando a serial-UART para debugar

## main.h
```
/* Private includes ----------------------------------------------------------
*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */
...
/* USER CODE BEGIN Private defines */
// Enable/Disable logs
#define ENABLE_USER_LOG
1
#define ENABLE_DEBUG_LOG 0
/* -------------------------------------- */
/* ------------ LOG MACROS ------------- */
#if ENABLE_USER_LOG
#define USER_LOG(fmt, ...) printf("[USER] " fmt "\r\n", ##__VA_ARGS__)
#else
#define USER_LOG(fmt, ...)
#endif
#if ENABLE_DEBUG_LOG
#define DEBUG_LOG(fmt, ...) printf("[DEBUG] " fmt "\r\n", ##__VA_ARGS__)
#else
#define DEBUG_LOG(fmt, ...)
#endif
/* USER CODE END Private defines */
```

## main.c
```
/* Private user code ---------------------------------------------------------
*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len) {
HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 100);
return len;
}
/* USER CODE END 0 */
...
/* USER CODE BEGIN 2 */
if (ESP_Init() != ESP8266_OK){
USER_LOG("Failed to initialize... Check Debug logs");
Error_Handler();
}
if (ESP_ConnectWiFi("Aaaa", "bbb", ip_buf, sizeof(ip_buf)) != ESP8266_OK){
USER_LOG("Failed to connect to wifi... Check Debug logs");
Error_Handler();}
/* USER CODE END 2 */
```
