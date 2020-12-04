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
#ifndef _AUDIO_SAMPLE_H_
#define _AUDIO_SAMPLE_H_

#include <stdbool.h>

void audio_sample_init(unsigned short *base, int size, unsigned int rate);

void audio_sample_start(void);

bool audio_sample_completed(void);

#endif

