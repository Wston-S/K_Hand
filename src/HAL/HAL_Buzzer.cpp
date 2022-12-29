#include "HAL/HAL.h"

#define BUZZ_PIN                22
#define BUZZ_LEDC_CHANNEL       2
#define BUZZ_LEDC_TIMER_BIT     12
#define BUZZ_LEDC_BASE_FREQ     5000

static bool IsEnable = true;
static int32_t duration = 0;
static uint32_t freq = 0;

static void BuzzerThread(void* argument)
{
    for (;;)
    {
        if (duration > 0)
        {
            ledcWriteTone(BUZZ_LEDC_CHANNEL, freq);
            delay(duration);
            ledcWriteTone(BUZZ_LEDC_CHANNEL, 0);

            duration = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void HAL::Buzz_Init()
{
    pinMode(BUZZ_PIN, OUTPUT);
    ledcAttachPin(BUZZ_PIN, BUZZ_LEDC_CHANNEL);
    ledcSetup(BUZZ_LEDC_CHANNEL, 10, BUZZ_LEDC_TIMER_BIT);
    ledcWriteTone(BUZZ_LEDC_CHANNEL, 0);

    // Create Buzzer thread
    TaskHandle_t handleBuzzerThread;
    xTaskCreate(
        BuzzerThread,
        "BuzzerThread",
        800,
        nullptr,
        1,
        &handleBuzzerThread);
}

void HAL::Buzz_Tone(int32_t _freq, int32_t _duration)
{
    if (!IsEnable)
        return;

    if (_duration == 0)
    {
        ledcWriteTone(BUZZ_LEDC_CHANNEL, _freq);
    } else
    {
        freq = _freq;
        duration = _duration;
    }
}

void HAL::Buzz_SetEnable(bool en)
{
    IsEnable = en;
}
