### Exemplo de uso da biblioteca

## main.c
```
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "flash.h"
/* USER CODE END Includes */


  /* USER CODE BEGIN 2 */

  //Salvando uma string na flash interna
  char write_data[50];
  memset(write_data, 0, sizeof(write_data));
  strcpy(write_data, "Hello World!!!");
  save_to_flash((uint8_t*)write_data);

  //Lendo uma string na flash interna
  char read_data[50];
  memset(read_data, 0, sizeof(read_data));
  read_flash((uint8_t*)read_data);

  //Salvando um número float na flash interna
  float write_number = 235.756f;
  float *pointer_write = &write_number;
  save_to_flash((uint8_t*)pointer_write);

  //Lendo um número float na flash interna
  float read_number = 0.0f;
  float *pointer_read = &read_number;
  read_flash((uint8_t*)pointer_read);

  //Salvando um número int na flash interna
  char writeValue[30];
  uint8_t number_write = 43, number_read;
  uint8_t *pointer_write1 = &number_write;
  save_to_flash((uint8_t*)pointer_write1);

  //Lendo um número int na flash interna
  uint8_t *pointer_read1 = &number_read;
  read_flash((uint8_t*)pointer_read1);

  //Associando o número lido em uma string
  sprintf(writeValue,"O valor da variavel number= %d",number_read);

  /* USER CODE END 2 */

```
