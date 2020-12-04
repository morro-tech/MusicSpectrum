/******************************************************************************
 * @brief    ��ƵƵ����ʾ
 *
 * Copyright (c) 2013~2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Chovere Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        ����
 * 2016-02-27     Morro        ��ģ�黯��ʽ��д
 * 2020-09-20     Morro        �Ż��Ű�ṹ�Լ�����
 ******************************************************************************/
#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

#include "led_matrix.h"

#define NPT                    256            //��������

#define SAMPLE_RATE            15000          //Ĭ�ϳ���Ƶ��

/*����*/
typedef struct {    
    unsigned int rise_timer;                 //������ʱ��
    unsigned int fall_timer;                 //���䶨ʱ��  
    int          value;
}bar_obj_t;

/*����*/
typedef struct {
    unsigned int hover_timer;               //��ͣ��ʱ��
    unsigned int fall_timer;                //���䶨ʱ��
    int          value;
}buoy_obj_t;


/*Ƶ����ʾ����*/
typedef struct {
    unsigned int bar_rise_time;             //��������ʱ��(ms)
    unsigned int bar_fall_time;             //��������ʱ��(ms)
    int          bar_color;                 //������ʾ��ɫ 
    
    unsigned int buoy_hover_time;           //������ͣʱ��(ms)
    unsigned int buoy_fall_time;            //��������ʱ��(ms)
    int          buoy_color;                //��ʾ��ɫ    
    
    bar_obj_t    bar[LED_MATRIX_WIDTH];     //�����߶�
    buoy_obj_t   buoy[LED_MATRIX_WIDTH];    //����߶�

    unsigned char power[LED_MATRIX_WIDTH]; //Ƶ������ֵ
}spectrum_view_t;

void spectrum_config_set(spectrum_view_t *);
void spectrum_config_get(spectrum_view_t *);

#endif
