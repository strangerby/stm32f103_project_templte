/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */
 
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include "main.h"
#include "bsp_uart.h"


#define _SCB_BASE       (0xE000E010UL)
#define _SYSTICK_CTRL   (*(rt_uint32_t *)(_SCB_BASE + 0x0))
#define _SYSTICK_LOAD   (*(rt_uint32_t *)(_SCB_BASE + 0x4))
#define _SYSTICK_VAL    (*(rt_uint32_t *)(_SCB_BASE + 0x8))
#define _SYSTICK_CALIB  (*(rt_uint32_t *)(_SCB_BASE + 0xC))
#define _SYSTICK_PRI    (*(rt_uint8_t  *)(0xE000ED23UL))

// Updates the variable SystemCoreClock and must be called 
// whenever the core clock is changed during program execution.
extern void SystemCoreClockUpdate(void);

// Holds the system core clock, which is the system clock 
// frequency supplied to the SysTick timer and the processor 
// core clock.
extern uint32_t SystemCoreClock;

static uint32_t _SysTick_Config(rt_uint32_t ticks)
{
    if ((ticks - 1) > 0xFFFFFF)
    {
        return 1;
    }
    
    _SYSTICK_LOAD = ticks - 1; 
    _SYSTICK_PRI = 0xFF;
    _SYSTICK_VAL  = 0;
    _SYSTICK_CTRL = 0x07;  
     
    return 0;
}

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024 * 10
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
    
    HAL_Init();
    SystemClock_Config();
    /* System Clock Update */
    SystemCoreClockUpdate();
    bsp_uart_init(115200); 
    /* System Tick Configuration */
    _SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&UART_HandleStruct, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&UART_HandleStruct, (uint8_t *)(str + i), 1, 1);
    }
}

char rt_hw_console_getchar(void)
{
    int ch = -1;

    if (__HAL_UART_GET_FLAG(&UART_HandleStruct, UART_FLAG_RXNE) != RESET)
    {
        ch = UART_HandleStruct.Instance->DR & 0xff;
    }
    else
    {
        if(__HAL_UART_GET_FLAG(&UART_HandleStruct, UART_FLAG_ORE) != RESET)
        {
            __HAL_UART_CLEAR_OREFLAG(&UART_HandleStruct);
        }
        rt_thread_mdelay(10);
    }
    return ch;
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{

    DEM_CR |= (uint32_t)DEM_CR_TRCENA;

/* DWT CYCCNT 寄存器计数清 0 */
    DWT_CYCCNT = (uint32_t)0u;

/* 使能 Cortex-M DWT CYCCNT 寄存器 */
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;

    return HAL_OK;
}

/**
* @brief 读取当前时间戳
* @param 无
* @retval 当前时间戳，即 DWT_CYCCNT 寄存器的值
*/
uint32_t CPU_TS_TmrRd(void)
{
return ((uint32_t)DWT_CYCCNT);
}

/**
* @brief 读取当前时间戳
* @param 无
* @retval 当前时间戳，即 DWT_CYCCNT 寄存器的值
*/
uint32_t HAL_GetTick(void)
{
return ((uint32_t)DWT_CYCCNT*1000/SysClockFreq);
}

/**
* @brief 采用 CPU 的内部计数实现精确延时， 32 位计数器
* @param us : 延迟长度，单位 1 us
* @retval 无
* @note 使用本函数前必须先调用 CPU_TS_TmrInit 函数使能计数器，
或使能宏 CPU_TS_INIT_IN_DELAY_FUNCTION
最大延时值为 8 秒，即 8*1000*1000
*/
void CPU_TS_Tmr_Delay_US(uint32_t us)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;

 /* 在函数内部初始化时间戳寄存器， */
    #if (CPU_TS_INIT_IN_DELAY_FUNCTION)
 /* 初始化时间戳并清零 */
    HAL_InitTick(5);
    #endif

    ticks = us * (GET_CPU_ClkFreq() / 1000000); /* 需要的节拍数 */
    tcnt = 0;
    told = (uint32_t)CPU_TS_TmrRd(); /* 刚进入时的计数器值 */

    while (1) {
        tnow = (uint32_t)CPU_TS_TmrRd();
        if (tnow != told) {
 /* 32 位计数器是递增计数器 */
            if (tnow > told) {
                tcnt += tnow - told;
            }
 /* 重新装载 */
            else {
                tcnt += UINT32_MAX - told + tnow;
            }

                told = tnow;

 /*时间超过/等于要延迟的时间,则退出 */
                if (tcnt >= ticks)break;
        }
    }
}