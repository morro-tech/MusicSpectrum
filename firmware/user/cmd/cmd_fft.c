/******************************************************************************
 * @brief    设备相关命令
 *
 * Copyright (c) 2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2020/10/11     Morro        
 ******************************************************************************/
#include "cli.h"
#include "cr2fft.h"
#include "module.h"
#include "config.h"
#include "public.h"
#include <stdio.h>
#include <math.h>

#define FFT_NPT    256

#define PI         3.1415926535898
#define Fs         5120               //测试抽样频率

static complex_t sample_data[FFT_NPT];


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

/* 
 * @brief       显示系统信息
 */ 
static int do_fft_test(struct cli_obj *o, int argc, char *argv[])
{
    int ret, i;    
    unsigned int tick;
    float signal;
    long long real, image;
    for(i=0; i< FFT_NPT; i++) {
        
        signal = 1234  + 350 * sin(2*PI*200 * i/Fs)+ 
                         600 * sin(2*PI*50  * i/Fs)+
                         250 * sin(2*PI*90  * i/Fs);
        sample_data[i].real = (int)signal;
        sample_data[i].imag = 0;
    }     
    tick = get_tick();
    cr2_fft(sample_data, 512);
    printf("FFT time:%d ms\r\n", get_tick() - tick);
    tick = get_tick();
    for (i = 0; i < 32; i++) {
        real  = sample_data[i].real;
        image = sample_data[i].imag;
        ret = (int)(1.0 / InvSqrt(real * real + image * image )) / FFT_NPT;  
        
        if (i == 0)
            ret /= 2;
        printf("%d:%d\r\n", i, ret);
    }    
    return 0;
}cmd_register("fft", do_fft_test, "fft test program.");


