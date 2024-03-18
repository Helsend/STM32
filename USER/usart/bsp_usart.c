#include "bsp_usart.h"

void NVIC_Configuration(void)
//中断控制器NVIC配置
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//嵌套向量中断控制器组1
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;				//Usart为中断源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//抢断优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级为1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//中断使能
	
	NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
//Usart参数配置
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//打开串口GPIO时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	//打开串口外设时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	
	//串口中断优先级配置
	NVIC_Configuration();
	
	//初始化Usart Tx，推挽
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	//初始化Usart Rx，浮空
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	//配置串口设置并初始化
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;											//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//帧数据字长
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;														//校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//收发一起的工作模式
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	//使能串口接收中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	//用不到时可注释屏蔽
	//使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);
}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
//发送一个字符
{
	USART_SendData(pUSARTx,ch);	//发送一个字节到Usart
	//等待发送寄存器清空
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
//发送字符串
{
	unsigned int k=0;
	do {
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while (*(str + k)!='\0');
	//等待发送完成
	while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint32_t num)
//发送8位数据的数组
{
	unsigned int k=0;
	for(k=0;k<num;k++)
		Usart_SendByte( pUSARTx, array[k]);
	//等待发送完成
	while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

int fputc(int ch, FILE *f)
//重定向printf
{
	//发送一个字节到串口
	USART_SendData(DEBUG_USARTx, (uint8_t) ch);
	//等待发送完毕
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
	return (ch);
}

int fgetc(FILE *f)
//重定向scanf
{
	//等待串口输入数据
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(DEBUG_USARTx);
}

//void DEBUG_USART_IRQHandler(void)
////Usart中断服务函数
//{
//	uint8_t ucTemp;
//	if (USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
//	{
//		ucTemp = USART_ReceiveData( DEBUG_USARTx );
//		USART_SendData(DEBUG_USARTx,ucTemp);
//	}
//}
