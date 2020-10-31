/******************************************************************************
 * @brief    双色点阵驱动
 *
 * Copyright (c) 2013~2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2013-05-04     Morro        初版
 * 2016-02-27     Morro        按模块化方式重写
 * 2020-09-20     Morro        优化排版结构以及描述
 ******************************************************************************/
#include "public.h"
#include "module.h"
#include "led_matrix.h"
#include <string.h>

/*LED 显存*/
unsigned char vmem[LED_MATRIX_HEIGHT][LED_MATRIX_WIDTH];

/**
  * @brief  定时器3配置
  * @param  None
  * @retval None
  */
static void timer_config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    timer_conf(TIM3, 350);                              //刷新频率:(350 / 16)hz
    
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE ); 
    
	TIM_Cmd(TIM3, ENABLE);
    
    nvic_conf(TIM3_IRQn, 1, 0);
}

/**
  * @brief  清屏
  */
void LedMatrix_Clear(void)
{
    memset(vmem,0,sizeof(vmem));
}

/**
  * @brief  画点
  * @param  x,y   - 坐标
  * @param  color - 点颜色
  * @retval None
  */
void inline LedMatrix_DrawPoint(int x,int y, int color)
{
    vmem[y][x] = color;
}

/**
  * @brief  画垂直线
  * @param  None
  * @retval None
  */
void LedMatrix_DrawVLine(int x, int y1, int y2, int color)
{
    for (; y1 <= y2; y1++)
        vmem[y1][x] = color;
}

/**
  * @brief  画水平线
  * @param  None
  * @retval None
  */
void LedMatrix_DrawHLine(int y, int x1, int x2, int color)
{
    for (; x1 <= x2; x1++)
        vmem[y][x1] = color;
}
/**
  * @brief  led扫描
  *         
  * @param  None
  * @retval None
  */
void LedMatrix_Scan(void)
{
    static int row = 0;
    int  line;                                          /*列计数 */
    for (line = 0; line < LED_MATRIX_WIDTH; line++) {   /*80列显示点 */    
        /*
         * 	并串转换，将数据送到点阵上半屏
         */
        /*上半屏绿点*/
        MatrixG1 = (vmem[row][line] & LED_COLOR_GREEN) ? 0 : 1; 
        /*下半屏绿点*/
        MatrixG2 = (vmem[row + 16][line] & LED_COLOR_GREEN) ? 0 : 1;

        /*上半屏红点*/
        MatrixR1 = (vmem[row][line] & LED_COLOR_RED) ? 0 : 1;
        /*下半屏红点*/
        MatrixR2 = (vmem[row + 16][line] & LED_COLOR_RED) ? 0 : 1;
        MatrixCLK = 0;
        MatrixCLK = 1;//上升沿送出数据
    } 
    MatrixSTB = 0;//
    MatrixSTB = 1;//上升沿锁存一行数据	
    
    GPIOB->ODR = (GPIOB->ODR & 0x0FFF) | (row++ << 12);//显示当前行
    
    row &= 0xF;
}

/*定时器3中断服务程序,用于刷新LED,每次刷新两行,共32行*/
void TIM3_IRQHandler(void)
{	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
    LedMatrix_Scan();
}

/**
  * @brief  端点配置
  * @param  None
  * @retval None
  */
void LedMatrix_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
    /*
		GPIOA.6->R1,GPIOA.7->G1;推挽输出模式	
	*/
	GPIOA->CRL&=0X00FFFFFF;GPIOA->CRL|=0X33000000;

	/*
		GPIOB.0->R2,GPIOB.1->G2,GPIOB.2->CLK
		GPIOB.10->STB,GPIOB.11->OE,;推挽输出模式	
	*/
	GPIOB->CRL&=0XFFFFF000;GPIOB->CRL|=0X00000333;
	GPIOB->CRH&=0X00000000;GPIOB->CRH|=0X33333333;	
        
    MatrixOE=1;//关闭点阵
	MatrixR1=1;
	MatrixR2=1;
	MatrixG1 = 1;
	MatrixG2 = 1;
	MatrixCLK = 1;
	MatrixSTB = 1;
	MatrixOE=0;//打开点阵
    timer_config();
} driver_init("led", LedMatrix_Init);

