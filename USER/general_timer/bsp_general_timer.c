#include "bsp_general_timer.h"

static void TIMx_GPIO_Config(void)
//GPIO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(TIM_GPIO_CLK,ENABLE);			//使能GPIO
	
	COLOR_GPIO_REMAP_FUN();
	
	//初始化输出引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

static void TIMx_Mode_Config(void)
//定时器PWM配置
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(TIM_CLK,ENABLE);		//使能TIMx时钟
	
	//定时器配置
	TIM_TimeBaseStructure.TIM_Period=1000;											//定时周期
	TIM_TimeBaseStructure.TIM_Prescaler=72-1;										//预分频：1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;				//时钟分频系数：不分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//向上计数模式
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	//PWM配置
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;							//配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	//使能输出 
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;			//计数值小于CCR_Val时为低电平
	//设置通道跳变值，使能通道和预装载
	TIM_OCInitStructure.TIM_Pulse=0;						//通道2
	TIM_OC2Init(TIMx,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);
	TIM_OCInitStructure.TIM_Pulse=0;						//通道3
	TIM_OC3Init(TIMx,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);
	TIM_OCInitStructure.TIM_Pulse=0;						//通道4
	TIM_OC4Init(TIMx,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);	//使能TIMx重载寄存器ARR
	TIM_Cmd(TIMx,ENABLE);								//使能计数器
	
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);	//使能update中断
}

static void TIMx_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIMx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void General_TIMx_Init(void)
{
	TIMx_GPIO_Config();
	TIMx_Mode_Config();
	TIMx_NVIC_Config();
}
