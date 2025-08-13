### Exemplo de uso da biblioteca


## main.h
```
#define	LCD_RS GPIO_Pin_4
#define	LCD_RW GPIO_Pin_5
#define LCD_EN GPIO_Pin_6

#define LCD_DATA_PINS	GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3
#define LCD_DATA_PORT   GPIOA
#define LCD_CTL_PORT    GPIOA
```

## main.c
```
lcd_init();
lcd_clear();
lcd_puts("Temperatura:");

temp = 22;
lcd_gotoxy(0, 1);
//LCD_PutIntByte(temp);

LCD_PutHexByte(temp);
```
