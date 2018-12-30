////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : i2s_adc.c
//  版 本 号   : V1.0
//  作 		者   : TinderSmith
//  生成日期   : 2014-01-01
//  最近修改   : 2018-03-28
//  功能描述   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  版权所有   : 任复衡
//  说 		明   :
//              ----------------------------------------------------------------
//              1.2018-3-28,配合梦想创新20180320型号开发板的ADC模块使用
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "i2s_adc.h"

/*
 * 函数名：ADC_GPIO_Config
 * 描述  ：配置ADC的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void ADC_PowerManage_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_InitStruct.Pin = ADC_EN_P5V0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(ADC_EN_P5V0Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = ADC_EN_P3V3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(ADC_EN_P3V3Port, &GPIO_InitStruct);
}

void ADC_Control_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = ADC_Control_MS|ADC_Control_RESET|ADC_Control_I2S_LJ|
											  ADC_Control_MDIV|ADC_Control_HPF;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(ADC_Control_PortA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = ADC_Control_M0|ADC_Control_M1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(ADC_Control_PortC, &GPIO_InitStruct);
}
void ADC_POWER(uint8_t power,uint8_t state)
{
	switch(power){
		case ADC_P3V3:
			if(state)
				HAL_GPIO_WritePin(ADC_EN_P3V3Port,ADC_EN_P3V3_Pin,GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(ADC_EN_P3V3Port,ADC_EN_P3V3_Pin,GPIO_PIN_RESET);
			break;
			
		case ADC_P5V0:
			if(state)
				HAL_GPIO_WritePin(ADC_EN_P5V0Port,ADC_EN_P5V0_Pin,GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(ADC_EN_P5V0Port,ADC_EN_P5V0_Pin,GPIO_PIN_RESET);
			break;
	}
}
void ADC_Reset(void)
{
	HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_RESET,GPIO_PIN_RESET);
	HAL_Delay(150);
	HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_RESET,GPIO_PIN_SET);
}

void ADC_Mode(uint8_t mode)
{
			if(mode==ADC_Logic_Master)
				HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_MS,GPIO_PIN_SET);
			else if(mode==ADC_Logic_Slave)
				HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_MS,GPIO_PIN_RESET);
}

void ADC_I2S_Format(uint8_t format)
{
	if(format==ADC_Logic_Mode_I2S)
		HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_I2S_LJ,GPIO_PIN_SET);
	else if(format==ADC_Logic_Mode_LJ)
		HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_I2S_LJ,GPIO_PIN_RESET);
}
void ADC_MDIV(uint8_t mdiv)
{
	if(mdiv==ADC_Logic_Mode_MDIV)
		HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_MDIV,GPIO_PIN_SET);
	else if(mdiv==ADC_Logic_Mode_unMDIV)
		HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_MDIV,GPIO_PIN_RESET);
}
void ADC_HPF(uint8_t hpf)
{
	if(hpf==ADC_Logic_Mode_HPF)
		HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_HPF,GPIO_PIN_SET);
	else if(hpf==ADC_Logic_Mode_unHPF)
		HAL_GPIO_WritePin(ADC_Control_PortA,ADC_Control_HPF,GPIO_PIN_RESET);
}
void ADC_M0(uint8_t m0)
{
	if(m0==ADC_Logic_Mode_M0)
		HAL_GPIO_WritePin(ADC_Control_PortC,ADC_Control_M0,GPIO_PIN_SET);
	else if(m0==ADC_Logic_Mode_unM0)
		HAL_GPIO_WritePin(ADC_Control_PortC,ADC_Control_M0,GPIO_PIN_RESET);
}
void ADC_M1(uint8_t m1)
{
	if(m1==ADC_Logic_Mode_M1)
		HAL_GPIO_WritePin(ADC_Control_PortC,ADC_Control_M1,GPIO_PIN_SET);
	else if(m1==ADC_Logic_Mode_unM1)
		HAL_GPIO_WritePin(ADC_Control_PortC,ADC_Control_M1,GPIO_PIN_RESET);
}

/*
 * 函数名：I2S_ADC_INIT
 * 描述  ：初始化I2S的ADC
 * 输入  ：无
 * 输出  ：无
 */
void I2S_ADC_INIT(void){
	ADC_PowerManage_GPIO_Config();      //配置ADC用到的电源管理GPIO
	ADC_Control_GPIO_Config();					//配置ADC用到的逻辑管理GPIO
	
	ADC_POWER(ADC_P3V3,ADC_POWER_ON);//ADC数字电路上电
	ADC_POWER(ADC_P5V0,ADC_POWER_ON);//ADC模拟电路上电
	
	
	ADC_Reset();
	ADC_Mode(ADC_Logic_Slave);
	ADC_I2S_Format(ADC_Logic_Mode_I2S);
	ADC_MDIV(ADC_Logic_Mode_unMDIV);
	ADC_HPF(ADC_Logic_Mode_HPF);
	
	ADC_M0(ADC_Logic_Mode_unM0);
	ADC_M1(ADC_Logic_Mode_unM1);
}
