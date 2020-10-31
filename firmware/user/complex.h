/******************************************************************************
 * @brief     复数相关定义及操作
 *
 * Copyright (c) 2013~2020, <master_roger@sina->com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        初版
 * 2020-10-01     Morro        优化排版结构以及描述
 ******************************************************************************/

#ifndef _COMPLEX_H_
#define _COMPLEX_H_

typedef struct {                             
	long real;                               //实部
	long imag;                               //虚部	
}complex_t;

/**
  * @brief      复数加法
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
  * @brief      复数减法
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
  * @brief      复数乘法
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
