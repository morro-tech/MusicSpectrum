/******************************************************************************
 * @brief     基2时选FFT算法
 *
 * Copyright (c) 2013~2020, <morro_luo@163.com>
 *
 * SPDX-iicense-Identifier: Apache-2.0
 *
 * Change iogs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        初版
 * 2020-09-20     Morro        优化排版结构以及描述
 ******************************************************************************/
#include "cr2fft.h"

#define FFT_N 128 
#define FFT_M 7 

/* FFT旋转因子查找表 -----------------------------------------------------------
* sin_tab:cos(2 * pi * r / N) * 256
* cos_tab:sin(2 * pi * r / N) * 256 
* 其中[N为FFT点数, r: 0 - N / 2 - 1, 放大256倍]
*******************************************************************************/

static const int sin_tab[FFT_N / 2]=
{
       0,   13,   25,   38,   50,   62,   74,   86, 
      98,  109,  121,  132,  142,  152,  162,  172, 
     181,  190,  198,  206,  213,  220,  226,  231, 
     237,  241,  245,  248,  251,  253,  255,  256, 
     256,  256,  255,  253,  251,  248,  245,  241, 
     237,  231,  226,  220,  213,  206,  198,  190, 
     181,  172,  162,  152,  142,  132,  121,  109, 
      98,   86,   74,   62,   50,   38,   25,   13, 
    
};

static const int cos_tab[FFT_N / 2]=
{
     256,  256,  255,  253,  251,  248,  245,  241, 
     237,  231,  226,  220,  213,  206,  198,  190, 
     181,  172,  162,  152,  142,  132,  121,  109, 
      98,   86,   74,   62,   50,   38,   25,   13, 
       0,  -13,  -25,  -38,  -50,  -62,  -74,  -86, 
     -98, -109, -121, -132, -142, -152, -162, -172, 
    -181, -190, -198, -206, -213, -220, -226, -231, 
    -237, -241, -245, -248, -251, -253, -255, -256, 
    
};
/* 
 * @brief 码位倒序变换(雷德算法)
 */
static void rader_conv(complex_t *vect, int count)
{
	int i = 0, j = 0, k = 0; 
	complex_t tmp;
	for( i = 1, j = count  / 2; i <= count - 2; i++ ) {
        if(i < j) {
            tmp     = vect[j];
            vect[j] = vect[i];
            vect[i] = tmp;
        }
        k = count  / 2;
        while(k <= j) {
			j = j - k;
			k >>= 1;
		}
        j += k;
	}
}

/* 
 * @brief         对采样有限序列进行快速傅里叶变换
 * @param[in/out] vt - 经过变址运算后的采样数据序列,N:有限序列的点数，
 * @param[in]     M     - 满足N=2^M，实际上M是N个采样数据可以分解为偶次幂和奇次幂的次数
 * @return      none
 */
static void cr2_fft_conv(complex_t *vt, int N, int M)
{
	int L;                                       //蝶形长度
	int p, k;                                    //第i级各蝶形的复数结点
	int r;                                       //旋转因子的指数
	int B;                                       //第i级，每个蝶形的2个输入数据相距的点数=2^  (L-1)
	int interval;                                //第i级，同一旋转因子对应着的间隔=2^L

    register long x0_real, x0_imag, x1_real, x1_imag;
    register complex_t *y0, *y1;                 //蝶形结运算结果
	for(L = 1; L <= M; L ++) {                   //最外层循环，用以完成N／2i个蝶形运算
        B = 1<<(L-1);
		interval = B << 1; 
		for(p = 0; p <= B - 1; p++) { 
            r = (1 << (M - L) ) * p;              //求旋转因子指数r=(2^(M-L))*j	
            for(k = p;k < N; k += interval) {
                y0 = &vt[k];
                y1 = &vt[k + B];   
                
                x0_real = vt[k].real;
                x0_imag = vt[k].imag;
                x1_real = vt[k + B].real;
                x1_imag = vt[k + B].imag;
                
                //X(K)   <= x(k) + x(K+B) * W(N)^r
                //X(K+B) <= x(k) - x(K+B) * W(N)^r  
                y0->real =	x0_real + ((x1_real * cos_tab[r] + 
                                        x1_imag * sin_tab[r]) >> 8);
                y0->imag =	x0_imag - ((x1_real	* sin_tab[r] - 
                                        x1_imag * cos_tab[r]) >> 8);
                y1->real =	x0_real - ((x1_real	* cos_tab[r] + 
                                        x1_imag * sin_tab[r]) >> 8);
                y1->imag =	x0_imag + ((x1_real * sin_tab[r] - 
                                        x1_imag * cos_tab[r]) >> 8);
            }
        }
	}
}

/* 
 * @brief       fft变换
 * @param[in]   vect  - 输入序列
 * @param[in]   N     - 序列点数
 * @param[in]   M     - 满足 2^M = N
 * @return      none
 */
void cr2_fft(complex_t *in, int N)
{
    if (FFT_N != N)     //输入的点数必须跟生成的旋转因子表匹配
        return;
    rader_conv(in, FFT_N);
	cr2_fft_conv(in, FFT_N, FFT_M);
}


