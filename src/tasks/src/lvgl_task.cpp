#include <Arduino.h>
#include <lvgl.h>
#include <Port/lv_port_disp.h>
#include <Port/lv_port_indev.h>
#include <stdio.h>
//#include "demos/lv_demos.h"
#include "./demo/demo.h"

extern int enc_pressed(void);
extern int16_t getencoder_diff(void);
extern void lv_example_anim_2(void);

static TaskHandle_t  lvglTask_Handle;

static void lvglTask_init(void)
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    //test

    /*获取LVGL版本信息*/
#if 0
    String LVGL_Arduino = "Hello LVGL! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch(); //版本
    LVGL_Arduino += String(", I'm Kuver!");
    label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, LVGL_Arduino.c_str());
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0); 
#endif
	
#if 1
	lv_obj_t *btn1 = lv_btn_create(lv_scr_act());//创建按钮1
	lv_obj_set_size(btn1,50,30);
	lv_obj_set_pos(btn1,0,0);
	
	lv_obj_t *btn2 = lv_btn_create(lv_scr_act());//创建按钮2
	lv_obj_set_pos(btn2,0,60);

	lv_obj_t *btn3 = lv_btn_create(lv_scr_act());//创建按钮3
	lv_obj_set_pos(btn3,0,120);

	lv_obj_t *btn4 = lv_btn_create(lv_scr_act());//创建按钮4
	lv_obj_set_pos(btn4,0,180);

	lv_obj_t *roller = lv_roller_create(lv_scr_act());//创建滚轮控件
	lv_roller_set_options(roller,"1\n2\n3\n4\n5\n6",LV_ROLLER_MODE_INFINITE);
	lv_obj_set_pos(roller,100,100);
	
	lv_group_add_obj(getEncoderGroup() ,btn1);
	lv_group_add_obj(getEncoderGroup() ,btn2);
	lv_group_add_obj(getEncoderGroup() ,btn3);
	lv_group_add_obj(getEncoderGroup() ,btn4);
	lv_group_add_obj(getEncoderGroup() ,roller);
	lv_group_set_editing(getEncoderGroup(),false);
#endif

    //lv_demo_stress();         //no
    //lv_demo_music();          //no
    //lv_demo_widgets();        //no
    //lv_demo_benchmark();      //no 
    
    //lv_example_ani1();
    //lv_example_ani2();
    //lv_example_ani3();
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

void lvgl_task_init(void)
{
    lvglTask_init();

    xTaskCreate(
            lvgl_task,          /* 任务函数 */
            "lvgl_task",         /* 任务名 */
            10*1024,            /* 任务栈大小，根据需要自行设置*/
            NULL,              /* 参数，入参为空 */
            1,                 /* 优先级 */
            &lvglTask_Handle);  /* 任务句柄 */
}