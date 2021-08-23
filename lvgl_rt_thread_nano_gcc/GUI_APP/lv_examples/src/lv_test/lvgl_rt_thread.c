#include "lvgl.h"
#include "lvgl_rt_thread.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_examples.h"
#include "lcd.h"
#include "touch.h"
#include "rtthread.h"


static void lvgl_enter(void* parameter)
{

    while (1)
    {
        tp_dev.scan(0);
        lv_task_handler(); 
        rt_thread_mdelay(5);
        
    }
    
} 

static void lvgl_tick_enter(void* parameter)
{
    lv_tick_inc(10);

} 



void lvgl_thread_init(void)
{
    rt_thread_t lvgl_thread = rt_thread_create("lvgl_thread", lvgl_enter,
                                            RT_NULL, 2048, 4, 10);
    if(lvgl_thread != RT_NULL) {
        rt_thread_startup(lvgl_thread);
        rt_kprintf("lvgl_thread success\r\n");
    } else {
        rt_kprintf("lvgl_thread failed\r\n");
    }

    rt_timer_t lvgl_timer = rt_timer_create("lvgl_timer", lvgl_tick_enter, 
                                            RT_NULL, 10, RT_TIMER_FLAG_PERIODIC);

    if(lvgl_timer != RT_NULL) {
        rt_timer_start(lvgl_timer);
        rt_kprintf("lvgl_timer success\r\n");
    } else {
        rt_kprintf("lvgl tiemr failed \r\n");
    }
}