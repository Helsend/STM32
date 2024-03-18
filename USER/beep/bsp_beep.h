#ifndef __BSP_BEEP_H
#define __BSP_BEEP_H

#include "stm32f10x.h"

#define BEEP_GPIO_PIN              GPIO_Pin_8
#define BEEP_GPIO_PORT             GPIOA
#define BEEP_GPIO_CLK              RCC_APB2Periph_GPIOA

#define beepHi(p,i) 			{p->BSRR=i;}
#define beepLo(p,i) 			{p->BRR=i;}
#define beepToggle(p,i) 	{p->ODR ^=i;}

#define BEEP_TOGGLE 				beepToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON 						beepHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF 						beepLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)

void BEEP_Init(void);

#endif /* __BSP_BEEP_H */
