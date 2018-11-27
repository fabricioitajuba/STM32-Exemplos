/*
 * DFPlayer.h
 *
 *  Created on: 22 de nov de 2018
 *      Author: Fabricio
 *      Versão: 1.3 - Testado versão beta
 *      Última atualização: 24/11/18
 *      obs:
 */

#ifndef DFPLAYER_H_
#define DFPLAYER_H_

//Definições da Inicialização
//Ex: ini_DFPlayer(INI_FLASH);
#define INI_U_DISK 			0x01
#define INI_TF_CARD			0x02
#define INI_PC				0x04
#define INI_FLASH			0x08
#define INI_U_DISK_TF_CARD	0x03

//Tipo de fonte da música
//Ex: specify_playback_source(FLASH);
#define _UT 		0
#define _TF 		1
#define _AUX 		2
#define _SLEEP 		3
#define _FLASH 		4

//Tipos de equalização
//Ex: specify_eq(ROCK);
#define NORMAL	0
#define POP		1
#define ROCK	2
#define JAZZ	3
#define CLASSIC	4
#define BASE	5

//Modo de playback
//Ex: specify_playback_mode(REPEAT)
#define REPEAT			0
#define FOLDER_REPEAT	1
#define SINGLE_REPEAT	2
#define RANDOM			3

//Variáveis
uint8_t msgBuf[10]; //buffer de recepção

// USART routines ** specific to USART1
void SetupUSART(void);
void USART_PutChar(char c);
void USART_PutStr(char *str);
void USART_GetStr(void);

void send_comand_DFPlayer(uint8_t cmd, uint8_t feedback, uint8_t para1, uint8_t para2);
uint16_t query_system(uint8_t query);
void ini_DFPlayer(uint8_t ini);

void next(void);
void previous(void);
void specify_tracking(uint16_t num);
void increase_volume(void);
void decrease_volume(void);
void volume(uint8_t vol);
void specify_eq(uint8_t eq);
void specify_playback_mode(uint8_t mode);
void specify_playback_source(uint8_t source);
void normal_working(void);
void playback(void);
void pause(void);

#endif /* DFPLAYER_H_ */
