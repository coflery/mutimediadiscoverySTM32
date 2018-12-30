#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx_hal.h"

#define LED_ON  1
#define LED_OFF 0

#define AMBER   0
#define RED     1
#define GREEN   2
#define BLUE    3

#define LED_GPIO_Port   GPIOE
#define AMBER_GPIO_Pin  GPIO_PIN_5
#define RED_GPIO_Pin    GPIO_PIN_4
#define GREEN_GPIO_Pin  GPIO_PIN_3
#define BLUE_GPIO_Pin   GPIO_PIN_2


void LED_INIT(void);
void LED_Switch(uint8_t led,uint8_t state);
void LED_Toggle(uint8_t led);

#endif /* __LED_H */
