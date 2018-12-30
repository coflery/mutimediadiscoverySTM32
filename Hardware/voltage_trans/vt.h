#ifndef __VT_H
#define	__VT_H

#include "stm32f4xx_hal.h"

#define VT_1 1
#define VT_2 2

#define VT_EN  1
#define VT_DIS 0

#define VT1_GPIO_Port GPIOC
#define VT2_GPIO_Port GPIOA

#define VT1_GPIO_Pin  GPIO_PIN_10
#define VT2_GPIO_Pin  GPIO_PIN_15


void Voltage_Translation_INIT(void);
void VT_Switch(uint8_t port,uint8_t state);

#endif /* __VT_H */
