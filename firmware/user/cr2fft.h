/******************************************************************************
 * @brief     基2时选FFT算法
 *
 * Copyright (c) 2013~2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        初版
 * 2020-09-20     Morro        优化排版结构以及描述
 ******************************************************************************/

#ifndef _CR2_FFT_H_
#define _CR2_FFT_H_

/*复数 -----------------------------------------------------------------------*/
typedef struct {                                 
	int real;                                    //实部
	int imag;                                    //虚部	
}complex_t;

void cr2_fft(complex_t *in, int N);

#endif
