/******************************************************************************
 * @brief     ��2ʱѡFFT�㷨
 *
 * Copyright (c) 2013~2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        ����
 * 2020-09-20     Morro        �Ż��Ű�ṹ�Լ�����
 ******************************************************************************/

#ifndef _CR2_FFT_H_
#define _CR2_FFT_H_

/*���� -----------------------------------------------------------------------*/
typedef struct {                                 
	int real;                                    //ʵ��
	int imag;                                    //�鲿	
}complex_t;

void cr2_fft(complex_t *in, int N);

#endif
