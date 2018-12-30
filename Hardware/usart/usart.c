//////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : usart.c
//  版 本 号   : V2.1
//  作    者   : TinderSmith
//  生成日期   : 2014-01-01
//  最近修改   : 2018-03-28
//  功能描述   : 配合microlib与STM32HAL库可以用串口完成标准输入输出(printf与scanf等)
//  版权所有   : 任复衡
//  依赖关系   : GPIO,NVIC,RCC,UART/USART,Stdint
//	说    明   : -----------------------------------------------------------------
//  2014-01-01   V1.0 第一版
//  2014-08-17   V1.1 按照原子哥的方法和ucos
//  2015-06-12   V1.2 修改支持gcc
//  2017-06-24   V2.0 A.加入stdlib的单字节收发功能
//                    B.通过开关中断方式实现中断与标准库的兼容
//                    C.重写接口函数实现头文件的标准化接口
//                    D.bug:修复Putchar中TC寄存器为TXE实现高暴发发送数据
//  2017-06-26   V2.1 实测HCLK=FCLK=40MHz下,每次1ms延迟,每次100字节,10000字节丢帧率0
//  2018-03-28   V2.2 调试用于多媒体音频开发板的串口2
//               -----------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////
#include "usart.h"

uint16_t USART_RX_STA=0;									//接收状态标记
uint8_t  USART_RX_BUF[USART_RxLen_MAX];		//接收缓冲,最大USART_REC_LEN字节,末字节为换行符
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
	/* RCC参数配置 */
	USARTx_RX_GPIO_CLK_ENABLE();
	USARTx_TX_GPIO_CLK_ENABLE();
	USARTx_CLK_ENABLE();
	
  /* GPIO参数配置 */
  GPIO_InitStruct.Pin = USARTx_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = USARTx_TX_AF;
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = USARTx_RX_PIN;
	GPIO_InitStruct.Alternate = USARTx_RX_AF;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
  /* UART参数配置 */
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
  /* 配置内核的UART的中断嵌套向量表,要放在HAL_UART_Init函数后才有效 */
  NVIC_UARTx_Init();
	
	/* 寄存器操作:UART发中断,必须使用中断方式发送数据时再完善其功能 */
	//SET_BIT(UartHandle.Instance->CR1, USART_CR1_TXEIE);
	/* 寄存器操作:UART收中断 */
	SET_BIT(UartHandle.Instance->CR1, USART_CR1_RXNEIE);
}

PUTCHAR_PROTOTYPE
{
	while( READ_BIT (UartHandle.Instance->SR, USART_SR_TXE) == 0){}//等待发送缓冲区(单字节模式)为空
		
		UartHandle.Instance->DR = (uint8_t)ch;//发送一个字节
		
	return ch;//回传发送的字节,这是stdio库的要求
}
GETCHAR_PROTOTYPE
{
	uint8_t RxData;
	
	CLEAR_BIT(UartHandle.Instance->CR1, USART_CR1_RXNEIE);//取消中断方式,用软件扫描读取数据
	
	while( READ_BIT (UartHandle.Instance->SR, USART_SR_RXNE) == 0){}//等待数据进入缓冲区
		RxData = (UartHandle.Instance->DR & 0xFF);//读取在DR寄存器内收到的一个字节
		
	SET_BIT(UartHandle.Instance->CR1, USART_CR1_RXNEIE);//恢复中断
	
	return RxData;
}

void USARTx_IRQHandler(void)
{
	uint8_t RxData;

#ifdef OS_TICKS_PER_SEC	 	//定义了时钟节拍数即证明使用了ucosII
	OSIntEnter();
#endif

	if(READ_BIT(UartHandle.Instance->SR, USART_SR_RXNE))//收到一个字节(收到数据要以回车换行\r\n结尾)
		{
			RxData=(UartHandle.Instance->DR & 0xFF);//读取在DR寄存器内当前收到的数据
			if((USART_RX_STA&0x8000)==0)//接收未完成
				{
					if(USART_RX_STA&0x4000)//收到回车符
						{
							if(RxData!='\n')USART_RX_STA=0;//接收错误,重新开始
							else USART_RX_STA|=0x8000;	//接收完成了
						}
					else //没收到回车符
						{
							if(RxData=='\r')USART_RX_STA|=0x4000;
							else
								{
									USART_RX_BUF[USART_RX_STA&0x3FFF]=RxData;
									USART_RX_STA++;
									if(USART_RX_STA>(USART_RxLen_MAX-1))
										USART_RX_STA=0;//数据太长而未收到结束标志,全部忽略并重新接收
								}
						}
				}
     }

#ifdef OS_TICKS_PER_SEC	 	//定义了时钟节拍数即证明使用了ucosII
	OSIntExit();
#endif
}

/* 匿名四轴的波形发送函数 */
void send2PC(uint8_t fun,uint8_t *data,uint8_t len)
{
	FILE *f;
	uint8_t send_buf[32];
	uint8_t i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)fputc(send_buf[i],f);	//发送数据到串口1
	HAL_Delay(1);//保证上位机能正常解码
}
/*将u16转换为senddata能够发送的大端模式*/
void sendWave2PC_u16(uint16_t num)
{
	uint8_t data[2];
	data[0]=(num>>8)&0XFF;
	data[1]=num&0XFF;
	
	send2PC(0XA1,data,2);
}
/*将float转换为senddata能够发送的大端模式*/
void sendWave2PC_float(float num)
{
	uint8_t i = 0;
	
	uint8_t data[4];
	uint8_t *p=(uint8_t *)&num;
	for(i = 0;i<4;i++)
		data[3-i]=*p++;
	send2PC(0Xa1,data,4);
}


