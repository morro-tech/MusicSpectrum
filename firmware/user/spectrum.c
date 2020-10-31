/******************************************************************************
 * @brief    ��ƵƵ��
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
#include "spectrum.h"
#include "audio_sample.h"
#include "module.h"
#include "cr2fft.h"
#include "public.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
 

/*�źŴ��� -------------------------------------------------------------------*/
typedef struct {
    int sample_rate_hz;                       //����Ƶ��(hz)    
    unsigned short audio_data[NPT * 3];       //��Ƶ����
    complex_t      input[NPT];
}signal_handler_t;



static signal_handler_t sig = {
    .sample_rate_hz = SAMPLE_RATE          //Ĭ�ϲ�����
};


static spectrum_view_t view =  {
    .buoy_hover_time  = 120,               //������ͣʱ��  
    .buoy_fall_time   = 30, 
    .buoy_color       = LED_COLOR_RED, 
    .bar_rise_time    = 10, 
    .bar_fall_time    = 20,  
    .bar_color        = LED_COLOR_GREEN
}; 

static void spectrum_view_render(void);

/* 
 * @brief       ��ʾ����LOGO(��������)
 * @return      none
 */
static void show_logo(spectrum_view_t *v)
{
    unsigned int startup_delay = get_tick();               //logo��ʾ��ʱ
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

//Ƶ�׷�����ʼ��
static void spectrum_init(void)
{
    show_logo(&view);
    //3������,������ÿ3����ƽ��
    audio_sample_init(sig.audio_data, NPT * 3, sig.sample_rate_hz * 3);
    audio_sample_start();
    
}

//���������㷨
static float InvSqrt(float x)
{
    float xhalf = 0.5f*x;
    int i = *(int*)&x;
    i = 0x5f3759df - (i >> 1);        // �����һ�����Ƹ�
    x = *(float*)&i;
    x = x*(1.5f - xhalf*x*x);         // ţ�ٵ�����
    return x;
}

//���㹦����(P = sqrt(r * r + i * i) / NPT)
static void calc_power(spectrum_view_t *v, complex_t *input)
{
    int i, j, result;
    long long real, image;
    //P = sqrt(r * r + i * i) / NPT
    for(i = 0, j = 1; i < LED_MATRIX_WIDTH; i++) {
        j += 2;
        //j = (int)(i  * 0.01875 * i + i) + 1;//��Ƶ�׽���ȡ��        
        real  = input[j].real;
        image = input[j].imag;
        result = (int)(1.0 / InvSqrt(real * real + image * image ));  
        
        result /= NPT  / 2.0;

        if (result > LED_MATRIX_HEIGHT)       //��������
            result = LED_MATRIX_HEIGHT;
        
        v->power[i] = result;
    }    
}


/* 
 * @brief       Ƶ�״�������
 * @return      none
 */
static void spectrum_process(void)
{
    
    int i;
    if(audio_sample_completed())   {	//�������          

        for (i = 0; i < NPT; i++) {
            sig.input[i].real = (sig.audio_data[3 * i] + 
                                 sig.audio_data[3 * i + 1] + 
                                 sig.audio_data[3 * i + 2]) / 3;
            sig.input[i].imag = 0;
        }
        
        audio_sample_start();          //������һ�β���
      
        cr2_fft(sig.input, NPT);

        calc_power(&view, sig.input);  //���㹦����
    }
}


/**
  * @brief  Ƶ����ͼ��Ⱦ
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
        
        //��������
        if (power != bar->value) {                /*������ֵ�뵱ǰ���߶Ȳ�һ��*/
            if (power < bar->value) {             /*����ʱ�䵽*/ 
                if (is_timeout(bar->fall_timer, v->bar_fall_time)) {
                    bar->value--;
                    bar->fall_timer = get_tick();
                }
            } else {
                bar->value = power;
            }
        }

        //�������
        if (bar->value != buoy->value){
            if (bar->value > buoy->value) {       
                buoy->value       = bar->value;   /*ȷ��������������*/
                buoy->hover_timer = get_tick();   /*��ʼ��ͣ��ʱ*/
            }
            if (is_timeout(buoy->hover_timer, v->buoy_hover_time)) {  //��ͣ��ʱ
                if (is_timeout(buoy->fall_timer, v->buoy_fall_time)) {
                    buoy->fall_timer = get_tick();
                    buoy->value--;
                }
            }
        }
        
        //��������
        for (j = 0; j < LED_MATRIX_HEIGHT - bar->value; j++)
            LedMatrix_DrawPoint(i, j, LED_COLOR_BLACK);
        
        for (; j < LED_MATRIX_HEIGHT; j++)
            LedMatrix_DrawPoint(i, j, v->bar_color); 
        
        if (buoy->value == 0)                       /*������ʾ�����*/
            buoy->value =1;
        
        //���Ƹ���
        LedMatrix_DrawPoint(i, LED_MATRIX_HEIGHT - buoy->value, v->buoy_color);        
    }
}
/**
  * @brief      ����Ƶ����ʾ����
  * @param[in]  v - ��ͼ����
  * @retval     none
  */
void spectrum_config_set(spectrum_view_t *v)
{
    view = *v;
}

/**
  * @brief      ��ȡƵ����ʾ����
  * @param[out] v - ��ͼ����
  * @retval     none
  */
void spectrum_config_get(spectrum_view_t *v)
{
    *v = view;
}

module_init("logo", spectrum_init);
task_register("spectrum_task", spectrum_process, 0);
task_register("spectrum_rende",spectrum_view_render, 5);
