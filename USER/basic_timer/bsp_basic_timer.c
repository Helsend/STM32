#include "bsp_basic_Timer.h"

uint32_t counter=0;	//ms计时变量

void BASIC_TIM_NVIC_Config(void)
// 中断优先级配置
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//中断组为2
	NVIC_InitStructure.NVIC_IRQChannel=BASIC_TIM_IRQ;					//中断来源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;		//主优先级为2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;					//子优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//使能中断
	
	NVIC_Init(&NVIC_InitStructure);
}

void BASIC_TIM_Config(void)
//基本定时器配置
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK,ENABLE);				//开启定时器时钟，内部时钟CK_INT=72M
		
	TIM_TimeBaseStructure.TIM_Period=1;										//自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Prescaler=71;								//时钟预分频数，CK_CNT=CK_INT/(71+1)=1M
	TIM_TimeBaseInit(BASIC_TIM,&TIM_TimeBaseStructure);		//初始化
	
	TIM_ClearFlag(BASIC_TIM,TIM_FLAG_Update);							//清除计数器中断标志位	
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);					//开启计数器中断
	TIM_Cmd(BASIC_TIM,ENABLE);														//使能计数器
}

//void BASIC_TIM_IRQHandler (void)
////定时器中断函数
//{
//	if(TIM_GetITStatus(BASIC_TIM,TIM_IT_Update)!= RESET)
//	{
//		counter++;
//		TIM_ClearITPendingBit(BASIC_TIM,TIM_FLAG_Update);
//	}
//}
