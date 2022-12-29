#include "Display.h"
#include <Arduino.h>
#include <lvgl.h>
#include "HAL/HAL.h"
#include <Port/lv_port/lv_port_disp.h>
#include <Port/lv_port/lv_port_indev.h>


static TaskHandle_t  lvglTask_Handle;


void demo1(void)
{
    String LVGL_Arduino;
    uint32_t vol = HAL::Power_GetBatteryVol();
    LVGL_Arduino += "Battery Vol:"+String(vol)+"mv   ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch(); //版本
    lv_obj_t *label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, LVGL_Arduino.c_str());
    lv_obj_align(label1, LV_ALIGN_BOTTOM_RIGHT, 0, 0); 


    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());//创建按钮1
	lv_obj_set_size(btn1,50,30);
	lv_obj_set_pos(btn1,0,0);
	
	lv_obj_t *btn2 = lv_btn_create(lv_scr_act());//创建按钮2
    lv_obj_set_size(btn2,50,30);
	lv_obj_set_pos(btn2,0,60);

	lv_obj_t *btn3 = lv_btn_create(lv_scr_act());//创建按钮3
    lv_obj_set_size(btn3,50,30);
	lv_obj_set_pos(btn3,0,120);

	lv_obj_t *btn4 = lv_btn_create(lv_scr_act());//创建按钮4
    lv_obj_set_size(btn4,50,30);
	lv_obj_set_pos(btn4,0,180);

	lv_obj_t *roller = lv_roller_create(lv_scr_act());//创建滚轮控件
	lv_roller_set_options(roller,"1\n2\n3\n4\n5\n6",LV_ROLLER_MODE_INFINITE);
	lv_obj_set_pos(roller,100,55);
	
	lv_group_add_obj(getEncoderGroup() ,btn1);
	lv_group_add_obj(getEncoderGroup() ,btn2);
	lv_group_add_obj(getEncoderGroup() ,btn3);
	lv_group_add_obj(getEncoderGroup() ,btn4);
	lv_group_add_obj(getEncoderGroup() ,roller);
	lv_group_set_editing(getEncoderGroup(),false);
}

static void lvgl_task(void *param)
{
    (void)param;

    Serial.printf("lvgl_task start to run\n");

    while(1)
    {       
        lv_timer_handler();

        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void Port_Init()
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
	
    demo1();

    xTaskCreate(
            lvgl_task,          /* 任务函数 */
            "lvgl_task",         /* 任务名 */
            10*1024,            /* 任务栈大小，根据需要自行设置*/
            NULL,              /* 参数，入参为空 */
            1,                 /* 优先级 */
            &lvglTask_Handle);  /* 任务句柄 */

    HAL::Backlight_SetGradual(100, 1300);
}
