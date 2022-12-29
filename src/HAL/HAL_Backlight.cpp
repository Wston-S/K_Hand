#include "HAL/HAL.h"


#define BK_DEFAULT          10
#define BK_CTL_PIN          25
#define BK_LEDC_CHANNEL     1
#define BK_LEDC_TIMER_BIT   12
#define BK_LEDC_BASE_FREQ   5000

static uint32_t bk_pwm;

void HAL::Backlight_Init()
{
    pinMode(BK_CTL_PIN, OUTPUT);
    ledcSetup(BK_LEDC_CHANNEL, BK_LEDC_BASE_FREQ, BK_LEDC_TIMER_BIT);
    ledcAttachPin(BK_CTL_PIN, BK_LEDC_CHANNEL);

    Backlight_SetValue(BK_DEFAULT);
}

int32_t HAL::Backlight_GetValue()
{
    return (int32_t)bk_pwm;
}

void HAL::Backlight_SetValue(int32_t val)
{
    bk_pwm = (uint32_t)map(val, 0, 100, 0, 4095);

    ledcWrite(BK_LEDC_CHANNEL, bk_pwm);
}