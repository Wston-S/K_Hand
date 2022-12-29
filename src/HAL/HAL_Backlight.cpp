#include "HAL/HAL.h"
#include "Port/Display.h"

#define BK_CTL_PIN          25
#define BK_LEDC_CHANNEL     1
#define BK_LEDC_TIMER_BIT   12
#define BK_LEDC_BASE_FREQ   5000

static uint32_t bk_pwm = 0;


/**
  * @brief  背光亮度渐变，受lv_anim控制
  * @param  obj:无用
  * @param  brightness:亮度值
  * @retval None
  */
static void Backlight_AnimCallback(void *obj, int32_t brightness)
{
    HAL::Backlight_SetValue(brightness);
}

void HAL::Backlight_Init()
{
    pinMode(BK_CTL_PIN, OUTPUT);
    ledcSetup(BK_LEDC_CHANNEL, BK_LEDC_BASE_FREQ, BK_LEDC_TIMER_BIT);
    ledcAttachPin(BK_CTL_PIN, BK_LEDC_CHANNEL);
}

int32_t HAL::Backlight_GetValue()
{
    return (int32_t)bk_pwm;
}

void HAL::Backlight_SetValue(int32_t val)
{
    uint32_t pwm = (uint32_t)map(val, 0, 100, 0, 4095);

    bk_pwm = val;

    ledcWrite(BK_LEDC_CHANNEL, pwm);
}


/**
  * @brief  背光设置，渐变效果
  * @param  target:目标亮度(0~1000 -> 0~100%)
  * @retval 无
  */
void HAL::Backlight_SetGradual(uint32_t target, uint16_t time)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) Backlight_AnimCallback);
    lv_anim_set_values(&a, Backlight_GetValue(), target);
    lv_anim_set_time(&a, time);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);

    lv_anim_start(&a);
}