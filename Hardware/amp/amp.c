////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : amp.c
//  版 本 号   : V1.1
//  作 		者   : TinderSmith
//  生成日期   : 2014-01-01
//  最近修改   : 2018-04-12
//  功能描述   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  版权所有   : 任复衡
//  说 		明   :
//              ----------------------------------------------------------------
//              1.2018-4-12,配合梦想创新20180405型号开发板的放大器模块使用
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "amp.h"

/*
 * 函数名：AMP_GPIO_Config
 * 描述  ：配置放大器使能端口对应的GPIO
 * 输入  ：无
 * 输出  ：无
 */
void AMP_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = PREAMP_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//前置放大器使能端是下拉电阻,所以使用推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(PREAMP_GPIO_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = ADC_BUF_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//缓冲放大器使能端是下拉电阻,所以使用推挽输出
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
 * 函数名：AMP_INIT
 * 描述  ：初始化放大器
 * 输入  ：无
 * 输出  ：无
 */
void AMP_INIT(void){
	AMP_GPIO_Config();
	AMP_Switch(PREAMP,AMP_EN);
	AMP_Switch(ADC_BUF,AMP_EN);
}
