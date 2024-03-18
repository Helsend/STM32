#include "main.h"

float Alarm_Temperature=30.0;	//温度阈值
uint16_t R=128,G=255,B=255;	//LED灯颜色RGB控制
uint16_t tone[]={262,294,330,349,392,440,494,523,587,659,698,784,880,988,1046,1175,1318,1397,1568,1760,1976,0};
uint8_t Music[]={11,14,13,15,14,11,9,12,10,12,15,14,13,12,11,10,9,11,14,13,15,14,11,9,12,10,12,15,14,13,15,17,13,14};	//音乐
uint8_t Time[]={2,6,2,2,2,2,2,8,6,2,6,2,2,2,2,2,8,2,6,2,2,2,2,2,8,2,2,2,2,4,4,4,4,6};	//音节持续时间
uint8_t music_len=34;	//音乐音节数

uint8_t sample_interval_count=0;	//温度采样间隔计数器，0.1s采样一次
uint8_t state=0;			//状态参数，0为正常工作，1为报警
uint8_t key_flag=0;		//上位机或板子上按键停止报警标志
uint16_t count=0;			//计数器，温度持续高于设定温度3s以上才报警，消除在阈值附近时的不正常工作
uint8_t select=0;//音乐指针参数
uint16_t space_count=0;	//音乐【空】时的计数器


int main(void)
{
	//初始化
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
//基本定时器中断函数
{
	static uint32_t i=0;
	static uint32_t j=0;
	if(TIM_GetITStatus(BASIC_TIM,TIM_IT_Update)!= RESET)
	{
		//有源蜂鸣器播放音乐
		if(state==1&&tone[Music[select]])	//非休止符
		{
			if(i!=(Time[select]*tone[Music[select]]/16))	//总周期数
			{
				//蜂鸣器pwm
				if(j++<50000/tone[Music[select]])BEEP_ON
				else if(j<500000/tone[Music[select]])BEEP_OFF
				else{i++;j=0;}	//下一个周期
			}
			else{i=0;j=0;select=(select+1)%music_len;}	//下一个音节
		}
		else if(state==1&&tone[Music[select]]==0)	//休止符
		{
			BEEP_OFF
			if(space_count++>=1000000/16){space_count=0;select=(select+1)%14;}
		}
		TIM_ClearITPendingBit(BASIC_TIM,TIM_FLAG_Update);
	}
}

void TIMx_IRQHandler(void)
//通用定时器每1ms产生一次中断
{
	if(TIM_GetITStatus(TIMx,TIM_IT_Update)!=RESET)
	{
		static uint16_t flag=0;
		static uint16_t temp=1;
		
		if(sample_interval_count++==100)//0.1s采样一次
		{
			//ADC温度转换计算
			ADC_ConvertedValueLocal[0]=(1.43-ADC_ConvertedValue[0]*3.3/4096)*1000/4.35+25;
			printf("%f\n",ADC_ConvertedValueLocal[0]);
			sample_interval_count=0;
		}
		
		//状态转换的判断
		if(ADC_ConvertedValueLocal[0]>=Alarm_Temperature)
		{
			//温度高于设定值时记数，大于3s报警
			if(count<3000)count++;
			if(key_flag==0&&count==3000)state=1;
			else state=0;
		}
		else{state=0;key_flag=0;count=0;}
		
		if(state==0)	//状态1时呼吸灯
		{
			BEEP_OFF
			TIMx->CCR2=temp*R/255;	//每次修改定时器的比较寄存器值
			TIMx->CCR3=temp*G/255;
			TIMx->CCR4=temp*B/255;
			//亮灭循环
			if(temp>=1000)flag=1;else if(temp<=0)flag=0;
			if(flag==0)temp++;else temp--;
		}
		else if(state==1)	//状态2时闪烁灯
		{
			if(temp++>=250){flag=(flag+1)%2;temp=0;}
			if(flag==0){TIMx->CCR2=0;TIMx->CCR3=0;TIMx->CCR4=0;}
			else{TIMx->CCR2=R*1000/255;TIMx->CCR3=G*1000/255;TIMx->CCR4=B*1000/255;}
		}
		
		TIM_ClearITPendingBit(TIMx,TIM_IT_Update);	//清除中断标志位
	}
}

void KEY1_IRQHandler(void)
//按键1按下时产生中断，判断是否切换状态
{
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
	{
		if(state==1)key_flag=1;
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
	}
}

void DEBUG_USART_IRQHandler(void)
//Usart中断服务函数
{
	if (USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{
		if(state==1)key_flag=1;
		USART_ClearITPendingBit(DEBUG_USARTx,USART_IT_RXNE);
	}
}
