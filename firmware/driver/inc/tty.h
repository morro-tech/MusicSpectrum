/******************************************************************************
 * @brief    tty���ڴ�ӡ����
 *
 * Copyright (c) 2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2015-07-03     Morro
 ******************************************************************************/

#ifndef	_TTY_H_
#define	_TTY_H_

#define TTY_RXBUF_SIZE		 256
#define TTY_TXBUF_SIZE		 1024

/*�ӿ����� --------------------------------------------------------------------*/
typedef struct {
    void (*init)(int baudrate);                                   
    unsigned int (*write)(const void *buf, unsigned int len);    
    unsigned int (*read)(void *buf, unsigned int len);           
    bool (*tx_isfull)(void);                                    /*���ͻ�������*/
    bool (*rx_isempty)(void);                                   /*���ջ�������*/
}tty_t;

extern const tty_t tty;

#endif
