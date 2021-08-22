/*
 * @Description: 
 * @Author: yzy
 * @Date: 2020-05-27 22:53:05
 * @LastEditors: yzy
 * @LastEditTime: 2020-05-29 11:45:07
 * @FilePath: \hal\BSP\bsp_uart.h
 */ 
#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "main.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200

#define RXBUFFERSIZE   1 //缓存大小
extern uint8_t aRxBuffer[RXBUFFERSIZE]; //HAL库USART接收Buffer

void bsp_uart_init(uint32_t BaudRate);
extern UART_HandleTypeDef UART_HandleStruct;

#endif // !__BSP_UART_H__