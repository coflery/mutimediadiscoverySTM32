////////////////////////////////////////////////////////////////////////////////
//  �� �� ��   : phantompower.c
//  �� �� ��   : V1.1
//  �� 		��   : TinderSmith
//  ��������   : 2014-01-01
//  ����޸�   : 2018-03-30
//  ��������   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  ��Ȩ����   : �θ���
//  ˵ 		��   :
//              ----------------------------------------------------------------
//              1.2018-3-30,������봴��20180320�ͺſ������DACģ��ʹ��
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "phantompower.h"

/*
 * ��������PhantomPower_GPIO_Config
 * ����  �����û����Դʹ�ܵ�I/O��
 * ����  ����
 * ���  ����
 */
void PhantomPower_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_InitStruct.Pin = PHANTOMPOWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(PHANTOMPOWER_PortE, &GPIO_InitStruct);
}

void PhantomPower(uint8_t state)
{
if(state==PHANTOMPOWER_ON)
	HAL_GPIO_WritePin(PHANTOMPOWER_PortE,PHANTOMPOWER_Pin,GPIO_PIN_SET);
else if(state==PHANTOMPOWER_OFF)
	HAL_GPIO_WritePin(PHANTOMPOWER_PortE,PHANTOMPOWER_Pin,GPIO_PIN_RESET);
}
/*
 * ��������I2S_DAC_INIT
 * ����  ����ʼ��I2S��DAC
 * ����  ����
 * ���  ����
 */
void PhantomPower_INIT(void){
	PhantomPower_GPIO_Config(); //���û����Դ�õ���GPIO
	PhantomPower(PHANTOMPOWER_ON);//�����Դ�ϵ�
}
