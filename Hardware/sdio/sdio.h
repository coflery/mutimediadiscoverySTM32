#ifndef __SDIO_H
#define	__SDIO_H

#include "stm32f4xx_hal.h"



void MX_SDIO_SD_Init(void);
void HAL_SD_MspInit(SD_HandleTypeDef* hsd);
void HAL_SD_MspDeInit(SD_HandleTypeDef* hsd);

#endif /* __SDIO_H */
