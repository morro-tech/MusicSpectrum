/******************************************************************************
 * @brief    tty串口打印驱动
 *
 * Copyright (c) 2020, <master_roger@sina.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs: 
 * Date           Author       Notes 
 * 2015-07-03     Morro
 ******************************************************************************/
#include "ringbuffer.h"
#include "tty.h"
#include "public.h"
#include <string.h>

#if (TTY_RXBUF_SIZE & (TTY_RXBUF_SIZE - 1)) != 0 
    #error "TTY_RXBUF_SIZE must be power of 2!"
#endif

#if (TTY_TXBUF_SIZE & (TTY_TXBUF_SIZE - 1)) != 0 
    #error "TTY_RXBUF_SIZE must be power of 2!"
#endif


static unsigned char rxbuf[TTY_RXBUF_SIZE];         /*接收缓冲区 */
static unsigned char txbuf[TTY_TXBUF_SIZE];         /*发送缓冲区 */
static ring_buf_t rbsend, rbrecv;                   /*收发缓冲区管理*/

/*
 * @brief	    串口初始化
 * @param[in]   baudrate - 波特率
 * @return 	    none
 */
static void uart_init(int baudrate)
{
    ring_buf_init(&rbsend, txbuf, sizeof(txbuf));/*初始化环形缓冲区 */
    ring_buf_init(&rbrecv, rxbuf, sizeof(rxbuf)); 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
    
    gpio_conf(GPIOA, GPIO_Mode_AF_PP, GPIO_Pin_9 | GPIO_Pin_10);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    uart_conf(USART1, baudrate);                    /*串口配置*/
    
    nvic_conf(USART1_IRQn, 1, 1);

}

/*
 * @brief	    向串口发送缓冲区内写入数据并启动发送
 * @param[in]   buf       -  数据缓存
 * @param[in]   len       -  数据长度
 * @return 	    实际写入长度(如果此时缓冲区满,则返回len)
 */
static unsigned int uart_write(const void *buf, unsigned int len)
{   
    unsigned int ret;
    ret = ring_buf_put(&rbsend, (unsigned char *)buf, len);  
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    return ret; 
}

/*
 * @brief	    读取串口接收缓冲区的数据
 * @param[in]   buf       -  数据缓存
 * @param[in]   len       -  数据长度
 * @return 	    (实际读取长度)如果接收缓冲区的有效数据大于len则返回len否则返回缓冲
 *              区有效数据的长度
 */
static unsigned int uart_read(void *buf, unsigned int len)
{
    return ring_buf_get(&rbrecv, (unsigned char *)buf, len);
}

/*发送缓冲区满*/
static bool tx_isfull(void)
{
    return ring_buf_len(&rbsend) == TTY_TXBUF_SIZE;
}

/*接收缓冲区空*/
bool rx_isempty(void)
{
    return ring_buf_len(&rbrecv) == 0;
}

/*控制台接口定义 -------------------------------------------------------------*/
const tty_t tty = {
    uart_init,
    uart_write,
    uart_read,
    tx_isfull,
    rx_isempty
};

/*
 * @brief	    串口1收发中断
 * @param[in]   none
 * @return 	    none
 */
void USART1_IRQHandler(void)
{     
    unsigned char data;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { /*数据接收处理*/
        data = USART_ReceiveData(USART1);
        ring_buf_put(&rbrecv, &data, 1);                   
    }
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) {  /*数据发送处理*/
        if (ring_buf_get(&rbsend, &data, 1))    
            USART_SendData(USART1, data);            
        else{
            USART_ITConfig(USART1, USART_IT_TXE, DISABLE);    
        }
    }
    if (USART_GetITStatus(USART1, USART_IT_ORE) != RESET) {
        data = USART_ReceiveData(USART1);        
    }
}
