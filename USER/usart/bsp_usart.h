#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include <stdio.h>

//串口Usart1宏
#define DEBUG_USARTx 									USART1
#define DEBUG_USART_CLK 							RCC_APB2Periph_USART1
#define DEBUG_USART_APBxClkCmd 				RCC_APB2PeriphClockCmd
#define DEBUG_USART_BAUDRATE 					115200

//Usart GPIO引脚宏
#define DEBUG_USART_GPIO_CLK 					RCC_APB2Periph_GPIOA
#define DEBUG_USART_GPIO_APBxClkCmd 	RCC_APB2PeriphClockCmd
#define DEBUG_USART_TX_GPIO_PORT 			GPIOA
#define DEBUG_USART_TX_GPIO_PIN 			GPIO_Pin_9
#define DEBUG_USART_RX_GPIO_PORT 			GPIOA
#define DEBUG_USART_RX_GPIO_PIN 			GPIO_Pin_10

//Usart中断入口宏
#define DEBUG_USART_IRQ 							USART1_IRQn
#define DEBUG_USART_IRQHandler 				USART1_IRQHandler

void NVIC_Configuration(void);
void USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint32_t num);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

#endif /* __BSP_USART_H */
