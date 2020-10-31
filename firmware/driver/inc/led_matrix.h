/******************************************************************************
 * @brief    ˫ɫ��������
 *
 * Copyright (c) 2013~2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2012-12-23     Morro        ����
 * 2016-02-27     Morro        ��ģ�黯��ʽ��д
 * 2020-06-13     Morro        �Ż��Ű�ṹ�Լ�����
 ******************************************************************************/


#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

#include "stm32f10x_bitmap.h"

#define LED_MATRIX_WIDTH              80        //������
#define LED_MATRIX_HEIGHT             32        //����߶�

#define MatrixR1	                 GPIOAW(6)  //�ϰ���������ݿ�
#define MatrixG1	                 GPIOAW(7)  //�ϰ����̵����ݿ�

#define MatrixR2	                 GPIOBW(0)  //�°���������ݿ�
#define MatrixG2	                 GPIOBW(1)  //�°����̵����ݿ�

#define MatrixCLK	                 GPIOBW(2)  //����ʱ����������
#define MatrixSTB	                 GPIOBW(10) //����������������
#define MatrixOE	                 GPIOBW(11) //��������ʹ�ܶ�


/*��ɫ���� --------------------------------------------------------------------*/
#define LED_COLOR_BLACK               0x00
#define LED_COLOR_RED                 0x01
#define LED_COLOR_GREEN               0x02
#define LED_COLOR_YELLOW              0x03

void LedMatrix_Clear(void);
void LedMatrix_DrawPoint(int x,int y, int color);
void LedMatrix_DrawVLine(int x, int y1, int y2, int color);
void LedMatrix_DrawHLine(int y, int x1, int x2, int color);

#endif
