/******************************************************************************
 * @brief    ���������
 *
 * Copyright (c) 2020, <morro_luo@163.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2020-05-18     Morro               
 ******************************************************************************/
#include "config.h"
#include "module.h"
#include <stdio.h>
#include "public.h"


/* 
 * @brief       ��������� 
 * @return      none
 */
int main(void)
{    
    module_task_init();                         /*ģ���ʼ��*/
    while (1) {    
        module_task_process();                   /*������ѯ*/
    }
}
