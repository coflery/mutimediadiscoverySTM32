#include "dma.h"

DMA_HandleTypeDef hdma_spi2_tx;
DMA_HandleTypeDef hdma_i2s2_ext_rx;



void I2S_DMA_Config(I2S_HandleTypeDef *hi2s)
{ 
  static DMA_HandleTypeDef I2S_TxDMA_Handle;
  /* Enable the DMA clock */
  AUDIO_OUT_I2Sx_DMAx_CLK_ENABLE();

  if(hi2s->Instance == AUDIO_OUT_I2Sx_Channel)
  {
    /* Configure the hdma_i2s_rx handle parameters */
    I2S_TxDMA_Handle.Init.Channel             = AUDIO_OUT_I2Sx_DMAx_CHANNEL;
    I2S_TxDMA_Handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    I2S_TxDMA_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;
    I2S_TxDMA_Handle.Init.MemInc              = DMA_MINC_ENABLE;
    I2S_TxDMA_Handle.Init.PeriphDataAlignment = AUDIO_OUT_I2Sx_DMAx_PERIPH_DATA_SIZE;
    I2S_TxDMA_Handle.Init.MemDataAlignment    = AUDIO_OUT_I2Sx_DMAx_MEM_DATA_SIZE;
    I2S_TxDMA_Handle.Init.Mode                = DMA_NORMAL;
    I2S_TxDMA_Handle.Init.Priority            = DMA_PRIORITY_HIGH;
    I2S_TxDMA_Handle.Init.FIFOMode            = DMA_FIFOMODE_ENABLE;
    I2S_TxDMA_Handle.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
    I2S_TxDMA_Handle.Init.MemBurst            = DMA_MBURST_SINGLE;
    I2S_TxDMA_Handle.Init.PeriphBurst         = DMA_MBURST_SINGLE;

    I2S_TxDMA_Handle.Instance = AUDIO_OUT_I2Sx_DMAx_STREAM;

    /* Associate the DMA handle */
    __HAL_LINKDMA(hi2s, hdmatx, I2S_TxDMA_Handle);

    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&I2S_TxDMA_Handle);

    /* Configure the DMA Stream */
		if(HAL_DMA_Init(&I2S_TxDMA_Handle) != HAL_OK)
		{
			/* Initialization Error */
			Error_Handler();
		}
  }
  /* I2S DMA IRQ Channel configuration */
  HAL_NVIC_SetPriority(AUDIO_OUT_I2Sx_DMAx_IRQ, 4, 0);
  HAL_NVIC_EnableIRQ(AUDIO_OUT_I2Sx_DMAx_IRQ);
}
void AUDIO_OUT_I2Sx_DMAx_IRQHandler(void)
{
	I2S_DealWithDMA();
}


void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
}
