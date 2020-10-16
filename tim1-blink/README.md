- Alguns comandos para trabalhar com interrupção por timer:

NVIC_DisableIRQ(TIMx_IRQn);//Desabilita interrupção
NVIC_EnableIRQ(TIMx_IRQn);//Habilita interrupção

Onde "x" é o timer
 
Ex:
NVIC_DisableIRQ(TIM2_IRQn);//Desabilita interrupção pelo timer2

NVIC_EnableIRQ(TIM2_IRQn);//Habilita interrupção pelo timer2

