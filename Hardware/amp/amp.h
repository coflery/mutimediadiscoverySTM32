#ifndef __AMP_H
#define	__AMP_H

#include "stm32f4xx_hal.h"

#define PREAMP  1
#define ADC_BUF 2

#define AMP_EN  1
#define AMP_DIS 0

#define PREAMP_GPIO_Port GPIOB
#define ADC_BUF_GPIO_Port GPIOB

#define PREAMP_GPIO_Pin   GPIO_PIN_8
#define ADC_BUF_GPIO_Pin  GPIO_PIN_9


void AMP_INIT(void);
void AMP_Switch(uint8_t amp,uint8_t state);

#endif /* __AMP_H */
