#ifndef __I2S_ADC_H
#define	__I2S_ADC_H

#include "stm32f4xx_hal.h"

#define ADC_POWER_ON  1
#define ADC_POWER_OFF 0

#define ADC_P3V3 0
#define ADC_P5V0 1
//---------------------------------------
#define ADC_EN_P3V3Port      GPIOE
#define ADC_EN_P3V3_Pin      GPIO_PIN_7
#define ADC_EN_P5V0Port      GPIOB
#define ADC_EN_P5V0_Pin      GPIO_PIN_0
//---------------------------------------
#define ADC_Control_PortA    GPIOA
#define ADC_Control_MS       GPIO_PIN_2
#define ADC_Control_RESET    GPIO_PIN_1
#define ADC_Control_I2S_LJ   GPIO_PIN_7
#define ADC_Control_MDIV     GPIO_PIN_5
#define ADC_Control_HPF      GPIO_PIN_6

#define ADC_Control_PortC    GPIOC
#define ADC_Control_M0       GPIO_PIN_5
#define ADC_Control_M1       GPIO_PIN_4
//---------------------------------------

#define ADC_Logic_Master          1
#define ADC_Logic_Slave           0
#define ADC_Logic_RESET           0
#define ADC_Logic_unRESET         1
#define ADC_Logic_Mode_I2S        1
#define ADC_Logic_Mode_LJ         0
#define ADC_Logic_Mode_MDIV       1
#define ADC_Logic_Mode_unMDIV     0
#define ADC_Logic_Mode_HPF        0
#define ADC_Logic_Mode_unHPF      1
#define ADC_Logic_Mode_M0         1
#define ADC_Logic_Mode_unM0       0
#define ADC_Logic_Mode_M1         1
#define ADC_Logic_Mode_unM1       0

void I2S_ADC_INIT(void);
void ADC_POWER(uint8_t power,uint8_t state);

#endif /* __I2S_ADC_H */
