#include <Arduino.h>
#include "./../inc/led_task.h"

#define LED_MODE    1       //0->渐变  1->闪烁

// define for ledc
#define LEDC_CHANNEL_0      0
#define LEDC_TIMER_12_BIT   12
#define LEDC_BASE_FREQ      5000
#define LEDC_PIN            21

static TaskHandle_t ledTask_Handle;

static int brightness = 0;
static int fadeAmount = 10;
static int ledc_dir = 1;

static void led_task(void *param)
{
    (void)param;

    Serial.printf("led_task start to run\n");

    while (1)
    {
#if LED_MODE==0        
        if (brightness <= 1000)
        {
            ledc_dir = 1;
        }
        else if (brightness >= 4095)
        {
            ledc_dir = -1;
        }

        brightness = brightness + ledc_dir*fadeAmount;
        ledcWrite(LEDC_CHANNEL_0, brightness);
#else
        ledcWrite(LEDC_CHANNEL_0, 4095);
        vTaskDelay(pdMS_TO_TICKS(700));
        ledcWrite(LEDC_CHANNEL_0, 0);
        vTaskDelay(pdMS_TO_TICKS(700));
#endif

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

static void ledTask_init(void)
{
    pinMode(LEDC_PIN, OUTPUT);
    ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
    ledcAttachPin(LEDC_PIN, LEDC_CHANNEL_0);
}

void led_task_init(void)
{
    ledTask_init();

    xTaskCreate(
        led_task,         /* 任务函数 */
        "led_task",       /* 任务名 */
        4 * 1024,         /* 任务栈大小，根据需要自行设置*/
        NULL,             /* 参数，入参为空 */
        2,                /* 优先级 */
        &ledTask_Handle); /* 任务句柄 */
}