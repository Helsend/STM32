#include "main.h"

float Alarm_Temperature=30.0;	//�¶���ֵ
uint16_t R=128,G=255,B=255;	//LED����ɫRGB����
uint16_t tone[]={262,294,330,349,392,440,494,523,587,659,698,784,880,988,1046,1175,1318,1397,1568,1760,1976,0};
uint8_t Music[]={11,14,13,15,14,11,9,12,10,12,15,14,13,12,11,10,9,11,14,13,15,14,11,9,12,10,12,15,14,13,15,17,13,14};	//����
uint8_t Time[]={2,6,2,2,2,2,2,8,6,2,6,2,2,2,2,2,8,2,6,2,2,2,2,2,8,2,2,2,2,4,4,4,4,6};	//���ڳ���ʱ��
uint8_t music_len=34;	//����������

uint8_t sample_interval_count=0;	//�¶Ȳ��������������0.1s����һ��
uint8_t state=0;			//״̬������0Ϊ����������1Ϊ����
uint8_t key_flag=0;		//��λ��������ϰ���ֹͣ������־
uint16_t count=0;			//���������¶ȳ��������趨�¶�3s���ϲű�������������ֵ����ʱ�Ĳ���������
uint8_t select=0;//����ָ�����
uint16_t space_count=0;	//���֡��ա�ʱ�ļ�����


int main(void)
{
	//��ʼ��
	EXTI_Key_Config();
	BEEP_Init();
	ADCx_Init();
	USART_Config();
	General_TIMx_Init();
	BASIC_TIM_Config();
	while(1)
	{
		
	}
}

void BASIC_TIM_IRQHandler (void)
//������ʱ���жϺ���
{
	static uint32_t i=0;
	static uint32_t j=0;
	if(TIM_GetITStatus(BASIC_TIM,TIM_IT_Update)!= RESET)
	{
		//��Դ��������������
		if(state==1&&tone[Music[select]])	//����ֹ��
		{
			if(i!=(Time[select]*tone[Music[select]]/16))	//��������
			{
				//������pwm
				if(j++<50000/tone[Music[select]])BEEP_ON
				else if(j<500000/tone[Music[select]])BEEP_OFF
				else{i++;j=0;}	//��һ������
			}
			else{i=0;j=0;select=(select+1)%music_len;}	//��һ������
		}
		else if(state==1&&tone[Music[select]]==0)	//��ֹ��
		{
			BEEP_OFF
			if(space_count++>=1000000/16){space_count=0;select=(select+1)%14;}
		}
		TIM_ClearITPendingBit(BASIC_TIM,TIM_FLAG_Update);
	}
}

void TIMx_IRQHandler(void)
//ͨ�ö�ʱ��ÿ1ms����һ���ж�
{
	if(TIM_GetITStatus(TIMx,TIM_IT_Update)!=RESET)
	{
		static uint16_t flag=0;
		static uint16_t temp=1;
		
		if(sample_interval_count++==100)//0.1s����һ��
		{
			//ADC�¶�ת������
			ADC_ConvertedValueLocal[0]=(1.43-ADC_ConvertedValue[0]*3.3/4096)*1000/4.35+25;
			printf("%f\n",ADC_ConvertedValueLocal[0]);
			sample_interval_count=0;
		}
		
		//״̬ת�����ж�
		if(ADC_ConvertedValueLocal[0]>=Alarm_Temperature)
		{
			//�¶ȸ����趨ֵʱ����������3s����
			if(count<3000)count++;
			if(key_flag==0&&count==3000)state=1;
			else state=0;
		}
		else{state=0;key_flag=0;count=0;}
		
		if(state==0)	//״̬1ʱ������
		{
			BEEP_OFF
			TIMx->CCR2=temp*R/255;	//ÿ���޸Ķ�ʱ���ıȽϼĴ���ֵ
			TIMx->CCR3=temp*G/255;
			TIMx->CCR4=temp*B/255;
			//����ѭ��
			if(temp>=1000)flag=1;else if(temp<=0)flag=0;
			if(flag==0)temp++;else temp--;
		}
		else if(state==1)	//״̬2ʱ��˸��
		{
			if(temp++>=250){flag=(flag+1)%2;temp=0;}
			if(flag==0){TIMx->CCR2=0;TIMx->CCR3=0;TIMx->CCR4=0;}
			else{TIMx->CCR2=R*1000/255;TIMx->CCR3=G*1000/255;TIMx->CCR4=B*1000/255;}
		}
		
		TIM_ClearITPendingBit(TIMx,TIM_IT_Update);	//����жϱ�־λ
	}
}

void KEY1_IRQHandler(void)
//����1����ʱ�����жϣ��ж��Ƿ��л�״̬
{
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
	{
		if(state==1)key_flag=1;
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
	}
}

void DEBUG_USART_IRQHandler(void)
//Usart�жϷ�����
{
	if (USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{
		if(state==1)key_flag=1;
		USART_ClearITPendingBit(DEBUG_USARTx,USART_IT_RXNE);
	}
}
