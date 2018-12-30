#ifndef __SPI_H_
#define __SPI_H_
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/13 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
#define DUMMY_Byte 0xFF
 
 				  	    													  
void SPI1_Init(void);			 //��ʼ��SPI��
void SPI2_Init(void);
void SPI1_SetSpeed(uint8_t SpeedSet); //����SPI�ٶ�
void SPI2_SetSpeed(uint8_t SpeedSet); //����SPI�ٶ�
uint8_t SPI1_ReadWriteByte(uint8_t TxByte);
uint8_t SPI2_ReadWriteByte(uint8_t TxByte);
uint8_t SPI1_WriteByte(uint8_t * TxByte,uint16_t size);
uint8_t SPI2_WriteByte(uint8_t * TxByte,uint16_t size);
#endif
