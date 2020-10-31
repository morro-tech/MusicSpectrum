/******************************************************************************
 * @brief    ��λ�����ϵ�GPIO�˿ڵ�ĳ��λӳ�䵽λ����������ʵ�ֵ���λ����
 * ����Ƭ������λ������ĳ�����أ����������ֽڵĵ�ַΪ A,λ���Ϊ n(0<=n<=7)��
 * 
 * ��ñ����ڱ������ĵ�ַΪ��AliasAddr�� 0x42000000+((A-0x40000000)*8+n)*4  
 * 
 *  =0x42000000+ (A-0x40000000)*32 + n*4 
 * ��ʽ�У���*4����ʾһ����Ϊ 4���ֽڣ���*8����ʾһ���ֽ����� 8�����ء� 
 *
 * Copyright (c) 2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 
 ******************************************************************************/

#ifndef _GPIOBITMAP_H_
#define _GPIOBITMAP_H_

/**********************λ����GPIO������ݼĴ����ĵ�ַӳ��**************/
#define GPIOA_ODR_Address    0x4001080C 
#define GPIOB_ODR_Address    0x40010C0C 
#define GPIOC_ODR_Address    0x4001100C 
#define GPIOD_ODR_Address    0x4001140C 
#define GPIOE_ODR_Address    0x4001180C 
#define GPIOF_ODR_Address    0x40011A0C    
#define GPIOG_ODR_Address    0x40011E0C    
/**λ����GPIO�������ݼĴ����ĵ�ַӳ��********/
#define GPIOA_IDR_Address    0x40010808 
#define GPIOB_IDR_Address    0x40010C08 
#define GPIOC_IDR_Address    0x40011008 
#define GPIOD_IDR_Address    0x40011408 
#define GPIOE_IDR_Address    0x40011808 
#define GPIOF_IDR_Address    0x40011A08 
#define GPIOG_IDR_Address    0x40011E08 

/**λ��������λ��ַӳ��**********************/
#define GPIOAW(n) *((volatile unsigned long *)(0x42000000+ (GPIOA_ODR_Address-0x40000000)*32 + (n)*4))
#define GPIOBW(n) *((volatile unsigned long *)(0x42000000+ (GPIOB_ODR_Address-0x40000000)*32 + (n)*4))
#define GPIOCW(n) *((volatile unsigned long *)(0x42000000+ (GPIOC_ODR_Address-0x40000000)*32 + (n)*4))
#define GPIODW(n) *((volatile unsigned long *)(0x42000000+ (GPIOD_ODR_Address-0x40000000)*32 + (n)*4))
#define GPIOEW(n) *((volatile unsigned long *)(0x42000000+ (GPIOE_ODR_Address-0x40000000)*32 + (n)*4))
#define GPIOFW(n) *((volatile unsigned long *)(0x42000000+ (GPIOF_ODR_Address-0x40000000)*32 + (n)*4))
#define GPIOGW(n) *((volatile unsigned long *)(0x42000000+ (GPIOG_ODR_Address-0x40000000)*32 + (n)*4))

#define GPIOAR(n) *((volatile unsigned long *)(0x42000000+ (GPIOA_IDR_Address-0x40000000)*32 + (n)*4))
#define GPIOBR(n) *((volatile unsigned long *)(0x42000000+ (GPIOB_IDR_Address-0x40000000)*32 + (n)*4))
#define GPIOCR(n) *((volatile unsigned long *)(0x42000000+ (GPIOC_IDR_Address-0x40000000)*32 + (n)*4))
#define GPIODR(n) *((volatile unsigned long *)(0x42000000+ (GPIOD_IDR_Address-0x40000000)*32 + (n)*4))
#define GPIOER(n) *((volatile unsigned long *)(0x42000000+ (GPIOE_IDR_Address-0x40000000)*32 + (n)*4))
#define GPIOFR(n) *((volatile unsigned long *)(0x42000000+ (GPIOF_IDR_Address-0x40000000)*32 + (n)*4))
#define GPIOGR(n) *((volatile unsigned long *)(0x42000000+ (GPIOG_IDR_Address-0x40000000)*32 + (n)*4))

#endif

