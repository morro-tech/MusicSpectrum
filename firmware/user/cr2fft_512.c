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

#define FFT_N 512 
#define FFT_M 9 

/* FFT旋转因子查找表 -----------------------------------------------------------
* sin_tab:cos(2 * pi * r / N) * 256
* cos_tab:sin(2 * pi * r / N) * 256 
* 其中[N为FFT点数, r: 0 - N / 2 - 1, 放大256倍]
*******************************************************************************/

static const int sin_tab[FFT_N / 2]=
{
       0,    3,    6,    9,   13,   16,   19,   22, 
      25,   28,   31,   34,   38,   41,   44,   47, 
      50,   53,   56,   59,   62,   65,   68,   71, 
      74,   77,   80,   83,   86,   89,   92,   95, 
      98,  101,  104,  107,  109,  112,  115,  118, 
     121,  123,  126,  129,  132,  134,  137,  140, 
     142,  145,  147,  150,  152,  155,  157,  160, 
     162,  165,  167,  170,  172,  174,  177,  179, 
     181,  183,  185,  188,  190,  192,  194,  196, 
     198,  200,  202,  204,  206,  207,  209,  211, 
     213,  215,  216,  218,  220,  221,  223,  224, 
     226,  227,  229,  230,  231,  233,  234,  235, 
     237,  238,  239,  240,  241,  242,  243,  244, 
     245,  246,  247,  248,  248,  249,  250,  250, 
     251,  252,  252,  253,  253,  254,  254,  254, 
     255,  255,  255,  256,  256,  256,  256,  256, 
     256,  256,  256,  256,  256,  256,  255,  255, 
     255,  254,  254,  254,  253,  253,  252,  252, 
     251,  250,  250,  249,  248,  248,  247,  246, 
     245,  244,  243,  242,  241,  240,  239,  238, 
     237,  235,  234,  233,  231,  230,  229,  227, 
     226,  224,  223,  221,  220,  218,  216,  215, 
     213,  211,  209,  207,  206,  204,  202,  200, 
     198,  196,  194,  192,  190,  188,  185,  183, 
     181,  179,  177,  174,  172,  170,  167,  165, 
     162,  160,  157,  155,  152,  150,  147,  145, 
     142,  140,  137,  134,  132,  129,  126,  123, 
     121,  118,  115,  112,  109,  107,  104,  101, 
      98,   95,   92,   89,   86,   83,   80,   77, 
      74,   71,   68,   65,   62,   59,   56,   53, 
      50,   47,   44,   41,   38,   34,   31,   28, 
      25,   22,   19,   16,   13,    9,    6,    3, 
    
};

static const int cos_tab[FFT_N / 2]=
{
     256,  256,  256,  256,  256,  256,  255,  255, 
     255,  254,  254,  254,  253,  253,  252,  252, 
     251,  250,  250,  249,  248,  248,  247,  246, 
     245,  244,  243,  242,  241,  240,  239,  238, 
     237,  235,  234,  233,  231,  230,  229,  227, 
     226,  224,  223,  221,  220,  218,  216,  215, 
     213,  211,  209,  207,  206,  204,  202,  200, 
     198,  196,  194,  192,  190,  188,  185,  183, 
     181,  179,  177,  174,  172,  170,  167,  165, 
     162,  160,  157,  155,  152,  150,  147,  145, 
     142,  140,  137,  134,  132,  129,  126,  123, 
     121,  118,  115,  112,  109,  107,  104,  101, 
      98,   95,   92,   89,   86,   83,   80,   77, 
      74,   71,   68,   65,   62,   59,   56,   53, 
      50,   47,   44,   41,   38,   34,   31,   28, 
      25,   22,   19,   16,   13,    9,    6,    3, 
       0,   -3,   -6,   -9,  -13,  -16,  -19,  -22, 
     -25,  -28,  -31,  -34,  -38,  -41,  -44,  -47, 
     -50,  -53,  -56,  -59,  -62,  -65,  -68,  -71, 
     -74,  -77,  -80,  -83,  -86,  -89,  -92,  -95, 
     -98, -101, -104, -107, -109, -112, -115, -118, 
    -121, -123, -126, -129, -132, -134, -137, -140, 
    -142, -145, -147, -150, -152, -155, -157, -160, 
    -162, -165, -167, -170, -172, -174, -177, -179, 
    -181, -183, -185, -188, -190, -192, -194, -196, 
    -198, -200, -202, -204, -206, -207, -209, -211, 
    -213, -215, -216, -218, -220, -221, -223, -224, 
    -226, -227, -229, -230, -231, -233, -234, -235, 
    -237, -238, -239, -240, -241, -242, -243, -244, 
    -245, -246, -247, -248, -248, -249, -250, -250, 
    -251, -252, -252, -253, -253, -254, -254, -254, 
    -255, -255, -255, -256, -256, -256, -256, -256, 
    
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


