#ifndef __BSP_BASIC_TIMER_H
#define __BSP_BASIC_TIMER_H

#include "stm32f10x.h"

//������ʱ����غ�
#define BASIC_TIM6	// ���ʹ�� TIM7��ע�͵�����꼴��
#ifdef BASIC_TIM6
#define BASIC_TIM									TIM6
#define BASIC_TIM_APBxClock_FUN		RCC_APB1PeriphClockCmd
#define BASIC_TIM_CLK							RCC_APB1Periph_TIM6
#define BASIC_TIM_IRQ							TIM6_IRQn
#define BASIC_TIM_IRQHandler			TIM6_IRQHandler
#else
#define BASIC_TIM									TIM7
#define BASIC_TIM_APBxClock_FUN		RCC_APB1PeriphClockCmd
#define BASIC_TIM_CLK							RCC_APB1Periph_TIM7
#define BASIC_TIM_IRQ							TIM7_IRQn
#define BASIC_TIM_IRQHandler			TIM7_IRQHandler
#endif

extern uint32_t NumberOfMS;

void BASIC_TIM_NVIC_Config(void);
void BASIC_TIM_Config(void);

#endif


