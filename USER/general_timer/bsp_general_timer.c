#include "bsp_general_timer.h"

static void TIMx_GPIO_Config(void)
//GPIO��ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(TIM_GPIO_CLK,ENABLE);			//ʹ��GPIO
	
	COLOR_GPIO_REMAP_FUN();
	
	//��ʼ���������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

static void TIMx_Mode_Config(void)
//��ʱ��PWM����
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(TIM_CLK,ENABLE);		//ʹ��TIMxʱ��
	
	//��ʱ������
	TIM_TimeBaseStructure.TIM_Period=1000;											//��ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler=72-1;										//Ԥ��Ƶ��1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;				//ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		//���ϼ���ģʽ
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	//PWM����
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;							//����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;	//ʹ����� 
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;			//����ֵС��CCR_ValʱΪ�͵�ƽ
	//����ͨ������ֵ��ʹ��ͨ����Ԥװ��
	TIM_OCInitStructure.TIM_Pulse=0;						//ͨ��2
	TIM_OC2Init(TIMx,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIMx,TIM_OCPreload_Enable);
	TIM_OCInitStructure.TIM_Pulse=0;						//ͨ��3
	TIM_OC3Init(TIMx,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIMx,TIM_OCPreload_Enable);
	TIM_OCInitStructure.TIM_Pulse=0;						//ͨ��4
	TIM_OC4Init(TIMx,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIMx,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIMx,ENABLE);	//ʹ��TIMx���ؼĴ���ARR
	TIM_Cmd(TIMx,ENABLE);								//ʹ�ܼ�����
	
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);	//ʹ��update�ж�
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
