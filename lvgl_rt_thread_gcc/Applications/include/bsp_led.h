/*
 * @Description: 
 * @Author: yzy
 * @Date: 2020-05-23 20:09:45
 * @LastEditors: yzy
 * @LastEditTime: 2020-06-04 09:41:13
 * @FilePath: \hal\BSP\bsp_led.h
 */ 
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "main.h"

#define LED2_PROT   GPIOE
#define LED3_PROT   GPIOB

#define LED2_PIN    GPIO_PIN_5
#define LED3_PIN    GPIO_PIN_5 

#define LED2        2
#define LED3        4


void bsp_led_init(void);
void bsp_led_on(uint8_t LED_NUM);
void bsp_led_off(uint8_t LED_NUM);
void bsp_led_toggle(uint8_t LED_NUM);

#endif // !__BSP_LED_H__

