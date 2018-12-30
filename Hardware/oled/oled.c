////////////////////////////////////////////////////////////////////////////////
//  文 件 名   : oled.h
//  版 本 号   : V2.0
//  作    者   : TinderSmith
//  生成日期   : 2014-01-01
//  最近修改   : 2017-06-24
//  功能描述   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  版权所有   : 任复衡
//	说    明   :
//              ----------------------------------------------------------------
//              GND        接电源地
//              VCC        接5V或3.3v电源
//              CS         接PB12
//              RES        接PC1
//              DC         接PC2
//              D0(SCKL)   接PB10(SCK)
//              D1(SDIN)   接PC3 (MOSI)
//              V1.0 初次生成
//              V1.2 修改SPI操作方式
//              V2.0 推翻模拟SPI重写用HAL库实现硬SPI通信
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "oled.h"
#include "oledfont.h"
#include "spi.h"
//OLED显存格式
//GRAM[0][0 1 2 3 ... 127]
//GRAM[1][0 1 2 3 ... 127]
//GRAM[2][0 1 2 3 ... 127]
//GRAM[3][0 1 2 3 ... 127]
//GRAM[4][0 1 2 3 ... 127]
//GRAM[5][0 1 2 3 ... 127]
//GRAM[6][0 1 2 3 ... 127]
//GRAM[7][0 1 2 3 ... 127]
uint8_t OLED_GRAM[128][8];

#ifdef OLED_MODE
//SPI模式向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
	if(cmd)
		OLED_DC_Set();
	else
	  OLED_DC_Clr();
#ifdef SOFT_SPI
	OLED_CS_Clr();
#endif
	OLED_SPI_WriteByte(&dat,1);
	
#ifdef SOFT_SPI
	OLED_CS_Set();
#endif
}
#else
//并口模式向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{
	DATAOUT(dat);
	if(cmd)
	  OLED_DC_Set();
	else
	  OLED_DC_Clr();
	OLED_CS_Clr();
	OLED_WR_Clr();
	OLED_WR_Set();
	OLED_CS_Set();
	OLED_DC_Set();
}
#endif
/* 开启OLED显示 */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0xAF,OLED_CMD);
}
/* 关闭OLED显示 */
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0xAE,OLED_CMD);
}
//更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{  
		OLED_WR_Byte (0xB0+i,OLED_CMD);    //设置要写入页地址(0-7)
		OLED_WR_Byte (0x00,OLED_CMD);      //设置要写入页内偏移量的低4位
		OLED_WR_Byte (0x10,OLED_CMD);      //设置要写入页内偏移量的高4位
		for(n=0;n<128;n++)
			OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
	}
}
/*清屏,完成后屏幕是纯黑*/
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0x00;
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n函数
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
  OLED_ClearLine(y,size);	
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
	OLED_Refresh_Gram();
}
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(uint8_t x,uint8_t y,const char *p,uint8_t size)
{
	OLED_ClearLine(y,size);
  while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
  {       
      if(x>(128-(size/2))){x=0;y+=size;}
      if(y>(64-size))
				{
					y=x=0;
					OLED_Clear();
				}
      OLED_ShowChar(x,y,*p,size,1);	 
      x+=size/2;
      p++;
  }
	OLED_Refresh_Gram();
}
//清除一行
//y:起点y坐标
//size:字体大小
void OLED_ClearLine(uint8_t y,uint8_t size)
{
	uint8_t x=0;
  while(x<128)//判断是不是非法字符!
  {
		OLED_ShowChar(x,y,' ',size,1);
		x+=size/2;
  }
}

void OLED_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
#ifdef OLED_MODE	/*四线SPI*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_3;//2线SPI半双工模式下的MOSI线,这里是OLED的SDIN
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//复用推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_10;//2线SPI半双工模式下的SCK时钟线,这里是OLED的SCKL
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//复用推挽输出
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_2;//2线SPI半双工模式下的MISO线可以作为普通IO口使用,这里是OLED的D/C#
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_1;//这里是OLED的RST#复位线
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_12;//2线SPI半双工模式下的NSS线软件模式下作为普通IO使用,这里是OLED的CS#
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	//复用推挽输出
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
#else
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
 	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;//速度50MHz
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11,GPIO_PIN_SET);	
 #endif
}

//初始化SSD1306
void OLED_Init(void)
{
#ifdef OLED_MODE	/*四线SPI模式*/
	SPI2_Init();//初始化SPI2,其中包含GPIO，RCC，SPI2初始化
#else
	OLED_GPIO_Init();
#endif
	
	OLED_RST_Set();
	HAL_Delay(1);//手册说:复位电平至少要维持3us
	OLED_RST_Clr();
	HAL_Delay(1);
	OLED_RST_Set();
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--关闭显示
	OLED_WR_Byte(0x10,OLED_CMD);//--设置高列地址
	OLED_WR_Byte(0x40,OLED_CMD);//--设置开始行地址,[5:0],行数(0x00-0x3F)
	
	OLED_WR_Byte(0x81,OLED_CMD);//--进入亮度设置
	OLED_WR_Byte(0x88,OLED_CMD);//--设置亮度(0-255)
	
	OLED_WR_Byte(0xA1,OLED_CMD);//--设置SEG/列映射(左右反置/正常)(0xa0/0xa1)
	OLED_WR_Byte(0xC0,OLED_CMD);//--设置COM/行扫描方向(上下反置/正常)(0xc0/0xc8)
	
	OLED_WR_Byte(0xA8,OLED_CMD);//--设置复用率(15到63)
	OLED_WR_Byte(0x3F,OLED_CMD);//--64个都打开
	
	OLED_WR_Byte(0xD3,OLED_CMD);//--设置第一列的偏移量 (0x00-0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//--设置为默认值0
	
	/* OLED的刷新系数对闪烁感影响很大 */
	OLED_WR_Byte(0xD5,OLED_CMD);//--设置内部振荡器频率和刷新分频系数
	OLED_WR_Byte(0xF1,OLED_CMD);//--分频比=1(bit[3:0]),设置振荡器为最高频(bit[7:4])
	
	OLED_WR_Byte(0xD9,OLED_CMD);//--设置预充电周期
	OLED_WR_Byte(0xF1,OLED_CMD);//--设置预充电16时钟&放电1时钟
	
	OLED_WR_Byte(0xDA,OLED_CMD);//--硬件COM引脚配置
	OLED_WR_Byte(0x12,OLED_CMD);//--备选COM引脚配置
	OLED_WR_Byte(0xDB,OLED_CMD);//--VCOMH电压倍率
	
	OLED_WR_Byte(0x20,OLED_CMD);//--进入页地址模式设置
	OLED_WR_Byte(0x02,OLED_CMD);//--0x00列地址模式/0x01行地址模式/0x02页地址模式
	
	/* 手册要求以下时序以启动电荷泵 */
	OLED_WR_Byte(0x8D,OLED_CMD);//--进入亮电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD);//--0x14打开/0x10关闭
	OLED_WR_Byte(0xAF,OLED_CMD);//--开启显示(0xae开屏幕/0xaf关屏幕)
	
	OLED_WR_Byte(0xA4,OLED_CMD);//--信号源选择(0xa4RAM/0xa5全白屏)
	OLED_WR_Byte(0xA6,OLED_CMD);//--显示方式(0xa6正色/0xa7反色)
	
	OLED_Clear();
	OLED_Refresh_Gram();
}
