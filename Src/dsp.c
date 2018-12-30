//#define FFT_NUM  2048
//#define FFT_STEP  1
//#define OLED_HIGHT 64
//#include "arm_math.h"
//float FFT_input[FFT_NUM*2];
//float FFT_MO_output[FFT_NUM];  //作为存放模值
//unsigned short i;
//arm_cfft_radix4_instance_f32 scfft;	
//
//
//
//	arm_cfft_radix4_init_f32(&scfft,FFT_NUM,0,1);  // 初始化scfft结构体，设定FFT相关参数
//	
//				for(i=0;i<FFT_NUM;i++)//生成信号序列
//			{
//				/*实部*/
//				 FFT_input[2*i]=1+ //直流分量 10
//				                   1*arm_sin_f32(2*PI*10*i/FFT_NUM)+ //幅度为1,频率为1hz的成分
//								   3*arm_sin_f32(2*PI*i*40/FFT_NUM)+ //幅度为3,频率为4hz的成分
//				                   5*arm_cos_f32(2*PI*i*80/FFT_NUM);	//幅度为5,频率为8hz的成分
//				//虚部全部为0 
//				FFT_input[2*i+1]=0;
//			}
//			arm_cfft_radix4_f32(&scfft,FFT_input);  //进行傅里叶变换
//			
//			arm_cmplx_mag_f32(FFT_input,FFT_MO_output,FFT_NUM);  //计算模值
