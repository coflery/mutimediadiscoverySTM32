#ifndef __DMA_H__
#define __DMA_H__
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "i2s.h"


/* I2S DMA Stream Tx definitions */
#define AUDIO_OUT_I2Sx_DMAx_CLK_ENABLE()         __HAL_RCC_DMA1_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_DMAx_CLK_DISABLE()        __HAL_RCC_DMA1_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_DMAx_STREAM               DMA1_Stream5
#define AUDIO_OUT_I2Sx_DMAx_CHANNEL              DMA_CHANNEL_0
#define AUDIO_OUT_I2Sx_DMAx_IRQ                  DMA1_Stream5_IRQn
#define AUDIO_OUT_I2Sx_DMAx_PERIPH_DATA_SIZE     DMA_PDATAALIGN_HALFWORD
#define AUDIO_OUT_I2Sx_DMAx_MEM_DATA_SIZE        DMA_MDATAALIGN_HALFWORD
#define AUDIO_OUT_I2Sx_DMAx_IRQHandler           DMA1_Stream5_IRQHandler
#define AUDIO_OUT_I2Sx_Channel                   SPI3

#define AUDIO_MAL_MODE_NORMAL

#define DMA_MAX_SIZE                             0xFFFFU
#define DMA_MAX(x)                               ((x) < DMA_MAX_SIZE ? (x):DMA_MAX_SIZE)

void I2S_DMA_Config(I2S_HandleTypeDef *hi2s);

void MX_DMA_Init(void);
#endif
