#include "stm32f4xx_hal.h"
#include "usb_device.h"
#include "sys.h"
#include "usart.h"
#include "file.h"
#include "led.h"
#include "vt.h"
#include "i2s_dac.h"
#include "i2s_adc.h"
#include "phantompower.h"
#include "ext_osc.h"
#include "u8g_arm.h"
#include "sdio.h"
#include "amp.h"
#include "dma.h"

static u8g_t u8g;

void draw(void)
{
	u8g_SetFont(&u8g,u8g_font_profont10);//set current font
	u8g_DrawStr(&u8g, 6, 12, "Welcome Coflery!");//write string - you set coordinates and string
	u8g_DrawBox(&u8g, 30, 30, 35, 35);//draw some box
}



int main(void)
{
	HAL_Init();
	SystemClock_Config();
	HAL_Delay(500);
	MX_GPIO_Init();
	RTC_Init();
	USART_Init();
	MX_I2C2_Init();
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_i2c, u8g_com_hw_i2c_fn); //³õÊ¼»¯u8glibÆÁÄ»Çý¶¯
	
	u8g_FirstPage(&u8g);
	do
	{
		draw();
	} while ( u8g_NextPage(&u8g) );
	
	I2S_DAC_INIT();
	DAC_POWER(DAC_POWER_ON);
	LED_INIT();
	Voltage_Translation_INIT();
		
  //MX_DMA_Init();
	
	//MX_USB_DEVICE_Init();
	
	VT_Switch(VT_1,VT_EN);
	VT_Switch(VT_2,VT_EN);
	
	I2S_ADC_INIT();
	PhantomPower_INIT();
	//MX_SDIO_SD_Init();
	AMP_INIT();
	
	while(1)
	{
		LED_Switch(AMBER,LED_ON);
		HAL_Delay(100);
		LED_Switch(RED,LED_ON);
		HAL_Delay(100);
		LED_Switch(GREEN,LED_ON);
		HAL_Delay(100);
		LED_Switch(BLUE,LED_ON);
		HAL_Delay(100);
		LED_INIT();
		HAL_Delay(100);
		LED_Switch(BLUE,LED_ON);
		HAL_Delay(100);
		LED_Switch(GREEN,LED_ON);
		HAL_Delay(100);
		LED_Switch(RED,LED_ON);
		HAL_Delay(100);
		LED_Switch(AMBER,LED_ON);
		HAL_Delay(100);
		LED_INIT();
	}
	//AnalysisFile();
}
