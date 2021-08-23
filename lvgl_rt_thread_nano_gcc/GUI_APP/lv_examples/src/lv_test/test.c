#include "test.h"
#include "main.h"
#include "rtthread.h"




lv_obj_t* label1;
lv_obj_t* label2;
lv_obj_t* dialog;
lv_obj_t* scr;
lv_obj_t* cancel;
lv_obj_t* ok;
lv_obj_t* btn1 = NULL, *btn2 = NULL, *btn3 = NULL;
lv_obj_t* btn1_lab;
static lv_style_t label1_style;


void btn_handleer(lv_obj_t* obj, lv_event_t event)
{
    if(obj == btn1) {
        switch(event) {
            case LV_EVENT_CLICKED :
                lv_label_set_text(btn1_lab, "LV_EVENT_CLICKED");
            break;
            case LV_EVENT_LONG_PRESSED:
                lv_label_set_text(btn1_lab, "LV_EVENT_LONG_PRESSED");
            break;
            case LV_EVENT_LONG_PRESSED_REPEAT:
                lv_label_set_text(btn1_lab, "LV_EVENT_LONG_PRESSED_REPEAT");
            break;
            case LV_EVENT_PRESS_LOST:
                lv_label_set_text(btn1_lab, "LV_EVENT_PRESS_LOST");
            break;

        }
        
    }

}

void test_start(void)
{
    scr = lv_scr_act(); 

    btn1 = lv_btn_create(scr, NULL);


    btn1_lab = lv_label_create(scr, NULL);

    lv_obj_set_drag(btn1, true);

    lv_style_init(&label1_style);

    lv_style_set_text_color(&label1_style, LV_STATE_DEFAULT, LV_COLOR_BLUE);
    lv_style_set_bg_color(&label1_style, LV_STATE_DEFAULT, LV_COLOR_GREEN);
    lv_style_set_bg_opa(&label1_style, LV_STATE_DEFAULT, LV_OPA_COVER);

    lv_obj_add_style(btn1_lab, LV_LABEL_PART_MAIN, &label1_style);
    lv_label_set_text(btn1_lab, "Normal");

    lv_obj_align(btn1_lab, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);
    lv_obj_align(btn1, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
    
    lv_obj_set_event_cb(btn1, btn_handleer);
        
}

