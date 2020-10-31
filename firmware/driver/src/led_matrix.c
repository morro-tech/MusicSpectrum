/******************************************************************************
 * @brief    ˫ɫ��������
 *
 * Copyright (c) 2013~2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        ����
 * 2016-02-27     Morro        ��ģ�黯��ʽ��д
 * 2020-09-20     Morro        �Ż��Ű�ṹ�Լ�����
 ******************************************************************************/
#include "public.h"
#include "module.h"
#include "led_matrix.h"
#include <string.h>

/*LED �Դ�*/
unsigned char vmem[LED_MATRIX_HEIGHT][LED_MATRIX_WIDTH];

/**
  * @brief  ��ʱ��3����
  * @param  None
  * @retval None
  */
static void timer_config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    timer_conf(TIM3, 350);                              //ˢ��Ƶ��:(350 / 16)hz
    
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE ); 
    
	TIM_Cmd(TIM3, ENABLE);
    
    nvic_conf(TIM3_IRQn, 1, 0);
}

/**
  * @brief  ����
  */
void LedMatrix_Clear(void)
{
    memset(vmem,0,sizeof(vmem));
}

/**
  * @brief  ����
  * @param  x,y   - ����
  * @param  color - ����ɫ
  * @retval None
  */
void inline LedMatrix_DrawPoint(int x,int y, int color)
{
    vmem[y][x] = color;
}

/**
  * @brief  ����ֱ��
  * @param  None
  * @retval None
  */
void LedMatrix_DrawVLine(int x, int y1, int y2, int color)
{
    for (; y1 <= y2; y1++)
        vmem[y1][x] = color;
}

/**
  * @brief  ��ˮƽ��
  * @param  None
  * @retval None
  */
void LedMatrix_DrawHLine(int y, int x1, int x2, int color)
{
    for (; x1 <= x2; x1++)
        vmem[y][x1] = color;
}
/**
  * @brief  ledɨ��
  *         
  * @param  None
  * @retval None
  */
void LedMatrix_Scan(void)
{
    static int row = 0;
    int  line;                                          /*�м��� */
    for (line = 0; line < LED_MATRIX_WIDTH; line++) {   /*80����ʾ�� */    
        /*
         * 	����ת�����������͵������ϰ���
         */
        /*�ϰ����̵�*/
        MatrixG1 = (vmem[row][line] & LED_COLOR_GREEN) ? 0 : 1; 
        /*�°����̵�*/
        MatrixG2 = (vmem[row + 16][line] & LED_COLOR_GREEN) ? 0 : 1;

        /*�ϰ������*/
        MatrixR1 = (vmem[row][line] & LED_COLOR_RED) ? 0 : 1;
        /*�°������*/
        MatrixR2 = (vmem[row + 16][line] & LED_COLOR_RED) ? 0 : 1;
        MatrixCLK = 0;
        MatrixCLK = 1;//�������ͳ�����
    } 
    MatrixSTB = 0;//
    MatrixSTB = 1;//����������һ������	
    
    GPIOB->ODR = (GPIOB->ODR & 0x0FFF) | (row++ << 12);//��ʾ��ǰ��
    
    row &= 0xF;
}

/*��ʱ��3�жϷ������,����ˢ��LED,ÿ��ˢ������,��32��*/
void TIM3_IRQHandler(void)
{	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    LedMatrix_Scan();
}

/**
  * @brief  �˵�����
  * @param  None
  * @retval None
  */
void LedMatrix_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
    /*
		GPIOA.6->R1,GPIOA.7->G1;�������ģʽ	
	*/
	GPIOA->CRL&=0X00FFFFFF;GPIOA->CRL|=0X33000000;

	/*
		GPIOB.0->R2,GPIOB.1->G2,GPIOB.2->CLK
		GPIOB.10->STB,GPIOB.11->OE,;�������ģʽ	
	*/
	GPIOB->CRL&=0XFFFFF000;GPIOB->CRL|=0X00000333;
	GPIOB->CRH&=0X00000000;GPIOB->CRH|=0X33333333;	
        
    MatrixOE=1;//�رյ���
	MatrixR1=1;
	MatrixR2=1;
	MatrixG1 = 1;
	MatrixG2 = 1;
	MatrixCLK = 1;
	MatrixSTB = 1;
	MatrixOE=0;//�򿪵���
    timer_config();
} driver_init("led", LedMatrix_Init);

