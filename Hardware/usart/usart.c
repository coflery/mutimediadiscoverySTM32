//////////////////////////////////////////////////////////////////////////////////
//  �� �� ��   : usart.c
//  �� �� ��   : V2.1
//  ��    ��   : TinderSmith
//  ��������   : 2014-01-01
//  ����޸�   : 2018-03-28
//  ��������   : ���microlib��STM32HAL������ô�����ɱ�׼�������(printf��scanf��)
//  ��Ȩ����   : �θ���
//  ������ϵ   : GPIO,NVIC,RCC,UART/USART,Stdint
//	˵    ��   : -----------------------------------------------------------------
//  2014-01-01   V1.0 ��һ��
//  2014-08-17   V1.1 ����ԭ�Ӹ�ķ�����ucos
//  2015-06-12   V1.2 �޸�֧��gcc
//  2017-06-24   V2.0 A.����stdlib�ĵ��ֽ��շ�����
//                    B.ͨ�������жϷ�ʽʵ���ж����׼��ļ���
//                    C.��д�ӿں���ʵ��ͷ�ļ��ı�׼���ӿ�
//                    D.bug:�޸�Putchar��TC�Ĵ���ΪTXEʵ�ָ߱�����������
//  2017-06-26   V2.1 ʵ��HCLK=FCLK=40MHz��,ÿ��1ms�ӳ�,ÿ��100�ֽ�,10000�ֽڶ�֡��0
//  2018-03-28   V2.2 �������ڶ�ý����Ƶ������Ĵ���2
//               -----------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////
#include "usart.h"

uint16_t USART_RX_STA=0;									//����״̬���
uint8_t  USART_RX_BUF[USART_RxLen_MAX];		//���ջ���,���USART_REC_LEN�ֽ�,ĩ�ֽ�Ϊ���з�
UART_HandleTypeDef UartHandle;

#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(FILE *f)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

void NVIC_UARTx_Init(void)
{
  /* USARTx_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USARTx_IRQn, 1, 0);
	/* Enable the USARTx global Interrupt */ 
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

void USART_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	/* RCC�������� */
	USARTx_RX_GPIO_CLK_ENABLE();
	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_CLK_ENABLE();
	
  /* GPIO�������� */
  GPIO_InitStruct.Pin = USARTx_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = USARTx_TX_AF;
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = USARTx_RX_PIN;
	GPIO_InitStruct.Alternate = USARTx_RX_AF;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
  /* UART�������� */
  UartHandle.Instance          = USARTx;
  UartHandle.Init.BaudRate     = USARTx_BaudRate;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	
  if (HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  /* �����ں˵�UART���ж�Ƕ��������,Ҫ����HAL_UART_Init���������Ч */
  NVIC_UARTx_Init();
	
	/* �Ĵ�������:UART���ж�,����ʹ���жϷ�ʽ��������ʱ�������书�� */
	//SET_BIT(UartHandle.Instance->CR1, USART_CR1_TXEIE);
	/* �Ĵ�������:UART���ж� */
	SET_BIT(UartHandle.Instance->CR1, USART_CR1_RXNEIE);
}

PUTCHAR_PROTOTYPE
{
	while( READ_BIT (UartHandle.Instance->SR, USART_SR_TXE) == 0){}//�ȴ����ͻ�����(���ֽ�ģʽ)Ϊ��
		
		UartHandle.Instance->DR = (uint8_t)ch;//����һ���ֽ�
		
	return ch;//�ش����͵��ֽ�,����stdio���Ҫ��
}
GETCHAR_PROTOTYPE
{
	uint8_t RxData;
	
	CLEAR_BIT(UartHandle.Instance->CR1, USART_CR1_RXNEIE);//ȡ���жϷ�ʽ,�����ɨ���ȡ����
	
	while( READ_BIT (UartHandle.Instance->SR, USART_SR_RXNE) == 0){}//�ȴ����ݽ��뻺����
		RxData = (UartHandle.Instance->DR & 0xFF);//��ȡ��DR�Ĵ������յ���һ���ֽ�
		
	SET_BIT(UartHandle.Instance->CR1, USART_CR1_RXNEIE);//�ָ��ж�
	
	return RxData;
}

void USARTx_IRQHandler(void)
{
	uint8_t RxData;

#ifdef OS_TICKS_PER_SEC	 	//������ʱ�ӽ�������֤��ʹ����ucosII
	OSIntEnter();
#endif

	if(READ_BIT(UartHandle.Instance->SR, USART_SR_RXNE))//�յ�һ���ֽ�(�յ�����Ҫ�Իس�����\r\n��β)
		{
			RxData=(UartHandle.Instance->DR & 0xFF);//��ȡ��DR�Ĵ����ڵ�ǰ�յ�������
			if((USART_RX_STA&0x8000)==0)//����δ���
				{
					if(USART_RX_STA&0x4000)//�յ��س���
						{
							if(RxData!='\n')USART_RX_STA=0;//���մ���,���¿�ʼ
							else USART_RX_STA|=0x8000;	//���������
						}
					else //û�յ��س���
						{
							if(RxData=='\r')USART_RX_STA|=0x4000;
							else
								{
									USART_RX_BUF[USART_RX_STA&0x3FFF]=RxData;
									USART_RX_STA++;
									if(USART_RX_STA>(USART_RxLen_MAX-1))
										USART_RX_STA=0;//����̫����δ�յ�������־,ȫ�����Բ����½���
								}
						}
				}
     }

#ifdef OS_TICKS_PER_SEC	 	//������ʱ�ӽ�������֤��ʹ����ucosII
	OSIntExit();
#endif
}

/* ��������Ĳ��η��ͺ��� */
void send2PC(uint8_t fun,uint8_t *data,uint8_t len)
{
	FILE *f;
	uint8_t send_buf[32];
	uint8_t i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)fputc(send_buf[i],f);	//�������ݵ�����1
	HAL_Delay(1);//��֤��λ������������
}
/*��u16ת��Ϊsenddata�ܹ����͵Ĵ��ģʽ*/
void sendWave2PC_u16(uint16_t num)
{
	uint8_t data[2];
	data[0]=(num>>8)&0XFF;
	data[1]=num&0XFF;
	
	send2PC(0XA1,data,2);
}
/*��floatת��Ϊsenddata�ܹ����͵Ĵ��ģʽ*/
void sendWave2PC_float(float num)
{
	uint8_t i = 0;
	
	uint8_t data[4];
	uint8_t *p=(uint8_t *)&num;
	for(i = 0;i<4;i++)
		data[3-i]=*p++;
	send2PC(0Xa1,data,4);
}


