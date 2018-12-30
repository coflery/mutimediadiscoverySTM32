#ifndef __UDA1380_H__
#define __UDA1380_H__

#include "stm32f4xx_hal.h"
#include "i2c.h" 
#include "i2s.h"
#include "usart.h"
#include "dma.h"


#define UDA1380_READ_ADDRESS      0x31
#define UDA1380_WRITE_ADDRESS     0x30

/* ÏµÍ³ÉèÖÃ¼Ä´æÆ÷ */
#define UDA1380_REG_EVALCLK	      0x00 //evaluation modes, WSPLL settings, clock divider and clock selectors
#define UDA1380_REG_I2S	          0x01 //I2S-bus I/O settings
#define UDA1380_REG_PWRCTRL	      0x02 //power control settings
#define UDA1380_REG_ANAMIX	      0x03 //analog mixer settings
/* Æ¬ÄÚÂË²¨Æ÷ */
#define UDA1380_REG_HEADAMP	      0x04 //reserved
#define UDA1380_REG_MSTRVOL	      0x10 //master volume control
#define UDA1380_REG_MIXVOL	      0x11 //mixer volume control
#define UDA1380_REG_MODEBBT	      0x12 //mode selection, left and right bass boost, and treble settings
#define UDA1380_REG_MSTRMUTE      0x13 //master mute, channel 1 and channel 2 de-emphasis and channel mute
#define UDA1380_REG_MIXSDO	      0x14 //mixer, silence detector and interpolation filter oversampling settings
/* ²ÉÑùÂË²¨Æ÷ÅäÖÃ¼Ä´æÆ÷ */
#define UDA1380_REG_DECVOL	      0x20 //decimator volume control
#define UDA1380_REG_PGA	          0x21 //PGA settings and mute
#define UDA1380_REG_ADC	          0x22 //ADC settings
#define UDA1380_REG_AGC	          0x23 //AGC settings
#define UDA1380_REG_DEC	          0x28 //decimator status
/* Èí¸´Î» */
#define UDA1380_REG_L3	          0x7f //ÖØÖÃ L3-Ä¬ÈÏÖµ

#define UDA1380_REG_HEADPHONE     0x18 //interpolation filter status

uint8_t  UDA1380_Configuration(void);
void UDA1380_SndNchar(const unsigned char * datbuf, uint32_t* datbufpt_pt, uint32_t snd_N);
uint32_t AUDIO_Play(uint16_t* pBuffer, uint32_t Size);
uint32_t AudioFlashPlay(uint16_t* pBuffer, uint32_t FullSize, uint32_t StartAdd);

#endif  // __UDA1380_H__
