/******************************************************************************
 * @brief    将位带区上的GPIO端口的某个位映射到位带别名区，实现单个位操作
 * 对于片上外设位带区的某个比特，记它所在字节的地址为 A,位序号为 n(0<=n<=7)，
 * 
 * 则该比特在别名区的地址为：AliasAddr＝ 0x42000000+((A-0x40000000)*8+n)*4  
 * 
 *  =0x42000000+ (A-0x40000000)*32 + n*4 
 * 上式中，“*4”表示一个字为 4个字节，“*8”表示一个字节中有 8个比特。 
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

/**********************位带上GPIO输出数据寄存器的地址映像**************/
#define GPIOA_ODR_Address    0x4001080C 
#define GPIOB_ODR_Address    0x40010C0C 
#define GPIOC_ODR_Address    0x4001100C 
#define GPIOD_ODR_Address    0x4001140C 
#define GPIOE_ODR_Address    0x4001180C 
#define GPIOF_ODR_Address    0x40011A0C    
#define GPIOG_ODR_Address    0x40011E0C    
/**位带上GPIO输入数据寄存器的地址映像********/
#define GPIOA_IDR_Address    0x40010808 
#define GPIOB_IDR_Address    0x40010C08 
#define GPIOC_IDR_Address    0x40011008 
#define GPIOD_IDR_Address    0x40011408 
#define GPIOE_IDR_Address    0x40011808 
#define GPIOF_IDR_Address    0x40011A08 
#define GPIOG_IDR_Address    0x40011E08 

/**位带别区上位地址映像**********************/
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

