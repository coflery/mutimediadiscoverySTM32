#include "stm32f4xx_hal.h"
#include "spi.h"
 
SPI_HandleTypeDef Spi1Handle;
SPI_HandleTypeDef Spi2Handle;
//SPI��ʼ��,���ó�����ģʽ������SD Card/W25X16/24L01/JF24C							  
void SPI1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();
	
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,GPIO_PIN_SET);
	
	/* ��ΪPA4��I2S���ˣ����������ԭ��SPI1��PA4 ��� ����ΪPC4����SD��Ƭѡ */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	

  Spi1Handle.Instance               = SPI1;
  Spi1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
  Spi1Handle.Init.Direction         = SPI_DIRECTION_2LINES;     //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
  Spi1Handle.Init.CLKPhase          = SPI_PHASE_2EDGE;          //���ݲ����ڵڶ���ʱ����
  Spi1Handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;        //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
  Spi1Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  Spi1Handle.Init.CRCPolynomial     = 7;                //CRCֵ����Ķ���ʽ
  Spi1Handle.Init.DataSize          = SPI_DATASIZE_8BIT;//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
  Spi1Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB; //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
  Spi1Handle.Init.NSS               = SPI_NSS_SOFT;     //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
  Spi1Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
  Spi1Handle.Init.Mode              = SPI_MODE_MASTER;  //����SPI����ģʽ:����Ϊ��SPI

  if(HAL_SPI_Init(&Spi1Handle) != HAL_OK)//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
  {
    Error_Handler();		//��ʼ������
  }
}

void SPI2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_SPI2_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_3;//2��SPI��˫��ģʽ�µ�MOSI��,������OLED��SDIN
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//�����������
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_10;//2��SPI��˫��ģʽ�µ�SCKʱ����,������OLED��SCKL
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//�����������
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_2;//2��SPI��˫��ģʽ�µ�MISO�߿�����Ϊ��ͨIO��ʹ��,������OLED��D/C#
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_1;//������OLED��RST#��λ��
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_12;//2��SPI��˫��ģʽ�µ�NSS�����ģʽ����Ϊ��ͨIOʹ��,������OLED��CS#
	#ifdef SOFT_SPI
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	//�������
	#else
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//�����������
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	#endif
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

  Spi2Handle.Instance               = SPI2;
  Spi2Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ8,apb1=20M,oled�������4M��ѡ��˷�Ƶ
  Spi2Handle.Init.Direction         = SPI_DIRECTION_1LINE;     //����SPI�������˫�������ģʽ:SPI����Ϊ˫�ߵ���ģʽ
  Spi2Handle.Init.CLKPhase          = SPI_PHASE_2EDGE;          //���ݲ����ڵڶ���ʱ����,����ʱ�ӿ���Ϊ��,���ǵڶ�ʱ������Ϊ������
  Spi2Handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;        //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
  Spi2Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  Spi2Handle.Init.CRCPolynomial     = 7;                //CRCֵ����Ķ���ʽ
  Spi2Handle.Init.DataSize          = SPI_DATASIZE_8BIT;//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
  Spi2Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB; //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	
	#ifdef SOFT_SPI
		Spi2Handle.Init.NSS               = SPI_NSS_SOFT; //NSSƬѡ���������(SSM�Ĵ���=1)
	#else
		Spi2Handle.Init.NSS               = SPI_NSS_HARD_OUTPUT; //NSSƬѡ��Ӳ������(SSM�Ĵ���=0)
	#endif

  Spi2Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
  Spi2Handle.Init.Mode              = SPI_MODE_MASTER;  //����SPI����ģʽ:����Ϊ��SPI

  if(HAL_SPI_Init(&Spi2Handle) != HAL_OK)//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
  {
    Error_Handler();		//��ʼ������
  }
}
  
void SPI1_SetSpeed(uint8_t SpeedSet)
{
  Spi1Handle.Instance               = SPI1;
  Spi1Handle.Init.BaudRatePrescaler = SpeedSet;

  if(HAL_SPI_Init(&Spi1Handle) != HAL_OK)
  {
    Error_Handler();
  }
}
void SPI2_SetSpeed(uint8_t SpeedSet)
{
  Spi2Handle.Instance               = SPI2;
  Spi2Handle.Init.BaudRatePrescaler = SpeedSet;

  if(HAL_SPI_Init(&Spi2Handle) != HAL_OK)
  {
    Error_Handler();
  }
}

uint8_t SPI1_ReadWriteByte(uint8_t TxByte)
{
	uint8_t RxByte;
	
	if(HAL_SPI_TransmitReceive(&Spi1Handle,&TxByte,&RxByte,1,100)!=HAL_OK)
	{
		return DUMMY_Byte;
	}
	return RxByte;
}
uint8_t SPI2_ReadWriteByte(uint8_t TxByte)
{
	uint8_t RxByte;
	
	if(HAL_SPI_TransmitReceive(&Spi2Handle,&TxByte,&RxByte,1,500)!=HAL_OK)
	{
		return DUMMY_Byte;
	}
	return RxByte;
}
uint8_t SPI1_WriteByte(uint8_t * TxByte,uint16_t size)
{
	if(HAL_SPI_Transmit(&Spi1Handle,TxByte,size,10)!=HAL_OK)
	{
		return 1;
	}
	return 0;
}
uint8_t SPI2_WriteByte(uint8_t * TxByte,uint16_t size)
{
	if(HAL_SPI_Transmit(&Spi2Handle,TxByte,size,10)!=HAL_OK)
	{
		return 1;
	}
	return 0;
}
