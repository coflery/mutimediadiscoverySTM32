#ifndef __ADC_H__
#define __ADC_H__
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "usart.h"
#define FS_NUM       1

void storgeDMAWave(uint16_t *lable);
void MX_ADC_Init(void);
#endif
