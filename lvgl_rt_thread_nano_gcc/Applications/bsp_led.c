/*
 * @Description: 
 * @Author: yzy
 * @Date: 2020-05-23 20:09:37
 * @LastEditors: yzy
 * @LastEditTime: 2020-06-04 10:12:30
 * @FilePath: \hal\BSP\bsp_led.c
 */ 
#include "bsp_led.h"

void bsp_led_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin =  LED2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LED2_PROT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED3_PIN;

    HAL_GPIO_Init(LED3_PROT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(LED2_PROT, LED2_PIN, GPIO_PIN_SET);   //输出高电平 关灯
    HAL_GPIO_WritePin(LED3_PROT, LED3_PIN, GPIO_PIN_SET);   //输出高电平 关灯

}

void bsp_led_on(uint8_t LED_NUM)
{
    switch(LED_NUM) {
    case LED2 :     
        HAL_GPIO_WritePin(LED2_PROT, LED2_PIN, GPIO_PIN_RESET); break;
    case LED3 :
        HAL_GPIO_WritePin(LED3_PROT, LED3_PIN, GPIO_PIN_RESET); break;
    case LED2|LED3 :
        HAL_GPIO_WritePin(LED2_PROT, LED2_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED3_PROT, LED3_PIN, GPIO_PIN_RESET); break;
    default : break;
    }

}


void bsp_led_off(uint8_t LED_NUM)
{
    switch(LED_NUM) {
    case LED2 :     
        HAL_GPIO_WritePin(LED2_PROT, LED2_PIN, GPIO_PIN_SET); break;
    case LED3 :
        HAL_GPIO_WritePin(LED3_PROT, LED3_PIN, GPIO_PIN_SET); break;
    case LED2 | LED3 :
        HAL_GPIO_WritePin(LED2_PROT, LED2_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED3_PROT, LED3_PIN, GPIO_PIN_SET); break;
    default : break;
    }
}

void bsp_led_toggle(uint8_t LED_NUM)
{
    switch(LED_NUM) {
    case LED2 :
        HAL_GPIO_TogglePin(LED2_PROT, LED2_PIN); break;
    case LED3 :
        HAL_GPIO_TogglePin(LED3_PROT, LED3_PIN); break;
    case (LED2 | LED3) :
        HAL_GPIO_TogglePin(LED2_PROT, LED2_PIN);
        HAL_GPIO_TogglePin(LED3_PROT, LED3_PIN); break;

    default : break;
    }
    


}