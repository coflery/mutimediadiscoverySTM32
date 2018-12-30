////////////////////////////////////////////////////////////////////////////////
//  �� �� ��   : i2s_dac.c
//  �� �� ��   : V1.1
//  �� 		��   : TinderSmith
//  ��������   : 2014-01-01
//  ����޸�   : 2018-03-26
//  ��������   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  ��Ȩ����   : �θ���
//  ˵ 		��   :
//              ----------------------------------------------------------------
//              1.2018-3-26,������봴��20180320�ͺſ������DACģ��ʹ��
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "i2s_dac.h"

/*
 * ��������DAC_GPIO_Config
 * ����  ������DAC��I/O��
 * ����  ����
 * ���  ����
 */
void DAC_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = DAC_POWER_EN_Pin|DAC_LOGIC_MUTE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(DAC_GPIO_PortD, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = DAC_LOGIC_ZD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(DAC_GPIO_PortD, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = DAC_LOGIC_DIF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(DAC_GPIO_PortC, &GPIO_InitStruct);
  
}

void DAC_POWER(uint8_t state)
{
if(state)
	HAL_GPIO_WritePin(DAC_GPIO_PortD,DAC_POWER_EN_Pin,GPIO_PIN_SET);
else
	HAL_GPIO_WritePin(DAC_GPIO_PortD,DAC_POWER_EN_Pin,GPIO_PIN_RESET);
}

void DAC_MUTE(uint8_t state)
{
if(state)
	HAL_GPIO_WritePin(DAC_GPIO_PortD,DAC_LOGIC_MUTE_Pin,GPIO_PIN_SET);
else
	HAL_GPIO_WritePin(DAC_GPIO_PortD,DAC_LOGIC_MUTE_Pin,GPIO_PIN_RESET);
}

void DAC_DIF(uint8_t state)
{
if(state)
	HAL_GPIO_WritePin(DAC_GPIO_PortC,DAC_LOGIC_DIF_Pin,GPIO_PIN_SET);
else
	HAL_GPIO_WritePin(DAC_GPIO_PortC,DAC_LOGIC_DIF_Pin,GPIO_PIN_RESET);
}

/*
 * ��������I2S_DAC_INIT
 * ����  ����ʼ��I2S��DAC
 * ����  ����
 * ���  ����
 */
void I2S_DAC_INIT(void){
	DAC_GPIO_Config();      //����DAC�õ���GPIO
	DAC_MUTE(DAC_MUTE_DIS); //ȡ������
	DAC_DIF(DAC_DIF_I2S);   //����I2S���ݸ�ʽ
	DAC_POWER(DAC_POWER_OFF);//DAC����Դ�ϵ�
}
