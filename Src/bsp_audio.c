#////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : bsp_audio.h
//  版 本 号   : V1.1
//  作 		者   : TinderSmith
//  生成日期   : 2014-01-01
//  最近修改   : 2018-04-21
//  功能描述   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  版权所有   : 任复衡
//  说 		明   :
//              ----------------------------------------------------------------
//              1.2018-4-21,配合梦想创新V2型号开发板的USB-AUDIO使用
//             
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "bsp_audio.h"
#include "ext_osc.h"

I2S_HandleTypeDef hi2s2;

/*
 * 函数名：I2Sx_Init
 * 描述  ：初始化外部DAC
 * 输入  ：无
 * 输出  ：无
 */
static void I2Sx_Init(uint32_t AudioFreq)  
{  
  /* Initialize the haudio_i2s Instance parameter */  
  hi2s2.Instance = SPI2;
  
  /* Disable I2S block */  
  __HAL_I2S_DISABLE(&hi2s2);
  
  hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B_EXTENDED;//在24bit上面传送16bit数据,用于兼容各种输出
	hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
	hi2s2.Init.AudioFreq = AudioFreq;
  hi2s2.Init.CPOL = I2S_CPOL_LOW;
	hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
	
	EXT_OSC_INIT();
	
	if(AudioFreq%48000 == 0)
	{
		EXT_OSC_Switch(EXT_OSC_24576,EXT_OSC_ON);//24.576外部有源晶振使能
	}
	else if(AudioFreq%44100 == 0)
	{
		EXT_OSC_Switch(EXT_OSC_225792,EXT_OSC_ON);//22.5792外部有源晶振使能
	}
	
  /* Init the I2S */
	if (HAL_I2S_Init(&hi2s2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
	//For HAL bugs: only add below code can use I2S EXT OSC @RenFuheng
	__HAL_RCC_PLLI2S_DISABLE();
	__HAL_RCC_I2S_CONFIG(RCC_I2SCLKSOURCE_EXT);	
	//set I2SPR register MCLK=Enable,ODD=Disable,I2SDIV=0x01;
	*(__IO uint32_t *)(SPI2_BASE + 0x20)= (0x00000201U);
	
}
const uint32_t I2SFreq[8] = {8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000};  
const uint32_t I2SPLLN[8] = {256,  429,   213,   429,   213,   271,   258,   344};  
const uint32_t I2SPLLR[8] = {5,    4,     2,     4,     2,     2,     3,     2};  
uint8_t BSP_AUDIO_OUT_Init(uint8_t Volume, uint32_t AudioFreq)  
{
    uint8_t index = 0, freqindex = 0xFF;
    RCC_PeriphCLKInitTypeDef RCC_ExCLKInitStruct;  
  
    //get the according P,N value and set into config,this is for audio clock provide  
    for(index = 0; index < 8; index++)  
    {  
        if(I2SFreq[index] == AudioFreq)
        {  
            freqindex = index;  
        }  
    }
    HAL_RCCEx_GetPeriphCLKConfig(&RCC_ExCLKInitStruct);  
    if(freqindex != 0xFF)  
    {
        RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_PLLI2S;//RCC_PERIPHCLK_I2S;
        RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = I2SPLLN[freqindex];  
				RCC_ExCLKInitStruct.PLLI2S.PLLI2SR = I2SPLLR[freqindex];
    }
    else
    {
        RCC_ExCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_PLLI2S;
				RCC_ExCLKInitStruct.PLLI2S.PLLI2SN = 100;
				RCC_ExCLKInitStruct.PLLI2S.PLLI2SR = 2;
    }
		HAL_RCCEx_PeriphCLKConfig(&RCC_ExCLKInitStruct);
		
    I2Sx_Init(AudioFreq);
    return AUDIO_OK;
}

//-------------------for I2s interrupt--------------------------//
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Manage the remaining file size and new address offset: This function
     should be coded by user (its prototype is already declared in stm324xg_eval_audio.h) */
		TransferComplete_CallBack_FS();
}

/**
  * @brief Tx Transfer Half completed callbacks
  * @param hi2s: I2S handle
  */
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
  /* Manage the remaining file size and new address offset: This function
     should be coded by user (its prototype is already declared in stm324xg_eval_audio.h) */
		HalfTransfer_CallBack_FS();
}
uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Cmd)
{
  /* Call the Codec Mute function */
	  /* Set the Mute mode */
  if(Cmd == AUDIO_MUTE_ON)
  {
		DAC_MUTE(DAC_MUTE_EN); //静音
		LED_Switch(AMBER,LED_ON);
  }
  else if(Cmd == AUDIO_MUTE_OFF)/* AUDIO_MUTE_OFF Disable the Mute */
  {
		DAC_MUTE(DAC_MUTE_DIS); //取消静音
		LED_Switch(AMBER,LED_OFF);
  }
	return AUDIO_OK;
}

uint8_t BSP_AUDIO_OUT_Stop(void)
{
  /* Call the Media layer stop function */
  HAL_I2S_DMAStop(&hi2s2);
	DAC_MUTE(DAC_MUTE_EN); //静音
  HAL_Delay(1);
	DAC_POWER(DAC_POWER_OFF);//DAC主电源下电
	
  /* Return AUDIO_OK when all operations are correctly done */
  return AUDIO_OK;
}
