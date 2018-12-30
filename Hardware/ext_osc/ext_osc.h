#ifndef __EXT_OSC_H
#define	__EXT_OSC_H

#include "stm32f4xx_hal.h"

#define EXT_OSC_225792 0
#define EXT_OSC_24576  1

#define EXT_OSC_ON  1
#define EXT_OSC_OFF 0

#define EXT_OSC_EN_Port  GPIOB

#define EXT_OSC_225792_EN_Pin   GPIO_PIN_14
#define EXT_OSC_24576_EN_Pin    GPIO_PIN_15

void EXT_OSC_INIT(void);
void EXT_OSC_Switch(uint8_t osc,uint8_t state);

#endif /* __EXT_OSC_H */
