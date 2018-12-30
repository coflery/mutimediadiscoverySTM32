////////////////////////////////////////////////////////////////////////////////
//  �� �� ��   : ext_osc.c
//  �� �� ��   : V1.3
//  �� 		��   : TinderSmith
//  ��������   : 2018-03-28
//  ����޸�   : 2018-03-31
//  ��������   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  ��Ȩ����   : �θ���
//  ˵ 		��   :
//              ----------------------------------------------------------------
//              1.2018-3-31,������봴��20180320�ͺſ������DACģ��ʹ��
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "ext_osc.h"
#include "vt.h"//ʹ���ⲿOSCʱ��,�ж��ⲿI2S��������ź�

/*
 * ��������EXT_OSC_GPIO_Config
 * ����  ���ⲿ��Դ�����I/O��
 * ����  ����
 * ���  ����
 */
void EXT_OSC_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = EXT_OSC_225792_EN_Pin|EXT_OSC_24576_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;      //Ĭ������,���⾧���ܸ�������
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(EXT_OSC_EN_Port, &GPIO_InitStruct);
}

void EXT_OSC_Switch(uint8_t osc,uint8_t state)
{
	if(state == EXT_OSC_OFF) {
		HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_225792_EN_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_24576_EN_Pin,GPIO_PIN_RESET);
		VT_Switch(VT_1,VT_EN);//���ⲿI2S����
		VT_Switch(VT_2,VT_EN);
		return;
	}
	
	else if(state == EXT_OSC_ON){
		switch (osc){
			case EXT_OSC_225792:
						VT_Switch(VT_1,VT_DIS);//�ر��ⲿI2S����
						VT_Switch(VT_2,VT_DIS);
						HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_24576_EN_Pin,GPIO_PIN_RESET);//��24.476�ⲿ����,ԭ��:���������������Ž���һ��,�ʻ���
						HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_225792_EN_Pin,GPIO_PIN_SET); //��22.5792�ⲿ����,ԭ��:���������������Ž���һ��,�ʻ���
				break;
				
			case EXT_OSC_24576:
						VT_Switch(VT_1,VT_DIS);//�ر��ⲿI2S����
						VT_Switch(VT_2,VT_DIS);
						HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_225792_EN_Pin,GPIO_PIN_RESET);//��22.5792�ⲿ����,ԭ��:���������������Ž���һ��,�ʻ���
						HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_24576_EN_Pin,GPIO_PIN_SET);//��24.476�ⲿ����,ԭ��:���������������Ž���һ��,�ʻ���
				break;
		}
	}
}

/*
 * ��������EXT_OSC_INIT
 * ����  ����ʼ���ⲿ��Դ����
 * ����  ����
 * ���  ����
 */
void EXT_OSC_INIT(void){
	EXT_OSC_GPIO_Config();  //�����ⲿ��Դ����ʹ���õ��ĵ�I/O��
	EXT_OSC_Switch(EXT_OSC_24576,EXT_OSC_ON);//24.576�ⲿ��Դ����ʹ��
}
