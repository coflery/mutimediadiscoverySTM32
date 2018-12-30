////////////////////////////////////////////////////////////////////////////////
//  �� �� ��   : vt.c
//  �� �� ��   : V1.0
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
#include "vt.h"

/*
 * ��������VT_GPIO_Config
 * ����  �����õ�ƽת����ʹ�ܶ˿ڶ�Ӧ��GPIO
 * ����  ����
 * ���  ����
 */
void VT_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = VT1_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;//VTʹ�ܶ����Լ�����������,����ʹ�ÿ�©���
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(VT1_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = VT2_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(VT2_GPIO_Port, &GPIO_InitStruct);
  
}

void VT_Switch(uint8_t port,uint8_t state)
{
	switch(port)
	{
		case VT_1:
			if(state == VT_EN)
				HAL_GPIO_WritePin(VT1_GPIO_Port,VT1_GPIO_Pin,GPIO_PIN_RESET);
			else if(state == VT_DIS)
				HAL_GPIO_WritePin(VT1_GPIO_Port,VT1_GPIO_Pin,GPIO_PIN_SET);
			break;
		case VT_2:
			if(state == VT_EN)
				HAL_GPIO_WritePin(VT2_GPIO_Port,VT2_GPIO_Pin,GPIO_PIN_RESET);
			else if(state == VT_DIS)
				HAL_GPIO_WritePin(VT2_GPIO_Port,VT2_GPIO_Pin,GPIO_PIN_SET);
			break;
	}
}

/*
 * ��������I2S_DAC_INIT
 * ����  ����ʼ����ƽת����
 * ����  ����
 * ���  ����
 */
void Voltage_Translation_INIT(void){
	VT_GPIO_Config();
	VT_Switch(VT_1,VT_DIS);
	VT_Switch(VT_2,VT_DIS);
}
