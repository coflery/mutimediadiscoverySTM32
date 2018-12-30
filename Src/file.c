#include "file.h"

	FATFS fs;
	FATFS *fls = &fs;
	FIL fp;
	FRESULT res;
	UINT br;
	DWORD clust;
/* 此处开两个buffer交换缓冲,用于DMA传输数据到I2S */
uint16_t buffer1[BUFF_MAX_SS] ={0x00};
uint16_t buffer2[BUFF_MAX_SS] ={0x00};

WAVE_FormatTypeDef WAVE_Format;

__IO uint32_t WaveCounter;
__IO uint32_t WaveDataLength = 0;
__IO uint32_t XferCplt = 1;/* wait for DMA transfert complete Flag*/



void get_disk_info(void)	//获取SD卡内容
{
	SD_Initialize();
	res = f_mount(&fs,"0", 1);
	if (res != FR_OK)
	{
		printf(" >挂载文件系统失败代码: %u\r\n",res);
		return;
	}
	res = f_getfree("/",&clust,&fls);
	if (res == FR_OK)
	{
		printf(" >剩余%ld MB ""\r\n总共%ld MB \r\n",
						clust * fls->csize /2/1024,
						(DWORD)(fls->n_fatent - 2) * fls->csize /2/1024);
		//空闲的磁盘空间M=剩余簇数*每簇的扇区数/2/1024
		//总的磁盘空间M        =（总簇数-2）*每簇的扇区数/2/1024=可用簇数*每簇的扇区数/2/1024
		
		//u8g_DrawStr(&u8g, 2, 12, "Free/Total(MB)");
		
//		OLED_ShowString(0,1,"Free/Total(MB)",12);
//		OLED_ShowNum(0,2,clust * fls->csize /2/1024,4,12);
//		OLED_ShowString(25,2,"/       ",12);
//		OLED_ShowNum(30,2,(DWORD)(fls->n_fatent - 2) * fls->csize /2/1024,4,12);
	}
	
	else
	{
		printf(" >获得存储信息失败,代码%u\r\n",res);
	}
	f_mount(NULL,"0",1);
}

void AnalysisFile(void)
{
	DIR dirs;
	char path[]="0:/";
	
	
	Audio_Play(WAVE_Format.SampleRate);
	
	
	
	
	while(SD_Initialize())
  {
		printf(" >初始化SD卡失败\r\n");
		//OLED_ShowString(0,0,"SD Init Fail",12);
		HAL_Delay(100);
	}
	while (f_mount(&fs,"0", 1) != FR_OK)
	{
		printf(" >挂载文件系统失败\r\n");
		//OLED_ShowString(0,0,"Mount FatFs Fail",12);
		HAL_Delay(1000);
	}
	while(f_opendir(&dirs,path) != FR_OK)
	{
		printf(" >打开目录%s失败\r\n",path);
		//OLED_ShowString(0,0,"Open Dir Fail",12);
		HAL_Delay(1000);
	}
	while(f_open(&fp, "0:Audio.wav", FA_OPEN_EXISTING | FA_READ)!=FR_OK)
	{
		printf(" >Audio.wav文件打开失败\r\n");
		//OLED_ShowString(0,0,"File Open Fail",12);
		HAL_Delay(1000);
	}
	while(f_read (&fp, buffer1, BUFF_MAX_SS, &br)!=FR_OK)
	{
		printf(" >Audio.wav文件读取失败\r\n");
		//OLED_ShowString(0,0,"File Read Fail",12);
		HAL_Delay(1000);
	}
	printf(" >开始解析Wave文件\r\n");
	//OLED_ShowString(0,0,"Analysising wave file",12);
	
	WAVE_Format = WavePlayer_WaveParsing(buffer1);
	if (WAVE_Format.Result == Valid_WAVE_File)/* 文件格式支持 */
		{
			WaveDataLength = WAVE_Format.DataSize;
			printf(" >文件格式支持\r\n");
			//OLED_ShowString(0,0,"File Format OK",12);
			Audio_Play(WAVE_Format.SampleRate);
		}
	else
		{
			printf(" >文件格式不支持\r\n");
			//OLED_ShowString(0,0,"FileFormat Bad",12);
		}
}

uint16_t *plyingBuf=buffer1;
void Audio_Play(uint32_t AudioFreq)
{FILE *f;
uint16_t i;
	f_lseek(&fp, WAVE_Format.FileOffset);//移动偏移指针跳过文件头
  	f_read (&fp, buffer1, BUFF_MAX_SS, &br); 
  	f_read (&fp, buffer2, BUFF_MAX_SS, &br);
	printf(" >开始播放！\r\n");
	printf(" >Wave文件大小: %dM\r\n",WaveDataLength/1000/1000);
	I2S_Configuration(AudioFreq);
	
	UDA1380_Configuration();
	
	while(1)
		Audio_MAL_Play(buffer1, BUFF_MAX_SS);
	
	while(WaveDataLength != 0)
  {
		//OLED_ShowNum(0,0,GetTimeFlying(),8,12);
		i=0;
		while(i<BUFF_MAX_SS)
		{
			fputc((int)plyingBuf[i], f);
			i++;
		}
		
	}
	printf("传输完成\r\n");
}

//uint8_t buffer_switch = 1;
//void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
//{
//  /* Calculate the remaining audio data in the file and the new size 
//  for the DMA transfer. If the Audio files size is less than the DMA max 
//  data transfer size, so there is no calculation to be done, just restart 
//  from the beginning of the file ... */
//  /* Check if the end of file has been reached */
//	
//	Audio_MAL_Play(plyingBuf, BUFF_MAX_SS);
//	if(buffer_switch)//交叉双缓冲实现更高并发量
//	{
//		f_read (&fp, buffer1, BUFF_MAX_SS, &br);
//	}
//	else
//	{
//		f_read (&fp, buffer2, BUFF_MAX_SS, &br);
//	}
//	
//#ifdef AUDIO_MAL_MODE_NORMAL
//  if (WaveDataLength) WaveDataLength -= BUFF_MAX_SS;
//  if (WaveDataLength < BUFF_MAX_SS) WaveDataLength = 0;
//#endif
//}

//void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
//{
//	if(buffer_switch)//交叉双缓冲实现更高并发量
//		{
//			plyingBuf=buffer1;
//			buffer_switch = 0;
//		}
//	else
//		{
//			plyingBuf=buffer2;
//			buffer_switch = 1;
//		}
//}
