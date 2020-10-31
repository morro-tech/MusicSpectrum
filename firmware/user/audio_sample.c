/******************************************************************************
 * @brief    ��Ƶ���ݲ�������
 *
 * Copyright (c) 2013~2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Chovere Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        ����
 * 2020-10-20     Morro        �Ż��Ű�ṹ�Լ�����
 ******************************************************************************/
#include "public.h"
#include "audio_sample.h"

/*��Ƶ���ݲ����� -------------------------------------------------------------*/
typedef struct {
    bool            completed;                                //���β�����ɱ�־
    unsigned short *base;                                     //���ݻ���ַ
    unsigned int    size;                                     //�������ݳ���
    unsigned int    rate;                                     //����Ƶ��
}audio_sampler_t;

static audio_sampler_t sampler;


/**
  * @brief  adm DMA��������
  * @retval none
  */
static void adc_dma_config(void)
{
    DMA_InitTypeDef DMA_InitStructure;                      //DMA�ṹ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
    /* DMAͨ��1���� ----------------------------------------------------------*/
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(sampler.base);
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = sampler.size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High ;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1,&DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC,ENABLE);
    DMA_ClearITPendingBit(DMA1_IT_TC1);
    
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

/*************************************************************
  * @brief  ADC����(PA2)
  * @retval none
  ************************************************************/
static void adc_config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);
    GPIOA ->CRL &= 0XFFFFF0FF;
    ADC_DeInit(ADC1);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;     
	ADC_Init(ADC1, &ADC_InitStructure);    
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);          //ADC��ʱ��ΪPCLK2/6 = 72Mhz / 6 = 12
    /**
     * @���ò���ʱ��Ϊ28.5������ 
     * ��߲���Ƶ��Ϊ:Tclk = 12Mhz / (28.5 + 12.5) = 292khz
     */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_13Cycles5); 
	
	ADC_DMACmd(ADC1, ENABLE);   
    /* Enable ADC1 external trigger */ 
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);
  
	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);                //��λADC1У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȵ�У׼�Ĵ�����λ���
	ADC_StartCalibration(ADC1);                //��ʼУ׼ADC1
	while(ADC_GetCalibrationStatus(ADC1));     //�ȴ�У׼���

}

/*
 * @brief       ADC1 ��ʱ������(timer 2 cc2)
 * @param[in]   rate - ����Ƶ��(hz)
 * @return      none
 **/ 
static void adc_timer_config(int hz)
{
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_OCInitTypeDef         TIM_OCInitStructure = {0};
    RCC_ClocksTypeDef         Clocks;
    float                     Period;  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_GetClocksFreq(&Clocks);   
    TIM_Cmd(TIM2, DISABLE);
    /* Time Base configuration */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Prescaler = 1;  
    Period = (Clocks.PCLK1_Frequency  / hz) + 0.5;     
    TIM_TimeBaseStructure.TIM_Period = (uint32_t)Period - 1;        
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* TIM2 channel1 configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = 0x7F; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;     
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    
    

}

/* 
 * @brief       ��Ƶ������ʼ��
 * @param[in]   base - ���ݻ���ַ
 * @param[in]   size - �������ݳ���
 * @param[in]   rate - ����Ƶ��(hz)
 * @return      none
 */
void audio_sample_init(unsigned short *base, int size, unsigned int rate)
{
    sampler.base = base;
    sampler.size = size;
    adc_config();
    adc_timer_config(rate);
    adc_dma_config();
    nvic_conf(DMA1_Channel1_IRQn, 0, 4);
    
}

/* 
 * @brief       ������Ƶ����
 * @return      none
 */
void audio_sample_start(void)
{
    sampler.completed = false;
    TIM_Cmd(TIM2, ENABLE);    
    TIM_CtrlPWMOutputs(TIM2, ENABLE);
}

/* 
 * @brief       �ж��Ƿ�������
 * @return      bool
 */ 
bool audio_sample_completed(void)
{
    return sampler.completed;
}

/* 
 * @brief       ֹͣ��Ƶ����
 * @return      none
 */ 
void audio_sample_stop(void)
{
    TIM_CtrlPWMOutputs(TIM2, DISABLE);
    TIM_Cmd(TIM2, DISABLE);  
}

/**
  * @brief  DMA1 ��������жϴ��� 
  * @retval none
  */
void DMA1_Channel1_IRQHandler(void)
{    
    if(DMA1->ISR & DMA1_IT_TC1) {
        DMA1->IFCR = DMA1_IT_TC1;
        sampler.completed = true;
        audio_sample_stop();
    }	 
}

