#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

#define __STM32_PORT(port)  GPIO##port##_BASE
#define GET_PIN(PORTx,PIN) (rt_base_t)((16 * ( ((rt_base_t)__STM32_PORT(PORTx) - (rt_base_t)GPIOA_BASE)/(0x0400UL) )) + PIN)


int rt_hw_pin_init(void);

#ifdef __cplusplus
}
#endif

#endif