#include "adc.h"

void ADC_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	/* RCC参数配置 */
  __HAL_RCC_GPIOA_CLK_ENABLE();
	
  /* GPIO参数配置 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/** 
 ** Enable DMA controller clock
 **/

static DMA_HandleTypeDef hdma_adcx;
void ADC_DMA_Init(ADC_HandleTypeDef* hadc) 
{
	
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
	
  /* DMA外设初始化配置 */  
  hdma_adcx.Instance = DMA2_Stream0;
  hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adcx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adcx.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adcx.Init.Mode = DMA_CIRCULAR;
  hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&hdma_adcx);
  /* 连接DMA */
  __HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
	
  /* DMA2_Stream0 外设中断优先级配置和使能中断 */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 2, 1);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

//ADC值位置
__IO uint16_t uhADCxConvertedValue[FS_NUM];
__IO uint16_t sysADCStorge[FS_NUM];
static ADC_HandleTypeDef AdcHandle;
void MX_ADC_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;
	
	ADC_GPIO_Init();
	
	__HAL_RCC_ADC1_CLK_ENABLE();

	//HAL_PWR_DisableWakeUpPin(PWR_CSR_EWUP);                     //禁止wakeup PA0引脚，可以用来做普通引脚功能  
	
	AdcHandle.Instance = ADC1;
	AdcHandle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
	AdcHandle.Init.Resolution = ADC_RESOLUTION_12B;
	AdcHandle.Init.ScanConvMode = DISABLE;  //不使用多通道模式
	AdcHandle.Init.ContinuousConvMode = ENABLE; 
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	//AdcHandle.Init.NbrOfDiscConversion = 0;
	AdcHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	AdcHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;//右对齐
	AdcHandle.Init.NbrOfConversion = 1;
	AdcHandle.Init.DMAContinuousRequests = ENABLE;/*DMA转换请求，指定DMA执行单次（当转换数达标时停止传送）or连续请求模式；
																									备注：连续模式时候DMA必须配置为循环模式，否则当DMA buffer存满时将触发溢出； ENABLE or DISABLE */
	AdcHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

	if(HAL_ADC_Init(&AdcHandle) != HAL_OK)
	{
	/* Initialization Error */
		Error_Handler();
	}
	
	/*##-2- Configure ADC 采样 channel ######################################*/ 
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	
	/* F4的ADC最大频率36M，30M稳定	*/
	/*ADC FSclock = PCLK2/ADC_CLOCK_SYNC_PCLK_DIV8/(ADC_SAMPLETIME_480CYCLES+15采样位的影响时间)=5.05k */

	if(HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
	{
		/* Channel Configuration Error */
		Error_Handler();
	}
	
	ADC_DMA_Init(&AdcHandle);
	
	/*##-3- Start the conversion process and enable interrupt ##################*/  
  if(HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&uhADCxConvertedValue, FS_NUM) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler(); 
  }
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
//	uint8_t data[2];
//	
//  HAL_ADC_Stop_DMA(&AdcHandle);

//	/* Turn LED3 on: Transfer process is correct */
//	for(uint16_t i=0;i<FS_NUM;i++)
//	{
//		uint16_t temp;
//		temp=uhADCxConvertedValue[i];
//		Transfor_u16( temp,data);
//		send_date(0XA1,data,2);
//		HAL_Delay(1);
//	}
//	for(uint16_t i=0;i<FS_NUM;i++)
//	{
//		sysADCStorge[i]=uhADCxConvertedValue[i];
//	}
//	HAL_ADC_Start_DMA(&AdcHandle, (uint32_t*)&uhADCxConvertedValue, FS_NUM);
}

void storgeDMAWave(uint16_t *lable)
{
	for(uint16_t i=0;i<FS_NUM;i++)
	{
		lable[i]=sysADCStorge[i];
	}
}
void DMA2_Stream0_IRQHandler(void)
{	
	HAL_DMA_IRQHandler(&hdma_adcx);
}
