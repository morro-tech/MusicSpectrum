/******************************************************************************
 * @brief    音频数据采样管理
 *
 * Copyright (c) 2013~2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Chovere Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        初版
 * 2020-10-20     Morro        优化排版结构以及描述
 ******************************************************************************/
#include "public.h"
#include "audio_sample.h"

#define ADC_DMA_STREAM           DMA2_Stream0
#define AUDIO_IN_CHN             ADC_Channel_1


/*音频数据采样器 -------------------------------------------------------------*/
typedef struct {
    bool            completed;                                //本次采样完成标志
    unsigned short *base;                                     //数据基地址
    unsigned int    size;                                     //采样数据长度
    unsigned int    rate;                                     //采样频率
}audio_sampler_t;

static audio_sampler_t sampler;

/**
  * @brief  adm DMA传输配置(ADC1->ADC_DMA_STREAM, DMA_Channel_0)
  * @retval none
  */ 
static void adc_dma_config(void)
{
    DMA_InitTypeDef DMA_Structure;
    /* Enable DMA clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 

    while (DMA_GetCmdStatus(ADC_DMA_STREAM) != DISABLE){}

    DMA_Structure.DMA_Channel = DMA_Channel_0;                    
    DMA_Structure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
    DMA_Structure.DMA_Memory0BaseAddr = (uint32_t)sampler.base;
    DMA_Structure.DMA_DIR = DMA_DIR_PeripheralToMemory;           
    DMA_Structure.DMA_BufferSize = sampler.size;
    DMA_Structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Structure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_Structure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_Structure.DMA_Mode = DMA_Mode_Circular;                    
    DMA_Structure.DMA_Priority = DMA_Priority_High;
    DMA_Structure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_Structure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_Structure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_Structure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(ADC_DMA_STREAM, &DMA_Structure);
    
    DMA_ITConfig(ADC_DMA_STREAM, DMA_IT_TC, ENABLE);
    
    DMA_ClearITPendingBit(ADC_DMA_STREAM, DMA_IT_TCIF0);
    
    nvic_conf(DMA2_Stream0_IRQn, 0, 1);
    
    DMA_Cmd(ADC_DMA_STREAM, ENABLE);    
}

/*************************************************************
  * @brief  ADC配置(PA2)
  * @retval none
  ************************************************************/
static void adc_config(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    ADC_CommonInitTypeDef ADC_CommonInitStructure = {0};
    
    /* adc clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
    
    //ADC通用配置
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;        
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;     
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* ADC1 Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T5_CC2;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_RegularChannelConfig(ADC1, AUDIO_IN_CHN, 1, ADC_SampleTime_28Cycles);
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);

}

/*
 * @brief       ADC1 定时器触发(timer cc2)
 * @param[in]   rate - 触发频率(hz)
 * @return      none
 **/ 
static void adc_timer_config(int hz)
{
    RCC_ClocksTypeDef Clocks;

    float Period;  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef         TIM_OCInitStructure = {0};
    /* TIM1 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);      
    RCC_GetClocksFreq(&Clocks);    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 1;
    Period = (Clocks.PCLK1_Frequency  / hz) + 0.5;     
    TIM_TimeBaseStructure.TIM_Period = (uint32_t)Period - 1;    
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
     
    /*启用T5_CC2触发ADC转换*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 20;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;        
    TIM_OC2Init(TIM5, &TIM_OCInitStructure);     

}

/* 
 * @brief       音频采样初始化
 * @param[in]   base - 数据基地址
 * @param[in]   size - 采样数据长度
 * @param[in]   rate - 采样频率(hz)
 * @return      none
 */
void audio_sample_init(unsigned short *base, int size, unsigned int rate)
{
    sampler.base = base;
    sampler.size = size;
    
    //音频输入引脚配置
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
    gpio_conf(GPIOA, GPIO_Mode_AN, GPIO_PuPd_NOPULL, GPIO_Pin_1);
    
    adc_config();
    adc_timer_config(rate);
    adc_dma_config();

}

/* 
 * @brief       启动音频采样
 * @return      none
 */
void audio_sample_start(void)
{
    sampler.completed = false;
    
    ADC_Cmd(ADC1, ENABLE);
    TIM_Cmd(TIM5, ENABLE);    
}

/* 
 * @brief       判断是否采样完成
 * @return      bool
 */ 
bool audio_sample_completed(void)
{
    return sampler.completed;
}

/* 
 * @brief       停止音频采样
 * @return      none
 */ 
void audio_sample_stop(void)
{
    TIM_Cmd(TIM5, DISABLE);
    ADC_Cmd(ADC1, DISABLE); 
}

/**
  * @brief  DMA1 传输完成中断处理 
  * @retval none
  */
void DMA2_Stream0_IRQHandler(void)
{    
    if(DMA_GetITStatus(ADC_DMA_STREAM, DMA_IT_TCIF0)) {
        DMA_ClearITPendingBit(ADC_DMA_STREAM, DMA_IT_TCIF0);
        sampler.completed = true;
        audio_sample_stop();
    }	 
}

