////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : vt.c
//  版 本 号   : V1.0
//  作 		者   : TinderSmith
//  生成日期   : 2014-01-01
//  最近修改   : 2018-03-26
//  功能描述   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  版权所有   : 任复衡
//  说 		明   :
//              ----------------------------------------------------------------
//              1.2018-3-26,配合梦想创新20180320型号开发板的DAC模块使用
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "vt.h"

/*
 * 函数名：VT_GPIO_Config
 * 描述  ：配置电平转换器使能端口对应的GPIO
 * 输入  ：无
 * 输出  ：无
 */
void VT_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = VT1_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;//VT使能端有自己的上拉电阻,所以使用开漏输出
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
 * 函数名：I2S_DAC_INIT
 * 描述  ：初始化电平转换器
 * 输入  ：无
 * 输出  ：无
 */
void Voltage_Translation_INIT(void){
	VT_GPIO_Config();
	VT_Switch(VT_1,VT_DIS);
	VT_Switch(VT_2,VT_DIS);
}
