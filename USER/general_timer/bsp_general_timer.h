#ifndef __BSP_GENERAL_TIMER_H
#define __BSP_GENERAL_TIMER_H

#include "stm32f10x.h"

#define TIMx					TIM3

#define TIM_CLK				RCC_APB1Periph_TIM3
#define TIM_GPIO_CLK	(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)

#define COLOR_GPIO_REMAP_FUN() GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);

#define TIMx_IRQn					TIM3_IRQn
#define TIMx_IRQHandler		TIM3_IRQHandler

static void TIMx_GPIO_Config(void);
static void TIMx_Mode_Config(void);
static void TIMx_NVIC_Config(void);
void General_TIMx_Init(void);

#endif /* __BSP_GENERAL_TIMER_H */
