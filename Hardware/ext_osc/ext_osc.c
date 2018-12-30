////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : ext_osc.c
//  版 本 号   : V1.3
//  作 		者   : TinderSmith
//  生成日期   : 2018-03-28
//  最近修改   : 2018-03-31
//  功能描述   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  版权所有   : 任复衡
//  说 		明   :
//              ----------------------------------------------------------------
//              1.2018-3-31,配合梦想创新20180320型号开发板的DAC模块使用
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "ext_osc.h"
#include "vt.h"//使用外部OSC时候,切断外部I2S口输入的信号

/*
 * 函数名：EXT_OSC_GPIO_Config
 * 描述  ：外部有源晶振的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void EXT_OSC_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.Pin = EXT_OSC_225792_EN_Pin|EXT_OSC_24576_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;      //默认下拉,以免晶振受干扰起振
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(EXT_OSC_EN_Port, &GPIO_InitStruct);
}

void EXT_OSC_Switch(uint8_t osc,uint8_t state)
{
	if(state == EXT_OSC_OFF) {
		HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_225792_EN_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_24576_EN_Pin,GPIO_PIN_RESET);
		VT_Switch(VT_1,VT_EN);//打开外部I2S输入
		VT_Switch(VT_2,VT_EN);
		return;
	}
	
	else if(state == EXT_OSC_ON){
		switch (osc){
			case EXT_OSC_225792:
						VT_Switch(VT_1,VT_DIS);//关闭外部I2S输入
						VT_Switch(VT_2,VT_DIS);
						HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_24576_EN_Pin,GPIO_PIN_RESET);//关24.476外部晶振,原因:两个晶振输入引脚接在一起,故互斥
						HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_225792_EN_Pin,GPIO_PIN_SET); //开22.5792外部晶振,原因:两个晶振输入引脚接在一起,故互斥
				break;
				
			case EXT_OSC_24576:
						VT_Switch(VT_1,VT_DIS);//关闭外部I2S输入
						VT_Switch(VT_2,VT_DIS);
						HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_225792_EN_Pin,GPIO_PIN_RESET);//关22.5792外部晶振,原因:两个晶振输入引脚接在一起,故互斥
						HAL_GPIO_WritePin(EXT_OSC_EN_Port,EXT_OSC_24576_EN_Pin,GPIO_PIN_SET);//开24.476外部晶振,原因:两个晶振输入引脚接在一起,故互斥
				break;
		}
	}
}

/*
 * 函数名：EXT_OSC_INIT
 * 描述  ：初始化外部有源晶振
 * 输入  ：无
 * 输出  ：无
 */
void EXT_OSC_INIT(void){
	EXT_OSC_GPIO_Config();  //配置外部有源晶振使能用到的的I/O口
	EXT_OSC_Switch(EXT_OSC_24576,EXT_OSC_ON);//24.576外部有源晶振使能
}
