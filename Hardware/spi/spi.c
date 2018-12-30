#include "stm32f4xx_hal.h"
#include "spi.h"
 
SPI_HandleTypeDef Spi1Handle;
SPI_HandleTypeDef Spi2Handle;
//SPI初始化,配置成主机模式，访问SD Card/W25X16/24L01/JF24C							  
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
	
	/* 因为PA4给I2S用了，所以这个将原本SPI1的PA4 软件 控制为PC4用于SD卡片选 */
	GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	

  Spi1Handle.Instance               = SPI1;
  Spi1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;//定义波特率预分频的值:波特率预分频值为256
  Spi1Handle.Init.Direction         = SPI_DIRECTION_2LINES;     //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
  Spi1Handle.Init.CLKPhase          = SPI_PHASE_2EDGE;          //数据捕获于第二个时钟沿
  Spi1Handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;        //选择了串行时钟的稳态:时钟悬空高
  Spi1Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  Spi1Handle.Init.CRCPolynomial     = 7;                //CRC值计算的多项式
  Spi1Handle.Init.DataSize          = SPI_DATASIZE_8BIT;//设置SPI的数据大小:SPI发送接收8位帧结构
  Spi1Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB; //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
  Spi1Handle.Init.NSS               = SPI_NSS_SOFT;     //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
  Spi1Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
  Spi1Handle.Init.Mode              = SPI_MODE_MASTER;  //设置SPI工作模式:设置为主SPI

  if(HAL_SPI_Init(&Spi1Handle) != HAL_OK)//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
  {
    Error_Handler();		//初始化错误
  }
}

void SPI2_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_SPI2_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_3;//2线SPI半双工模式下的MOSI线,这里是OLED的SDIN
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//复用推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_10;//2线SPI半双工模式下的SCK时钟线,这里是OLED的SCKL
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//复用推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_2;//2线SPI半双工模式下的MISO线可以作为普通IO口使用,这里是OLED的D/C#
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_1;//这里是OLED的RST#复位线
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_12;//2线SPI半双工模式下的NSS线软件模式下作为普通IO使用,这里是OLED的CS#
	#ifdef SOFT_SPI
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	//推挽输出
	#else
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//复用推挽输出
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	#endif
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

  Spi2Handle.Instance               = SPI2;
  Spi2Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;//定义波特率预分频的值:波特率预分频值为8,apb1=20M,oled允许最高4M，选择八分频
  Spi2Handle.Init.Direction         = SPI_DIRECTION_1LINE;     //设置SPI单向或者双向的数据模式:SPI设置为双线单工模式
  Spi2Handle.Init.CLKPhase          = SPI_PHASE_2EDGE;          //数据捕获于第二个时钟沿,这里时钟空闲为高,于是第二时钟沿则为上升沿
  Spi2Handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;        //选择了串行时钟的稳态:时钟悬空高
  Spi2Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  Spi2Handle.Init.CRCPolynomial     = 7;                //CRC值计算的多项式
  Spi2Handle.Init.DataSize          = SPI_DATASIZE_8BIT;//设置SPI的数据大小:SPI发送接收8位帧结构
  Spi2Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB; //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	
	#ifdef SOFT_SPI
		Spi2Handle.Init.NSS               = SPI_NSS_SOFT; //NSS片选由软件控制(SSM寄存器=1)
	#else
		Spi2Handle.Init.NSS               = SPI_NSS_HARD_OUTPUT; //NSS片选由硬件控制(SSM寄存器=0)
	#endif

  Spi2Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
  Spi2Handle.Init.Mode              = SPI_MODE_MASTER;  //设置SPI工作模式:设置为主SPI

  if(HAL_SPI_Init(&Spi2Handle) != HAL_OK)//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
  {
    Error_Handler();		//初始化错误
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
