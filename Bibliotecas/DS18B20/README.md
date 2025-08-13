### Exemplo de uso da biblioteca

```
## main.c

#include "ds18b20.h"
#include "one_wire.h"


simple_float temperature;
int temp;


// Configure DS18B20 and 1-Wire on pin B12
// Please remember about adding pull-up resistor :)
ds18b20_init(GPIOB, GPIO_Pin_12, TIM2);

temperature = ds18b20_get_temperature_simple();
```