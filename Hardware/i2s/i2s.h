#ifndef __I2S_H__
#define __I2S_H__
#include "stm32f4xx_hal.h"
#include "uda1380.h"
#include "file.h"


//#define CODEC_MCLK_DISABLED
#define CODEC_MCLK_ENABLED

#define AUDIODATA_SIZE                           2   /* 16-bits audio data size */

#define AUDIO_OUT_I2Sx                           SPI2
#define AUDIO_OUT_I2Sx_CLK_ENABLE()              __HAL_RCC_SPI2_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_CLK_DISABLE()             __HAL_RCC_SPI2_CLK_DISABLE()

#define AUDIO_OUT_I2Sx_MCK_PIN                   GPIO_PIN_6
#define AUDIO_OUT_I2Sx_MCK_GPIO_PORT             GPIOC
#define AUDIO_OUT_I2Sx_MCK_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_MCK_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOC_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_MCK_AF                    GPIO_AF5_SPI2

#define AUDIO_OUT_I2Sx_SCK_PIN                   GPIO_PIN_13
#define AUDIO_OUT_I2Sx_SCK_GPIO_PORT             GPIOB
#define AUDIO_OUT_I2Sx_SCK_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_SCK_GPIO_CLK_DISABLE()    __HAL_RCC_GPIOB_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_SCK_AF                    GPIO_AF5_SPI2

#define AUDIO_OUT_I2Sx_WS_PIN                    GPIO_PIN_9
#define AUDIO_OUT_I2Sx_WS_GPIO_PORT              GPIOB
#define AUDIO_OUT_I2Sx_WS_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_WS_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOB_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_WS_AF                     GPIO_AF5_SPI2

#define AUDIO_OUT_I2Sx_SD_PIN                    GPIO_PIN_3
#define AUDIO_OUT_I2Sx_SD_GPIO_PORT              GPIOC
#define AUDIO_OUT_I2Sx_SD_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_SD_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOC_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_SD_AF                     GPIO_AF5_SPI2

#define AUDIO_OUT_I2Sx_extSD_PIN                 GPIO_PIN_2
#define AUDIO_OUT_I2Sx_extSD_GPIO_PORT           GPIOC
#define AUDIO_OUT_I2Sx_extSD_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define AUDIO_OUT_I2Sx_extSD_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define AUDIO_OUT_I2Sx_extSD_AF                  GPIO_AF6_I2S2ext

void I2S_Configuration(uint32_t AudioFreq);
void Audio_MAL_Play(uint16_t * Addr, uint16_t Size);
void I2S_DealWithDMA(void);
#endif
