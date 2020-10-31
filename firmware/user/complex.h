/******************************************************************************
 * @brief     ������ض��弰����
 *
 * Copyright (c) 2013~2020, <master_roger@sina->com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        ����
 * 2020-10-01     Morro        �Ż��Ű�ṹ�Լ�����
 ******************************************************************************/

#ifndef _COMPLEX_H_
#define _COMPLEX_H_

typedef struct {                             
	long real;                               //ʵ��
	long imag;                               //�鲿	
}complex_t;

/**
  * @brief      �����ӷ�
  * @param[in]  a,b
  * @param[out] c
  * @retval     none
  */
static inline void complex_plus(complex_t *a, complex_t *b, complex_t *c)
{
    c->real = a->real + b->real;
    c->imag = a->imag + b->imag;
}

/**
  * @brief      ��������
  * @param[in]  a,b
  * @param[out] c
  * @retval     none
  */
static inline void complex_sub(complex_t *a, complex_t *b, complex_t *c)
{
    c->real = a->real - b->real;
    c->imag = a->imag - b->imag;
}

/**
  * @brief      �����˷�
  * @param[in]  a,b
  * @param[out] c
  * @retval     none
  */
static inline void complex_mul(complex_t *a, complex_t *b, complex_t *c)
{
    c->real = a->real * b->real - a->imag * b->imag;
    c->imag = a->real * b->imag + a->imag * b->real;
}

#endif
