#include "bsp_usart.h"

void NVIC_Configuration(void)
//�жϿ�����NVIC����
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							//Ƕ�������жϿ�������1
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;				//UsartΪ�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//�������ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//�ж�ʹ��
	
	NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
//Usart��������
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	//�򿪴���GPIOʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	//�򿪴�������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	
	//�����ж����ȼ�����
	NVIC_Configuration();
	
	//��ʼ��Usart Tx������
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	//��ʼ��Usart Rx������
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	//���ô������ò���ʼ��
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;											//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;											//֡�����ֳ�
	USART_InitStructure.USART_StopBits = USART_StopBits_1;													//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No ;														//У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;									//�շ�һ��Ĺ���ģʽ
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	//�ò���ʱ��ע������
	//ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);
}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
//����һ���ַ�
{
	USART_SendData(pUSARTx,ch);	//����һ���ֽڵ�Usart
	//�ȴ����ͼĴ������
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
//�����ַ���
{
	unsigned int k=0;
	do {
		Usart_SendByte( pUSARTx, *(str + k) );
		k++;
	} while (*(str + k)!='\0');
	//�ȴ��������
	while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint32_t num)
//����8λ���ݵ�����
{
	unsigned int k=0;
	for(k=0;k<num;k++)
		Usart_SendByte( pUSARTx, array[k]);
	//�ȴ��������
	while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

int fputc(int ch, FILE *f)
//�ض���printf
{
	//����һ���ֽڵ�����
	USART_SendData(DEBUG_USARTx, (uint8_t) ch);
	//�ȴ��������
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);
	return (ch);
}

int fgetc(FILE *f)
//�ض���scanf
{
	//�ȴ�������������
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(DEBUG_USARTx);
}

//void DEBUG_USART_IRQHandler(void)
////Usart�жϷ�����
//{
//	uint8_t ucTemp;
//	if (USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
//	{
//		ucTemp = USART_ReceiveData( DEBUG_USARTx );
//		USART_SendData(DEBUG_USARTx,ucTemp);
//	}
//}
