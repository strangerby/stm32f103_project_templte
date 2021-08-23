/*
 * @Description: 
 * @Author: yzy
 * @Date: 2020-05-23 21:01:56
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-08-18 09:44:51
 * @FilePath: \hal\BSP\bsp_key.c
 */ 
#include "bsp_key.h"


void bsp_key_init(void)
{

    KEY_ENABLE_ALL_CLK();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = KEY_WKUP_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(KEY_WKUP_PROT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KEY1_PIN | KEY2_PIN | KEY3_PIN;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(KEY_PROT, &GPIO_InitStruct);

}

/*
mode : 0 不支持长按
       1 支持长按

*/

uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up=1;     //按键松开标志
    if(mode==1)key_up=1;    //支持连按
    if(key_up&&(KEY1_PIN_READ() == GPIO_PIN_RESET || KEY2_PIN_READ() == GPIO_PIN_RESET || \
        KEY3_PIN_READ() == GPIO_PIN_RESET || KEY_WKUP_PIN_READ() ==GPIO_PIN_SET)) {

        HAL_Delay(5);
        key_up=0;
        if(KEY1_PIN_READ() == GPIO_PIN_RESET)       return KEY1;
        else if(KEY2_PIN_READ() == GPIO_PIN_RESET)  return KEY2;
        else if(KEY3_PIN_READ() == GPIO_PIN_RESET)  return KEY3;
        else if(KEY_WKUP_PIN_READ() == GPIO_PIN_SET) return KEY_WKUP;          
    }else if(KEY1_PIN_READ() == GPIO_PIN_SET && KEY2_PIN_READ() == GPIO_PIN_SET && KEY3_PIN_READ() == GPIO_PIN_SET && KEY_WKUP_PIN_READ() == GPIO_PIN_RESET)key_up=1;
    return 0;   //无按键按下
}