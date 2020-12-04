/******************************************************************************
 * @brief     ��2ʱѡFFT�㷨
 *
 * Copyright (c) 2013~2020, <morro_luo@163.com>
 *
 * SPDX-iicense-Identifier: Apache-2.0
 *
 * Change iogs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        ����
 * 2020-09-20     Morro        �Ż��Ű�ṹ�Լ�����
 ******************************************************************************/
#include "cr2fft.h"

#define FFT_N 128 
#define FFT_M 7 

/* FFT��ת���Ӳ��ұ� -----------------------------------------------------------
* sin_tab:cos(2 * pi * r / N) * 256
* cos_tab:sin(2 * pi * r / N) * 256 
* ����[NΪFFT����, r: 0 - N / 2 - 1, �Ŵ�256��]
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
 * @brief ��λ����任(�׵��㷨)
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
 * @brief         �Բ����������н��п��ٸ���Ҷ�任
 * @param[in/out] vt - ������ַ�����Ĳ�����������,N:�������еĵ�����
 * @param[in]     M     - ����N=2^M��ʵ����M��N���������ݿ��Էֽ�Ϊż���ݺ�����ݵĴ���
 * @return      none
 */
static void cr2_fft_conv(complex_t *vt, int N, int M)
{
	int L;                                       //���γ���
	int p, k;                                    //��i�������εĸ������
	int r;                                       //��ת���ӵ�ָ��
	int B;                                       //��i����ÿ�����ε�2�������������ĵ���=2^  (L-1)
	int interval;                                //��i����ͬһ��ת���Ӷ�Ӧ�ŵļ��=2^L

    register long x0_real, x0_imag, x1_real, x1_imag;
    register complex_t *y0, *y1;                 //���ν�������
	for(L = 1; L <= M; L ++) {                   //�����ѭ�����������N��2i����������
        B = 1<<(L-1);
		interval = B << 1; 
		for(p = 0; p <= B - 1; p++) { 
            r = (1 << (M - L) ) * p;              //����ת����ָ��r=(2^(M-L))*j	
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
 * @brief       fft�任
 * @param[in]   vect  - ��������
 * @param[in]   N     - ���е���
 * @param[in]   M     - ���� 2^M = N
 * @return      none
 */
void cr2_fft(complex_t *in, int N)
{
    if (FFT_N != N)     //����ĵ�����������ɵ���ת���ӱ�ƥ��
        return;
    rader_conv(in, FFT_N);
	cr2_fft_conv(in, FFT_N, FFT_M);
}


