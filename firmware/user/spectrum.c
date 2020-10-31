/******************************************************************************
 * @brief    音频频谱
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
#include "spectrum.h"
#include "audio_sample.h"
#include "module.h"
#include "cr2fft.h"
#include "public.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
 

/*信号处理 -------------------------------------------------------------------*/
typedef struct {
    int sample_rate_hz;                       //抽样频率(hz)    
    unsigned short audio_data[NPT * 3];       //音频数据
    complex_t      input[NPT];
}signal_handler_t;



static signal_handler_t sig = {
    .sample_rate_hz = SAMPLE_RATE          //默认采样率
};


static spectrum_view_t view =  {
    .buoy_hover_time  = 120,               //浮标悬停时间  
    .buoy_fall_time   = 30, 
    .buoy_color       = LED_COLOR_RED, 
    .bar_rise_time    = 10, 
    .bar_fall_time    = 20,  
    .bar_color        = LED_COLOR_GREEN
}; 

static void spectrum_view_render(void);

/* 
 * @brief       显示开机LOGO(抽样曲线)
 * @return      none
 */
static void show_logo(spectrum_view_t *v)
{
    unsigned int startup_delay = get_tick();               //logo显示延时
    int i;  
    
    v->buoy_color = LED_COLOR_GREEN;
    v->bar_color  = LED_COLOR_YELLOW;
    for (i = 0; i < LED_MATRIX_WIDTH; i++) {
        if (i == LED_MATRIX_WIDTH / 2)
            v->power[i] = LED_MATRIX_HEIGHT;
        else 
            v->power[i] =( (int)(16 * (sin((i - 40)  * 0.45) / ((i - 40) * 0.45))  + 16));   
    }

    v->buoy_color = LED_COLOR_RED;
    v->bar_color  = LED_COLOR_GREEN;
    spectrum_view_render();
    while (get_tick() - startup_delay < 2000) {}  
    
}

//频谱分析初始化
static void spectrum_init(void)
{
    show_logo(&view);
    //3倍采样,后续会每3点做平均
    audio_sample_init(sig.audio_data, NPT * 3, sig.sample_rate_hz * 3);
    audio_sample_start();
    
}

//开方快速算法
static float InvSqrt(float x)
{
    float xhalf = 0.5f*x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);        // 计算第一个近似根
    x = *(float*)&i;
    x = x*(1.5f - xhalf*x*x);         // 牛顿迭代法
    return x;
}

//计算功率谱(P = sqrt(r * r + i * i) / NPT)
static void calc_power(spectrum_view_t *v, complex_t *input)
{
    int i, j, result;
    long long real, image;
    //P = sqrt(r * r + i * i) / NPT
    for(i = 0, j = 1; i < LED_MATRIX_WIDTH; i++) {
        j += 2;
        //j = (int)(i  * 0.01875 * i + i) + 1;//对频谱进行取样        
        real  = input[j].real;
        image = input[j].imag;
        result = (int)(1.0 / InvSqrt(real * real + image * image ));  
        
        result /= NPT  / 2.0;

        if (result > LED_MATRIX_HEIGHT)       //满屏处理
            result = LED_MATRIX_HEIGHT;
        
        v->power[i] = result;
    }    
}


/* 
 * @brief       频谱处理任务
 * @return      none
 */
static void spectrum_process(void)
{
    
    int i;
    if(audio_sample_completed())   {	//采样完成          

        for (i = 0; i < NPT; i++) {
            sig.input[i].real = (sig.audio_data[3 * i] + 
                                 sig.audio_data[3 * i + 1] + 
                                 sig.audio_data[3 * i + 2]) / 3;
            sig.input[i].imag = 0;
        }
        
        audio_sample_start();          //启动下一次采样
      
        cr2_fft(sig.input, NPT);

        calc_power(&view, sig.input);  //计算功率谱
    }
}


/**
  * @brief  频谱视图渲染
  * @param  none
  * @retval none
  */
static void spectrum_view_render(void)
{   
    int  i, j;
    spectrum_view_t *v = &view;
    int         power;
    bar_obj_t  *bar;
    buoy_obj_t *buoy;

    for (i = 0; i < LED_MATRIX_WIDTH; i++) {
        bar  = &v->bar[i];
        buoy = &v->buoy[i];
        power = v->power[i];
        
        //谱柱管理
        if (power != bar->value) {                /*谱能量值与当前柱高度不一致*/
            if (power < bar->value) {             /*下落时间到*/ 
                if (is_timeout(bar->fall_timer, v->bar_fall_time)) {
                    bar->value--;
                    bar->fall_timer = get_tick();
                }
            } else {
                bar->value = power;
            }
        }

        //浮标管理
        if (bar->value != buoy->value){
            if (bar->value > buoy->value) {       
                buoy->value       = bar->value;   /*确保浮标在最上面*/
                buoy->hover_timer = get_tick();   /*开始悬停计时*/
            }
            if (is_timeout(buoy->hover_timer, v->buoy_hover_time)) {  //悬停超时
                if (is_timeout(buoy->fall_timer, v->buoy_fall_time)) {
                    buoy->fall_timer = get_tick();
                    buoy->value--;
                }
            }
        }
        
        //绘制谱柱
        for (j = 0; j < LED_MATRIX_HEIGHT - bar->value; j++)
            LedMatrix_DrawPoint(i, j, LED_COLOR_BLACK);
        
        for (; j < LED_MATRIX_HEIGHT; j++)
            LedMatrix_DrawPoint(i, j, v->bar_color); 
        
        if (buoy->value == 0)                       /*浮标显示最底下*/
            buoy->value =1;
        
        //绘制浮标
        LedMatrix_DrawPoint(i, LED_MATRIX_HEIGHT - buoy->value, v->buoy_color);        
    }
}
/**
  * @brief      设置频谱显示参数
  * @param[in]  v - 视图参数
  * @retval     none
  */
void spectrum_config_set(spectrum_view_t *v)
{
    view = *v;
}

/**
  * @brief      获取频谱显示参数
  * @param[out] v - 视图参数
  * @retval     none
  */
void spectrum_config_get(spectrum_view_t *v)
{
    *v = view;
}

module_init("logo", spectrum_init);
task_register("spectrum_task", spectrum_process, 0);
task_register("spectrum_rende",spectrum_view_render, 5);
