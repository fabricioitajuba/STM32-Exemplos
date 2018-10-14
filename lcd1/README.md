Este simples programa faz a inicialização e escrita de um display 16x2 ou 16x4 que utiliza o controlador HD44780.

Microcontrolador utilizando a placa "Blue Pill"com o STM32F103C8

Fonte:
https://stm32withoutfear.blogspot.com/2016/10/stm32-lcd-wh16024-hd44780.html

A biblioteca utilizada é formada pelos arquivos: hd44780.c na pasta \src e os arquivos hd44780.h e lcd_cfg.h localizados na pasta \inc.

Lista de funções da biblioteca

A biblioteca pelo seu trabalho e conveniência do usuário - tem várias funções. Não consideraremos funções internas, mas consideremos as funções que precisamos para exibir várias informações.

lcdInit - função de inicialização da função de exibição inicial;

lcdClrScr - função de limpar a exibição, o cursor em sua posição inicial;

lcdReturn - Retorna o cursor para sua posição inicial;

lcdSetMode - configuração de modo: exibir on / off, cursor pisca on / off, cursor on / off;

lcdGoto - a função define o cursor para um local específico: uma linha, uma coluna;

lcdPuts - função de saída de linha de texto;

lcdPutc - função de saída de um tipo de caractere separado;

lcdLoadChar - função de carregar uma exibição personalizada de caracteres na memória;

lcdDrawChar - função de desenho na exibição de um caractere personalizado;

lcdBackSpace - a função de deslocar o cursor para um caractere à esquerda e destruí-lo;

lcdScroll - a função de deslocar todo o texto em uma coluna na direção certa (esquerda ou direita);

cursorShift é a função de mover o cursor para a posição direita ou esquerda sem destruir o caractere já representado. Projetado para edição / correção de texto;

lcdItos - função da saída dos números inteiros da tela;

lcdFos - função de exibir um número de tipo float;

lcdNtos é uma função de produzir um número de números inteiros com um certo número de dígitos. Descargas excessivas são cortadas. Desvantagens em falta são preenchidas com zeros. Para um local fixo na tela;

lcdDrawBar - função de desenho do indicador de desempenho;

lcdClrBar - indicador de função de limpeza.
