#ifndef __FILE_H_
#define __FILE_H_

#include "stm32f4xx_hal.h"
#include "mmc_sd.h"
#include "ff.h"
#include "uda1380.h"
#include "u8g_arm.h"
#include "wav.h"
#include "sys.h"

#define	BUFF_MAX_SS		4096	/* 512, 1024, 2048 or 4096 */

void get_disk_info(void);	//»ñÈ¡SD¿¨ÄÚÈÝ
void AnalysisFile(void);
void Audio_Play(uint32_t AudioFreq);
#endif
