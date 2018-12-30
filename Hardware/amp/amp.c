////////////////////////////////////////////////////////////////////////////////
//  �� �� ��   : amp.c
//  �� �� ��   : V1.1
//  �� 		��   : TinderSmith
//  ��������   : 2014-01-01
//  ����޸�   : 2018-04-12
//  ��������   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  ��Ȩ����   : �θ���
//  ˵ 		��   :
//              ----------------------------------------------------------------
//              1.2018-4-12,������봴��20180405�ͺſ�����ķŴ���ģ��ʹ��
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "amp.h"

/*
 * ��������AMP_GPIO_Config
 * ����  �����÷Ŵ���ʹ�ܶ˿ڶ�Ӧ��GPIO
 * ����  ����
 * ���  ����
 */
void AMP_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = PREAMP_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//ǰ�÷Ŵ���ʹ�ܶ�����������,����ʹ���������
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(PREAMP_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = ADC_BUF_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//����Ŵ���ʹ�ܶ�����������,����ʹ���������
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(ADC_BUF_GPIO_Port, &GPIO_InitStruct);
  
}

void AMP_Switch(uint8_t amp,uint8_t state)
{
	switch(amp)
	{
		case PREAMP:
			if(state == AMP_EN)
				HAL_GPIO_WritePin(PREAMP_GPIO_Port,PREAMP_GPIO_Pin,GPIO_PIN_SET);
			else if(state == AMP_DIS)
				HAL_GPIO_WritePin(PREAMP_GPIO_Port,PREAMP_GPIO_Pin,GPIO_PIN_RESET);
			break;
		case ADC_BUF:
			if(state == AMP_EN)
				HAL_GPIO_WritePin(ADC_BUF_GPIO_Port,ADC_BUF_GPIO_Pin,GPIO_PIN_SET);
			else if(state == AMP_DIS)
				HAL_GPIO_WritePin(ADC_BUF_GPIO_Port,ADC_BUF_GPIO_Pin,GPIO_PIN_RESET);
			break;
	}
}

/*
 * ��������AMP_INIT
 * ����  ����ʼ���Ŵ���
 * ����  ����
 * ���  ����
 */
void AMP_INIT(void){
	AMP_GPIO_Config();
	AMP_Switch(PREAMP,AMP_EN);
	AMP_Switch(ADC_BUF,AMP_EN);
}
