#ifndef __PHANTOMPOWER_H
#define	__PHANTOMPOWER_H

#include "stm32f4xx_hal.h"

#define PHANTOMPOWER_ON  1
#define PHANTOMPOWER_OFF 0


#define PHANTOMPOWER_PortE GPIOE
#define PHANTOMPOWER_Pin   GPIO_PIN_12

void PhantomPower_INIT(void);
void PhantomPower(uint8_t state);

#endif /* __PHANTOMPOWER_H */
