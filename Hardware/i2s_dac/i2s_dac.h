#ifndef __I2S_DAC_H
#define	__I2S_DAC_H

#include "stm32f4xx_hal.h"

#define DAC_POWER_ON  1
#define DAC_POWER_OFF 0

#define DAC_MUTE_DIS  1
#define DAC_MUTE_EN   0

#define DAC_DIF_I2S   0
#define DAC_DIF_LJ    1


#define DAC_GPIO_PortD GPIOD
#define DAC_GPIO_PortC GPIOC

#define DAC_LOGIC_ZD_Pin    GPIO_PIN_3
#define DAC_LOGIC_MUTE_Pin  GPIO_PIN_4
#define DAC_POWER_EN_Pin    GPIO_PIN_7
#define DAC_LOGIC_DIF_Pin   GPIO_PIN_11

void I2S_DAC_INIT(void);
void DAC_POWER(uint8_t state);
void DAC_MUTE(uint8_t state);
void DAC_DIF(uint8_t state);

#endif /* __I2S_DAC_H */
