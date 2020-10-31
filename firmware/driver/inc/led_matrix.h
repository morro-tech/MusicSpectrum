/******************************************************************************
 * @brief    双色点阵驱动
 *
 * Copyright (c) 2013~2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2012-12-23     Morro        初版
 * 2016-02-27     Morro        按模块化方式重写
 * 2020-06-13     Morro        优化排版结构以及描述
 ******************************************************************************/


#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

#include "stm32f10x_bitmap.h"

#define LED_MATRIX_WIDTH              80        //点阵宽度
#define LED_MATRIX_HEIGHT             32        //点阵高度

#define MatrixR1	                 GPIOAW(6)  //上半屏红点数据口
#define MatrixG1	                 GPIOAW(7)  //上半屏绿点数据口

#define MatrixR2	                 GPIOBW(0)  //下半屏红点数据口
#define MatrixG2	                 GPIOBW(1)  //下半屏绿点数据口

#define MatrixCLK	                 GPIOBW(2)  //点阵时钟驱动引脚
#define MatrixSTB	                 GPIOBW(10) //点阵数据锁存引脚
#define MatrixOE	                 GPIOBW(11) //点阵数据使能端


/*颜色定义 --------------------------------------------------------------------*/
#define LED_COLOR_BLACK               0x00
#define LED_COLOR_RED                 0x01
#define LED_COLOR_GREEN               0x02
#define LED_COLOR_YELLOW              0x03

void LedMatrix_Clear(void);
void LedMatrix_DrawPoint(int x,int y, int color);
void LedMatrix_DrawVLine(int x, int y1, int y2, int color);
void LedMatrix_DrawHLine(int y, int x1, int x2, int color);

#endif
