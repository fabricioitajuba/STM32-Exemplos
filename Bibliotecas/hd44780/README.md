### Exemplo de uso da biblioteca


## lcd_cfg.h
Configura os pinos

## main.c
```
/* USER CODE BEGIN Includes */
#include "hd44780.h"
/* USER CODE END Includes */


/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t valor=5;


/* USER CODE END PV */

/* USER CODE BEGIN 2 */
lcdInit();

lcdClrScr();
lcdGoto(LCD_1st_LINE, 0);
lcdPuts("Valor:\nValor=");
lcdItos(valor);

lcdGoto(LCD_2nd_LINE, 0);
lcdPuts("Valor=    ");
lcdGoto(LCD_2nd_LINE, 6);
lcdItos(valor);

/* USER CODE END 2 */
```

