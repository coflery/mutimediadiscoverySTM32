//#define FFT_NUM  2048
//#define FFT_STEP  1
//#define OLED_HIGHT 64
//#include "arm_math.h"
//float FFT_input[FFT_NUM*2];
//float FFT_MO_output[FFT_NUM];  //��Ϊ���ģֵ
//unsigned short i;
//arm_cfft_radix4_instance_f32 scfft;	
//
//
//
//	arm_cfft_radix4_init_f32(&scfft,FFT_NUM,0,1);  // ��ʼ��scfft�ṹ�壬�趨FFT��ز���
//	
//				for(i=0;i<FFT_NUM;i++)//�����ź�����
//			{
//				/*ʵ��*/
//				 FFT_input[2*i]=1+ //ֱ������ 10
//				                   1*arm_sin_f32(2*PI*10*i/FFT_NUM)+ //����Ϊ1,Ƶ��Ϊ1hz�ĳɷ�
//								   3*arm_sin_f32(2*PI*i*40/FFT_NUM)+ //����Ϊ3,Ƶ��Ϊ4hz�ĳɷ�
//				                   5*arm_cos_f32(2*PI*i*80/FFT_NUM);	//����Ϊ5,Ƶ��Ϊ8hz�ĳɷ�
//				//�鲿ȫ��Ϊ0 
//				FFT_input[2*i+1]=0;
//			}
//			arm_cfft_radix4_f32(&scfft,FFT_input);  //���и���Ҷ�任
//			
//			arm_cmplx_mag_f32(FFT_input,FFT_MO_output,FFT_NUM);  //����ģֵ
