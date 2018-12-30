////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : oled.h
//  版 本 号   : V2.0
//  作    者   : TinderSmith
//  生成日期   : 2014-01-01
//  最近修改   : 2017-06-24
//  功能描述   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  版权所有   : 任复衡
//	说    明   :
//              ----------------------------------------------------------------
//              GND        接电源地
//              VCC        接5V或3.3v电源
//              CS         接PB12
//              RES        接PC1
//              DC         接PC2
//              D0(SCKL)   接PB10(SCK)
//              D1(SDIN)   接PC3 (MOSI)
//              V1.0 初次生成
//              V1.2 修改SPI操作方式
//              V2.0 推翻模拟SPI重写用HAL库实现硬SPI通信
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H
#include "stm32f4xx_hal.h"
#include "sys.h"

#define OLED_MODE		//有定义为4线SPI模式,无定义为并口8080模式
//#define SOFT_SPI		//有定义则使用软件SPI模式
#define OLED_DATA		1	//写数据
#define OLED_CMD		0	//写命令
#define SIZE				8
#define XLevelL			0
#define XLevelH			16
#define Max_Column	128
#define Max_Row			64
#define	Brightness	255
#define X_WIDTH			128
#define Y_WIDTH			64

/*复位引脚*/
#define OLED_RST_Clr()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET)//RST
#define OLED_RST_Set()	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET)

#ifdef OLED_MODE
		/*4线SPI模式*/
		#define OLED_SPI_WriteByte(a,b)		SPI2_WriteByte(a,b)
		
		#ifdef SOFT_SPI
			#define OLED_CS_Clr()		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET)//CS#
			#define OLED_CS_Set()		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET)	
		#endif

		#define OLED_DC_Clr()		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET)//D/C#
		#define OLED_DC_Set()		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET)
#else
		/*8080并口模式*/
		#define DATAOUT(x)		HAL_GPIO_WritePinData(GPIOC, x, 0);//PB0到7为数据线
		#define OLED_CS_Clr()	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_RESET)//CS#
		#define OLED_CS_Set()	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_8,GPIO_PIN_SET)
		#define OLED_WR_Clr()	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET)//WR#
		#define OLED_WR_Set()	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET)
		#define OLED_RD_Clr()	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_RESET//RD#
		#define OLED_RD_Set()	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_10,GPIO_PIN_SET)
		#define OLED_DC_Clr()	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_RESET)//D/C#
		#define OLED_DC_Set()	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_11,GPIO_PIN_SET)
#endif


//OLED控制用函数
void OLED_Refresh_Gram(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  ;
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,const char *p,uint8_t size);
void OLED_ClearLine(uint8_t y,uint8_t size);
#endif
