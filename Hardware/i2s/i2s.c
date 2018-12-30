#include "i2s.h"


extern uint16_t *CurrentPos;

I2S_HandleTypeDef		I2S_Handle;

void I2S_OUT_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
	
	/* 打开I2S音频时钟(CLK) */
  AUDIO_OUT_I2Sx_CLK_ENABLE();
	
  /* Enable MCK, SCK, WS, SD and CODEC_INT GPIO clock */
  AUDIO_OUT_I2Sx_MCK_GPIO_CLK_ENABLE();
  AUDIO_OUT_I2Sx_SCK_GPIO_CLK_ENABLE();
  AUDIO_OUT_I2Sx_SD_GPIO_CLK_ENABLE();
  AUDIO_OUT_I2Sx_WS_GPIO_CLK_ENABLE();

  /* CODEC_I2S pins configuration: MCK, SCK, WS and SD pins */
  GPIO_InitStruct.Pin = AUDIO_OUT_I2Sx_MCK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = AUDIO_OUT_I2Sx_MCK_AF;
  HAL_GPIO_Init(AUDIO_OUT_I2Sx_MCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AUDIO_OUT_I2Sx_SCK_PIN;
  GPIO_InitStruct.Alternate = AUDIO_OUT_I2Sx_SCK_AF;
  HAL_GPIO_Init(AUDIO_OUT_I2Sx_SCK_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AUDIO_OUT_I2Sx_WS_PIN;
  GPIO_InitStruct.Alternate = AUDIO_OUT_I2Sx_WS_AF;
  HAL_GPIO_Init(AUDIO_OUT_I2Sx_WS_GPIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AUDIO_OUT_I2Sx_SD_PIN;
  GPIO_InitStruct.Alternate = AUDIO_OUT_I2Sx_SD_AF;
  HAL_GPIO_Init(AUDIO_OUT_I2Sx_SD_GPIO_PORT, &GPIO_InitStruct);
	
  /* Enable and set I2Sx Interrupt to a lower priority */
  HAL_NVIC_SetPriority(SPI2_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(SPI2_IRQn);
	
}

void I2S_OUT_Clock_Config(void)
{
	RCC_PeriphCLKInitTypeDef I2S_MainClock_MuxStruct;
	HAL_RCCEx_GetPeriphCLKConfig(&I2S_MainClock_MuxStruct);
	
	/* PLLI2SCLK(MCLK) = 8M /8 *135 /6 = 22.5M */
	I2S_MainClock_MuxStruct.PeriphClockSelection = RCC_PERIPHCLK_PLLI2S;
	I2S_MainClock_MuxStruct.PLLI2S.PLLI2SN = 135;
	I2S_MainClock_MuxStruct.PLLI2S.PLLI2SR = 6;
	/*  打开I2S主时钟(MCLK) */
	HAL_RCCEx_PeriphCLKConfig(&I2S_MainClock_MuxStruct);
}

void I2S_OUT_I2S_Config(uint32_t AudioFreq)
{
  /* Initialize the hAudioInI2s Instance parameter */
  I2S_Handle.Instance = AUDIO_OUT_I2Sx;
	/* Disable I2S block */
  __HAL_I2S_DISABLE(&I2S_Handle);
  
  /* I2S peripheral configuration */
  I2S_Handle.Init.Mode           = I2S_MODE_MASTER_TX;
  I2S_Handle.Init.Standard       = I2S_STANDARD_PHILIPS;
  I2S_Handle.Init.DataFormat     = I2S_DATAFORMAT_16B;
  I2S_Handle.Init.AudioFreq      = I2S_AUDIOFREQ_48K;
	I2S_Handle.Init.CPOL           = I2S_CPOL_LOW;
  I2S_Handle.Init.ClockSource    = I2S_CLOCK_EXTERNAL;
  I2S_Handle.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
	
#ifdef CODEC_MCLK_ENABLED
  I2S_Handle.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
#elif defined(CODEC_MCLK_DISABLED)
  I2S_Handle.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
#else
  #error "No selection for the MCLK output has been defined !"
#endif

  /* Init the I2S */
	if ( HAL_I2S_Init(&I2S_Handle) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
  /* Enable I2S block */
  __HAL_I2S_ENABLE(&I2S_Handle);
}

void I2S_Configuration(uint32_t AudioFreq)
{
	I2S_OUT_GPIO_Config();
	I2S_OUT_Clock_Config();
	I2S_OUT_I2S_Config(AudioFreq);
	if(HAL_I2S_GetState(&I2S_Handle) == HAL_I2S_STATE_READY)
  {
		I2S_DMA_Config(&I2S_Handle);
  }
}
void Audio_MAL_Play(uint16_t * Addr, uint16_t Size)
{
	HAL_I2S_Transmit_DMA(&I2S_Handle, Addr, DMA_MAX(Size / AUDIODATA_SIZE));
}
void I2S_DealWithDMA(void)
{
	HAL_DMA_IRQHandler(I2S_Handle.hdmatx);
}
