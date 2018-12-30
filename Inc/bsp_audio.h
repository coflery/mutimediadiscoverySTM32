#ifndef __BSP_AUDIO_H
#define	__BSP_AUDIO_H

#include "stm32f4xx_hal.h"
#include "usbd_audio_if.h"
#include "sys.h"
#include "i2s_dac.h"
#include "led.h"

#define AUDIO_OK     0
#define AUDIO_ERROR  1

/* MUTE commands */
#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0

#define DMA_MAX_SZE                         0xFFFF
#define DMA_MAX(x)           (((x) <= DMA_MAX_SZE)? (x):DMA_MAX_SZE)
#define AUDIODATA_SIZE              2

uint8_t BSP_AUDIO_OUT_Init(uint8_t Volume, uint32_t AudioFreq);
uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Cmd);
uint8_t BSP_AUDIO_OUT_Stop(void);
#endif /* __BSP_AUDIO_H */
