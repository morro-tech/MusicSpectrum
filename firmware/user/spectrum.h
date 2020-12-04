/******************************************************************************
 * @brief    音频频谱显示
 *
 * Copyright (c) 2013~2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Chovere Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        初版
 * 2016-02-27     Morro        按模块化方式重写
 * 2020-09-20     Morro        优化排版结构以及描述
 ******************************************************************************/
#ifndef _SPECTRUM_H_
#define _SPECTRUM_H_

#include "led_matrix.h"

#define NPT                    256            //采样点数

#define SAMPLE_RATE            15000          //默认抽样频率

/*柱条*/
typedef struct {    
    unsigned int rise_timer;                 //上升定时器
    unsigned int fall_timer;                 //下落定时器  
    int          value;
}bar_obj_t;

/*浮标*/
typedef struct {
    unsigned int hover_timer;               //悬停定时器
    unsigned int fall_timer;                //下落定时器
    int          value;
}buoy_obj_t;


/*频谱显示管理*/
typedef struct {
    unsigned int bar_rise_time;             //谱柱上升时间(ms)
    unsigned int bar_fall_time;             //谱柱下落时间(ms)
    int          bar_color;                 //谱柱显示颜色 
    
    unsigned int buoy_hover_time;           //浮标悬停时间(ms)
    unsigned int buoy_fall_time;            //浮标下落时间(ms)
    int          buoy_color;                //显示颜色    
    
    bar_obj_t    bar[LED_MATRIX_WIDTH];     //谱柱高度
    buoy_obj_t   buoy[LED_MATRIX_WIDTH];    //浮标高度

    unsigned char power[LED_MATRIX_WIDTH]; //频谱能量值
}spectrum_view_t;

void spectrum_config_set(spectrum_view_t *);
void spectrum_config_get(spectrum_view_t *);

#endif
