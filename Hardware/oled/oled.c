////////////////////////////////////////////////////////////////////////////////
//  �� �� ��   : oled.h
//  �� �� ��   : V2.0
//  ��    ��   : TinderSmith
//  ��������   : 2014-01-01
//  ����޸�   : 2017-06-24
//  ��������   : <MP3><stm32 vs1053 oled tfcard(spi) fat0.11>
//  ��Ȩ����   : �θ���
//	˵    ��   :
//              ----------------------------------------------------------------
//              GND        �ӵ�Դ��
//              VCC        ��5V��3.3v��Դ
//              CS         ��PB12
//              RES        ��PC1
//              DC         ��PC2
//              D0(SCKL)   ��PB10(SCK)
//              D1(SDIN)   ��PC3 (MOSI)
//              V1.0 ��������
//              V1.2 �޸�SPI������ʽ
//              V2.0 �Ʒ�ģ��SPI��д��HAL��ʵ��ӲSPIͨ��
//              ----------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
#include "oled.h"
#include "oledfont.h"
#include "spi.h"
//OLED�Դ��ʽ
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
//SPIģʽ��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����
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
//����ģʽ��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����
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
/* ����OLED��ʾ */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0xAF,OLED_CMD);
}
/* �ر�OLED��ʾ */
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0xAE,OLED_CMD);
}
//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
	{  
		OLED_WR_Byte (0xB0+i,OLED_CMD);    //����Ҫд��ҳ��ַ(0-7)
		OLED_WR_Byte (0x00,OLED_CMD);      //����Ҫд��ҳ��ƫ�����ĵ�4λ
		OLED_WR_Byte (0x10,OLED_CMD);      //����Ҫд��ҳ��ƫ�����ĸ�4λ
		for(n=0;n<128;n++)
			OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
	}
}
/*����,��ɺ���Ļ�Ǵ���*/
void OLED_Clear(void)
{
	uint8_t i,n;
	for(i=0;i<8;i++)
		for(n=0;n<128;n++)
			OLED_GRAM[n][i]=0x00;
}
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
	uint8_t pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
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
//m^n����
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
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
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(uint8_t x,uint8_t y,const char *p,uint8_t size)
{
	OLED_ClearLine(y,size);
  while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
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
//���һ��
//y:���y����
//size:�����С
void OLED_ClearLine(uint8_t y,uint8_t size)
{
	uint8_t x=0;
  while(x<128)//�ж��ǲ��ǷǷ��ַ�!
  {
		OLED_ShowChar(x,y,' ',size,1);
		x+=size/2;
  }
}

void OLED_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
#ifdef OLED_MODE	/*����SPI*/
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_3;//2��SPI��˫��ģʽ�µ�MOSI��,������OLED��SDIN
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//�����������
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_10;//2��SPI��˫��ģʽ�µ�SCKʱ����,������OLED��SCKL
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;	//�����������
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_2;//2��SPI��˫��ģʽ�µ�MISO�߿�����Ϊ��ͨIO��ʹ��,������OLED��D/C#
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = GPIO_PIN_1;//������OLED��RST#��λ��
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_12;//2��SPI��˫��ģʽ�µ�NSS�����ģʽ����Ϊ��ͨIOʹ��,������OLED��CS#
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;	//�����������
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
#else
	__HAL_RCC_GPIOE_CLK_ENABLE();
	
 	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //�������
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;//�ٶ�50MHz
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11,GPIO_PIN_SET);	
 #endif
}

//��ʼ��SSD1306
void OLED_Init(void)
{
#ifdef OLED_MODE	/*����SPIģʽ*/
	SPI2_Init();//��ʼ��SPI2,���а���GPIO��RCC��SPI2��ʼ��
#else
	OLED_GPIO_Init();
#endif
	
	OLED_RST_Set();
	HAL_Delay(1);//�ֲ�˵:��λ��ƽ����Ҫά��3us
	OLED_RST_Clr();
	HAL_Delay(1);
	OLED_RST_Set();
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--�ر���ʾ
	OLED_WR_Byte(0x10,OLED_CMD);//--���ø��е�ַ
	OLED_WR_Byte(0x40,OLED_CMD);//--���ÿ�ʼ�е�ַ,[5:0],����(0x00-0x3F)
	
	OLED_WR_Byte(0x81,OLED_CMD);//--������������
	OLED_WR_Byte(0x88,OLED_CMD);//--��������(0-255)
	
	OLED_WR_Byte(0xA1,OLED_CMD);//--����SEG/��ӳ��(���ҷ���/����)(0xa0/0xa1)
	OLED_WR_Byte(0xC0,OLED_CMD);//--����COM/��ɨ�跽��(���·���/����)(0xc0/0xc8)
	
	OLED_WR_Byte(0xA8,OLED_CMD);//--���ø�����(15��63)
	OLED_WR_Byte(0x3F,OLED_CMD);//--64������
	
	OLED_WR_Byte(0xD3,OLED_CMD);//--���õ�һ�е�ƫ���� (0x00-0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//--����ΪĬ��ֵ0
	
	/* OLED��ˢ��ϵ������˸��Ӱ��ܴ� */
	OLED_WR_Byte(0xD5,OLED_CMD);//--�����ڲ�����Ƶ�ʺ�ˢ�·�Ƶϵ��
	OLED_WR_Byte(0xF1,OLED_CMD);//--��Ƶ��=1(bit[3:0]),��������Ϊ���Ƶ(bit[7:4])
	
	OLED_WR_Byte(0xD9,OLED_CMD);//--����Ԥ�������
	OLED_WR_Byte(0xF1,OLED_CMD);//--����Ԥ���16ʱ��&�ŵ�1ʱ��
	
	OLED_WR_Byte(0xDA,OLED_CMD);//--Ӳ��COM��������
	OLED_WR_Byte(0x12,OLED_CMD);//--��ѡCOM��������
	OLED_WR_Byte(0xDB,OLED_CMD);//--VCOMH��ѹ����
	
	OLED_WR_Byte(0x20,OLED_CMD);//--����ҳ��ַģʽ����
	OLED_WR_Byte(0x02,OLED_CMD);//--0x00�е�ַģʽ/0x01�е�ַģʽ/0x02ҳ��ַģʽ
	
	/* �ֲ�Ҫ������ʱ����������ɱ� */
	OLED_WR_Byte(0x8D,OLED_CMD);//--��������ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD);//--0x14��/0x10�ر�
	OLED_WR_Byte(0xAF,OLED_CMD);//--������ʾ(0xae����Ļ/0xaf����Ļ)
	
	OLED_WR_Byte(0xA4,OLED_CMD);//--�ź�Դѡ��(0xa4RAM/0xa5ȫ����)
	OLED_WR_Byte(0xA6,OLED_CMD);//--��ʾ��ʽ(0xa6��ɫ/0xa7��ɫ)
	
	OLED_Clear();
	OLED_Refresh_Gram();
}
