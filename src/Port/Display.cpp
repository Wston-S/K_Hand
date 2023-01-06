#include "Display.h"
#include <Arduino.h>
#include <lvgl.h>
#include "HAL/HAL.h"
#include <Port/lv_port/lv_port_disp.h>
#include <Port/lv_port/lv_port_indev.h>
#include "App/Resources/ResourcePool.h"
#include "App/Pages/StatusBar/StatusBar.h"

TaskHandle_t  handleTaskLvgl;


static void bt_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);
    char *text = lv_label_get_text(label);

    if(code == LV_EVENT_SHORT_CLICKED) 
    {
        Serial.printf("bt_event_handler\n");
        Serial.printf("%s has clicked\n", text);

        HAL::Audio_PlayMusic(text);
    }
}

void demo1(void)
{
    lv_obj_remove_style_all(lv_scr_act());
    lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());
    
    String LVGL_Arduino;
    uint32_t vol = HAL::Power_GetBatteryVol();
    LVGL_Arduino += "Battery Vol:"+String(vol)+"mv   ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch(); //版本
    lv_obj_t *label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, LVGL_Arduino.c_str());
    lv_obj_align(label1, LV_ALIGN_BOTTOM_RIGHT, 0, 0); 

    lv_obj_t *label_temp;

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());//创建按钮1
	lv_obj_set_size(btn1,110,30);
	lv_obj_set_pos(btn1,0,0);
    label_temp = lv_label_create(btn1);
    lv_label_set_text(label_temp, "Error");
    lv_obj_add_event_cb(btn1, bt_event_handler, LV_EVENT_ALL, label_temp);
	
	lv_obj_t *btn2 = lv_btn_create(lv_scr_act());//创建按钮2
    lv_obj_set_size(btn2,110,30);
	lv_obj_set_pos(btn2,0,60);
    label_temp = lv_label_create(btn2);
    lv_label_set_text(label_temp, "Connect");
    lv_obj_add_event_cb(btn2, bt_event_handler, LV_EVENT_ALL, label_temp);

	lv_obj_t *btn3 = lv_btn_create(lv_scr_act());//创建按钮3
    lv_obj_set_size(btn3,110,30);
	lv_obj_set_pos(btn3,0,120);
    label_temp = lv_label_create(btn3);
    lv_label_set_text(label_temp, "Disconnect");
    lv_obj_add_event_cb(btn3, bt_event_handler, LV_EVENT_ALL, label_temp);

	lv_obj_t *btn4 = lv_btn_create(lv_scr_act());//创建按钮4
    lv_obj_set_size(btn4,150,30);
	lv_obj_set_pos(btn4,0,180);
    label_temp = lv_label_create(btn4);
    lv_label_set_text(label_temp, "BattChargeStart");
    lv_obj_add_event_cb(btn4, bt_event_handler, LV_EVENT_ALL, label_temp);

	lv_obj_t *roller = lv_roller_create(lv_scr_act());//创建滚轮控件
	lv_roller_set_options(roller,"1\n2\n3\n4\n5\n6",LV_ROLLER_MODE_INFINITE);
	lv_obj_set_pos(roller,150,20);
	
	lv_group_add_obj(getEncoderGroup() ,btn1);
	lv_group_add_obj(getEncoderGroup() ,btn2);
	lv_group_add_obj(getEncoderGroup() ,btn3);
	lv_group_add_obj(getEncoderGroup() ,btn4);
	lv_group_add_obj(getEncoderGroup() ,roller);
	lv_group_set_editing(getEncoderGroup(),false);
}

static void demo2()
{
    LV_IMG_DECLARE(img_src_satellite);

    lv_obj_t* img = lv_img_create(lv_scr_act());
     lv_img_set_src(img, Resource.GetImage("satellite"));
    lv_obj_set_pos(img, 10, 100);

    img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, Resource.GetImage("sd_card"));
    lv_obj_set_pos(img, 50, 100);

    img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, Resource.GetImage("bluetooth"));
    lv_obj_set_pos(img, 80, 100);

    img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, Resource.GetImage("battery"));
    lv_obj_set_pos(img, 100, 100);

    Serial.printf("demo2 end\n");
}   

static void lvgl_task(void *param)
{
    (void)param;

    Serial.printf("lvgl_task ready to run\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.printf("lvgl_task start to run\n");

    demo2();

    StatusBar::Appear(true);

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

    xTaskCreate(
            lvgl_task,          /* 任务函数 */
            "lvgl_task",         /* 任务名 */
            20*1024,            /* 任务栈大小，根据需要自行设置*/
            NULL,              /* 参数，入参为空 */
            configMAX_PRIORITIES - 1,                 /* 优先级 */
            &handleTaskLvgl);  /* 任务句柄 */

    HAL::Backlight_SetGradual(100, 1300);
}
