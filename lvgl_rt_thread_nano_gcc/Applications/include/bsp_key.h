#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "main.h"

#define     KEY_WKUP_PROT   GPIOA
#define     KEY_PROT        GPIOE   

#define     KEY_WKUP_PIN    GPIO_PIN_0
#define     KEY1_PIN        GPIO_PIN_4
#define     KEY2_PIN        GPIO_PIN_3
#define     KEY3_PIN        GPIO_PIN_2

#define     KEY_ENABLE_ALL_CLK()    __HAL_RCC_GPIOA_CLK_ENABLE(); __HAL_RCC_GPIOE_CLK_ENABLE()

#define     KEY_WKUP_PIN_READ()   HAL_GPIO_ReadPin(KEY_WKUP_PROT, KEY_WKUP_PIN)
#define     KEY1_PIN_READ()       HAL_GPIO_ReadPin(KEY_PROT, KEY1_PIN)
#define     KEY2_PIN_READ()       HAL_GPIO_ReadPin(KEY_PROT, KEY2_PIN)
#define     KEY3_PIN_READ()       HAL_GPIO_ReadPin(KEY_PROT, KEY3_PIN)


enum {
    KEY1 = 1,
    KEY2,
    KEY3,
    KEY_WKUP
};

void bsp_key_init(void);
uint8_t key_scan(uint8_t mode);

#endif