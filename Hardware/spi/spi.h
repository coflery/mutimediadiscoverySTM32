#ifndef __SPI_H_
#define __SPI_H_
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//SPI 驱动函数	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/6/13 
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
#define DUMMY_Byte 0xFF
 
 				  	    													  
void SPI1_Init(void);			 //初始化SPI口
void SPI2_Init(void);
void SPI1_SetSpeed(uint8_t SpeedSet); //设置SPI速度
void SPI2_SetSpeed(uint8_t SpeedSet); //设置SPI速度
uint8_t SPI1_ReadWriteByte(uint8_t TxByte);
uint8_t SPI2_ReadWriteByte(uint8_t TxByte);
uint8_t SPI1_WriteByte(uint8_t * TxByte,uint16_t size);
uint8_t SPI2_WriteByte(uint8_t * TxByte,uint16_t size);
#endif
