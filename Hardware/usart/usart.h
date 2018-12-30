#ifndef __USART_H
#define __USART_H	
#include "stm32f4xx_hal.h"
#include "sys.h"

/* 最大接收字节数 */
#define USART_RxLen_MAX                  200

/* 波特率配置 */
#define USARTx_BaudRate                  115200

/* 时钟配置 */
#define USARTx                           USART2
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()

/* NVIC配置 */
#define USARTx_IRQn                      USART2_IRQn
#define USARTx_IRQHandler                USART2_IRQHandler

/* GPIO配置 */
#define USARTx_TX_PIN                    GPIO_PIN_5
#define USARTx_TX_GPIO_PORT              GPIOD  
#define USARTx_TX_AF                     GPIO_AF7_USART2
#define USARTx_RX_PIN                    GPIO_PIN_6
#define USARTx_RX_GPIO_PORT              GPIOD 
#define USARTx_RX_AF                     GPIO_AF7_USART2


void sendWave2PC_u16(uint16_t num);
void sendWave2PC_float(float num);
void USART_Init(void);
#endif
