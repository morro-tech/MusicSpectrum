/******************************************************************************
 * @brief    ST MCU 通用外设配置
 *
 * Copyright (c) 2019, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 ******************************************************************************/
#ifndef _PUBLIC_H_
#define _PUBLIC_H_

#include "stm32f10x.h"
#include "stm32f10x_bitmap.h"

void gpio_conf(GPIO_TypeDef* GPIOx, GPIOMode_TypeDef Mode,uint32_t Pins);

void nvic_conf(int Channel, int Priority, int SubPriority);


void timer_conf(TIM_TypeDef* TIMx, unsigned int hz);

void uart_conf(USART_TypeDef *port, int baudrate);

#endif
