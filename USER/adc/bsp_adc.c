#include "bsp_adc.h"

__IO uint16_t ADC_ConvertedValue[NOFCHANEL];
__IO float ADC_ConvertedValueLocal[NOFCHANEL];

void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK,ENABLE);

	GPIO_InitStructure.GPIO_Pin=ADC_PIN1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(ADC_PORT,&GPIO_InitStructure);
}

void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	ADC_APBxClock_FUN(ADC_CLK,ENABLE);
	
	DMA_DeInit(ADC_DMA_CHANNEL);
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(ADCx->DR));
	DMA_InitStructure.DMA_MemoryBaseAddr=(u32)ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize=NOFCHANEL;
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;
	DMA_Init(ADC_DMA_CHANNEL,&DMA_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL,ENABLE);
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE ;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=NOFCHANEL;
	ADC_Init(ADCx,&ADC_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_RegularChannelConfig(ADCx,ADC_CHANNEL1,1,ADC_SampleTime_55Cycles5);
	
	ADC_DMACmd(ADCx,ENABLE);
	ADC_Cmd(ADCx,ENABLE);
	
	ADC_TempSensorVrefintCmd(ENABLE);
	
	ADC_ResetCalibration(ADCx);
	while (ADC_GetResetCalibrationStatus(ADCx));
	ADC_StartCalibration(ADCx);
	while (ADC_GetCalibrationStatus(ADCx));
	
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);
}

void ADC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
	//ADC_NVIC_Config();
}

void ADC_IRQHandler(void)
{
	if(ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET){}
	ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
}
