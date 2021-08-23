/*
 * @Description: 
 * @Author: yzy
 * @Date: 2020-05-27 22:52:59
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-08-17 21:19:33
 * @FilePath: \hal\BSP\bsp_uart.c
 */ 

#include "bsp_uart.h"
#include "stdio.h"

uint16_t USART_RX_STA = 0;       //接收状态标记
//uint8_t aUART_RX_BUF[UART_REC_LEN];
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
uint8_t aRxBuffer[RXBUFFERSIZE];    //HAL库使用的串口接收缓冲


UART_HandleTypeDef UART_HandleStruct;

/**
 * @description: uart初始化函数
 * @param {BaudRate : 串口波特率} 
 * @return:无 
 */
void bsp_uart_init(uint32_t BaudRate)
{

    UART_HandleStruct.Instance = USART1;
    UART_HandleStruct.Init.BaudRate = BaudRate;
    UART_HandleStruct.Init.StopBits = UART_STOPBITS_1;
    UART_HandleStruct.Init.WordLength = UART_WORDLENGTH_8B;
    UART_HandleStruct.Init.Parity = UART_PARITY_NONE;
    UART_HandleStruct.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART_HandleStruct.Init.Mode = UART_MODE_TX_RX;

    HAL_UART_Init(&UART_HandleStruct);

    HAL_UART_Receive_IT(&UART_HandleStruct, (uint8_t *)aRxBuffer, RXBUFFERSIZE);     //该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量


}

/*
 * @description: 用于初始化单片机资源. 列如初始化IO口  打开时钟  此函数会被HAL_UART_Init()调用
 * @param {huart 具体的串口}
 * @return 
 */

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    if(huart->Instance == USART1) {

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_AFIO_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        HAL_NVIC_EnableIRQ(USART1_IRQn);
        HAL_NVIC_SetPriority(USART1_IRQn, 3, 3);


    }

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1) {       //如果是串口1

        if((USART_RX_STA&0x8000)==0) {   //接收未完成
        
            if(USART_RX_STA&0x4000) {    //接收到了0x0d
            
                if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
                else USART_RX_STA|=0x8000;	//接收完成了 
            }
            else {                  //还没收到0X0D
            
                if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
                }		 
            }
        }

    }
}


//串口1中断服务程序
void USART1_IRQHandler(void)                	
{ 
    uint32_t timeout=0;
    
    HAL_UART_IRQHandler(&UART_HandleStruct);	//调用HAL库中断处理公用函数

    timeout=0;
    while (HAL_UART_GetState(&UART_HandleStruct) != HAL_UART_STATE_READY)//等待就绪
    {
     timeout++;     //超时处理
     if(timeout>HAL_MAX_DELAY) break;		
    
    }
     
    timeout=0;
    while(HAL_UART_Receive_IT(&UART_HandleStruct, aRxBuffer, RXBUFFERSIZE) != HAL_OK) //一次处理完成之后，重新开启中断并设置RxXferCount为1
    {
     timeout++; //超时处理
     if(timeout>HAL_MAX_DELAY) break;	
    }
} 


 
//gcc下的配置方式
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif 

PUTCHAR_PROTOTYPE 
{

HAL_UART_Transmit(&UART_HandleStruct, (uint8_t *)&ch, 1, 0xFFFF);
return ch;
}

