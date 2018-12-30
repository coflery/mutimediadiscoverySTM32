////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : led.c
//  版 本 号   : V2.2
//  作 		者   : TinderSmith
//  生成日期   : 2014-01-01
//  最近修改   : 2018-04-9
//  功能描述   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  版权所有   : 任复衡
//  说 		明   :
//              ----------------------------------------------------------------
//             1.20180421添加闪烁效果
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "led.h"

/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOE_CLK_ENABLE();

	GPIO_InitStruct.Pin = AMBER_GPIO_Pin|RED_GPIO_Pin|GREEN_GPIO_Pin|BLUE_GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
}

void LED_Switch(uint8_t led,uint8_t state)
{
	switch(led){
		case AMBER:
			if(state)
				HAL_GPIO_WritePin(LED_GPIO_Port,AMBER_GPIO_Pin,GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(LED_GPIO_Port,AMBER_GPIO_Pin,GPIO_PIN_RESET);
			break;
			
		case RED:
			if(state)
				HAL_GPIO_WritePin(LED_GPIO_Port,RED_GPIO_Pin,GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(LED_GPIO_Port,RED_GPIO_Pin,GPIO_PIN_RESET);
			break;
			
		case GREEN:
			if(state)
				HAL_GPIO_WritePin(LED_GPIO_Port,GREEN_GPIO_Pin,GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(LED_GPIO_Port,GREEN_GPIO_Pin,GPIO_PIN_RESET);
			break;
			
		case BLUE:
			if(state)
				HAL_GPIO_WritePin(LED_GPIO_Port,BLUE_GPIO_Pin,GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(LED_GPIO_Port,BLUE_GPIO_Pin,GPIO_PIN_RESET);
			break;
	}
}
void LED_Toggle(uint8_t led)
{
	switch(led){
		case AMBER:
				HAL_GPIO_TogglePin(LED_GPIO_Port,AMBER_GPIO_Pin);
			break;
			
		case RED:
				HAL_GPIO_TogglePin(LED_GPIO_Port,RED_GPIO_Pin);
			break;
			
		case GREEN:
				HAL_GPIO_TogglePin(LED_GPIO_Port,GREEN_GPIO_Pin);
			break;
			
		case BLUE:
			HAL_GPIO_TogglePin(LED_GPIO_Port,BLUE_GPIO_Pin);
			break;
	}
}
/*
 * 函数名：LED_INIT
 * 描述  ：初始化LED
 * 输入  ：无
 * 输出  ：无
 */
void LED_INIT(void){
	LED_GPIO_Config();      //配置LED用到的GPIO
	LED_Switch(AMBER,LED_OFF);
	LED_Switch(RED,  LED_OFF);
	LED_Switch(GREEN,LED_OFF);
	LED_Switch(BLUE, LED_OFF);
}
