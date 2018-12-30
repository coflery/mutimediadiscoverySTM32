////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : phantompower.c
//  版 本 号   : V1.1
//  作 		者   : TinderSmith
//  生成日期   : 2014-01-01
//  最近修改   : 2018-03-30
//  功能描述   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  版权所有   : 任复衡
//  说 		明   :
//              ----------------------------------------------------------------
//              1.2018-3-30,配合梦想创新20180320型号开发板的DAC模块使用
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "phantompower.h"

/*
 * 函数名：PhantomPower_GPIO_Config
 * 描述  ：配置幻象电源使能的I/O口
 * 输入  ：无
 * 输出  ：无
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
 * 函数名：I2S_DAC_INIT
 * 描述  ：初始化I2S的DAC
 * 输入  ：无
 * 输出  ：无
 */
void PhantomPower_INIT(void){
	PhantomPower_GPIO_Config(); //配置幻象电源用到的GPIO
	PhantomPower(PHANTOMPOWER_ON);//幻象电源上电
}
