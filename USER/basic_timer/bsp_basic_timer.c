#include "bsp_basic_Timer.h"

uint32_t counter=0;	//ms��ʱ����

void BASIC_TIM_NVIC_Config(void)
// �ж����ȼ�����
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);						//�ж���Ϊ2
	NVIC_InitStructure.NVIC_IRQChannel=BASIC_TIM_IRQ;					//�ж���Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;		//�����ȼ�Ϊ2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;					//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//ʹ���ж�
	
	NVIC_Init(&NVIC_InitStructure);
}

void BASIC_TIM_Config(void)
//������ʱ������
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK,ENABLE);				//������ʱ��ʱ�ӣ��ڲ�ʱ��CK_INT=72M
		
	TIM_TimeBaseStructure.TIM_Period=1;										//�Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Prescaler=71;								//ʱ��Ԥ��Ƶ����CK_CNT=CK_INT/(71+1)=1M
	TIM_TimeBaseInit(BASIC_TIM,&TIM_TimeBaseStructure);		//��ʼ��
	
	TIM_ClearFlag(BASIC_TIM,TIM_FLAG_Update);							//����������жϱ�־λ	
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);					//�����������ж�
	TIM_Cmd(BASIC_TIM,ENABLE);														//ʹ�ܼ�����
}

//void BASIC_TIM_IRQHandler (void)
////��ʱ���жϺ���
//{
//	if(TIM_GetITStatus(BASIC_TIM,TIM_IT_Update)!= RESET)
//	{
//		counter++;
//		TIM_ClearITPendingBit(BASIC_TIM,TIM_FLAG_Update);
//	}
//}
