/******************************************************************************
 * @brief    ˫ɫ��������
 *
 * Copyright (c) 2013~2020, <morro_luo@163.com>
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

#define SET_PIN_A(x)        GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction)(x))
#define SET_PIN_B(x)        GPIO_WriteBit(GPIOB, GPIO_Pin_1, (BitAction)(x))
#define SET_PIN_C(x)        GPIO_WriteBit(GPIOB, GPIO_Pin_2, (BitAction)(x))
#define SET_PIN_D(x)        GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(x))
//�ϰ���������ݿ�
#define SET_PIN_R1(x)       GPIO_WriteBit(GPIOB, GPIO_Pin_4, (BitAction)(x))
//�°���������ݿ�
#define SET_PIN_R2(x)       GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(x))
//�ϰ����̵����ݿ�
#define SET_PIN_G1(x)       GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
//�°����̵����ݿ�
#define SET_PIN_G2(x)       GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))
//����ʱ����������
#define SET_PIN_CLK(x)      GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)(x))
//����������������
#define SET_PIN_STB(x)      GPIO_WriteBit(GPIOB, GPIO_Pin_12, (BitAction)(x))
//��������ʹ�ܶ�
#define SET_PIN_OE(x)       GPIO_WriteBit(GPIOB, GPIO_Pin_13, (BitAction)(x))

/*LED �Դ�*/
unsigned char vmem[LED_MATRIX_HEIGHT][LED_MATRIX_WIDTH];


/**
  * @brief  �˿ڳ�ʼ��
  */
static void port_config(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
    gpio_conf(GPIOB, GPIO_Mode_OUT, GPIO_PuPd_NOPULL, 
              GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_2 | 
              GPIO_Pin_3 | GPIO_Pin_4  | GPIO_Pin_5 | 
              GPIO_Pin_6 | GPIO_Pin_7  | GPIO_Pin_8 | 
              GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12| GPIO_Pin_13);
        
    SET_PIN_OE(1);//�رյ���
	SET_PIN_R1(0);
	SET_PIN_R2(0);
	SET_PIN_G1(0);
	SET_PIN_G2(0);
	SET_PIN_CLK(0);
	SET_PIN_STB(0);
	SET_PIN_OE(0);//�򿪵��� 
    
}


/**
  * @brief  ����ɨ�趨ʱ������(timer3)
  * @param  None
  * @retval None
  */
static void timer_config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    
    timer_conf(TIM3, 300);                              //ˢ��Ƶ��:(400 / 16)hz
    
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
    for (line = 0; line < LED_MATRIX_WIDTH; line++) {   
        /*
         * 	����ת�����������͵������ϰ���
         */
        /*�ϰ����̵�*/
        SET_PIN_G1(vmem[row][line] & LED_COLOR_GREEN);

        /*�°����̵�*/
        SET_PIN_G2(vmem[row + 16][line] & LED_COLOR_GREEN);

        /*�ϰ������*/
        SET_PIN_R1(vmem[row][line] & LED_COLOR_RED);

        /*�°������*/

        SET_PIN_R2(vmem[row + 16][line] & LED_COLOR_RED);
        
        SET_PIN_CLK(0);
        SET_PIN_CLK(1);//�������ͳ�����

    } 
    SET_PIN_STB(0);
    SET_PIN_STB(1);     //����������һ������	
    SET_PIN_STB(0);
    
    GPIOB->ODR = GPIOB->ODR & 0xFFF0 | row;
    
    row++;
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
    port_config();
    timer_config();
} driver_init("led", LedMatrix_Init);

