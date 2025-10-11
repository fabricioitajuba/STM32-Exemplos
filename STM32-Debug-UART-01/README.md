# Utilizando a serial-UART para debugar

## STM32
![STM32](https://github.com/fabricioitajuba/STM32-Exemplos/blob/master/STM32-Debug-UART-01/Config/Configura%C3%A7%C3%A3o.png)

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

## Utilizando o terminal do Linux para enviar e receber dados pela serial (utilizando FTDI)

### - Verificar os dispositivos conectados:
```
$ lsusb
```

### - Para saber em qual porta foi atribuida para o FTDI:
```
$ sudo dmesg | grep -i 'attached to'
```
No Linux, as portas USB e COM são tratadas como arquivos especiais localizados no diretório /dev.  
As portas COM geralmente começam com ttyS (para portas COM padrão) ou ttyUSB (para adaptadores USB para serial).  

### Para listar as portas COM:
```
$ ls /dev/ttyS*
```

### Para listar as portas USB:
```
$ ls /dev/ttyUSB*
```

### - Para habilitar o uso do dispotivo (ex: ttyUSB0):
```
$ sudo chmod 777 /dev/ttyUSB0
```

### - Configurando a taxa de transmissão da porta serial:
```
$ stty -F /dev/ttyUSB0 speed 115200
```

### - Enviando dados pela serial:
```
$ echo "helloworld" >/dev/ttyUSB0
```

### - Recebendo dandos pela serial:
```
$ cat /dev/ttyUSB0
```
