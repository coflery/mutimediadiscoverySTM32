////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : i2s_dac.c
//  版 本 号   : V1.1
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
#include "i2s_dac.h"

/*
 * 函数名：DAC_GPIO_Config
 * 描述  ：配置DAC的I/O口
 * 输入  ：无
 * 输出  ：无
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
 * 函数名：I2S_DAC_INIT
 * 描述  ：初始化I2S的DAC
 * 输入  ：无
 * 输出  ：无
 */
void I2S_DAC_INIT(void){
	DAC_GPIO_Config();      //配置DAC用到的GPIO
	DAC_MUTE(DAC_MUTE_DIS); //取消静音
	DAC_DIF(DAC_DIF_I2S);   //输入I2S数据格式
	DAC_POWER(DAC_POWER_OFF);//DAC主电源上电
}
