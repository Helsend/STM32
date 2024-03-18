#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f10x.h"

#define ADCx										ADC1
#define ADC_APBxClock_FUN				RCC_APB2PeriphClockCmd
#define ADC_CLK									RCC_APB2Periph_ADC1

#define ADC_GPIO_APBxClock_FUN	RCC_APB2PeriphClockCmd
#define ADC_GPIO_CLK						RCC_APB2Periph_GPIOC
#define ADC_PORT								GPIOC
#define ADC_PIN1								GPIO_Pin_0
#define ADC_CHANNEL1						ADC_Channel_16

#define NOFCHANEL 							1

#define ADC_DMA_CHANNEL					DMA1_Channel1

#define ADC_IRQ									ADC1_2_IRQn
#define ADC_IRQHandler					ADC1_2_IRQHandler

extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];
extern __IO float ADC_ConvertedValueLocal[NOFCHANEL];

void ADCx_GPIO_Config(void);
void ADCx_Mode_Config(void);
void ADC_NVIC_Config(void);
void ADCx_Init(void);

#endif /* __BSP_ADC_H */
