# Utilizando a serial-UART para debugar

## main.h
```
/* Private includes ----------------------------------------------------------
*/
/* USER CODE BEGIN Includes */
**#include <stdio.h>**
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
