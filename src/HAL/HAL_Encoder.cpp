#include "HAL/HAL.h"
#include "App/Utils/ButtonEvent/ButtonEvent.h"


#define EC_K      9
#define EC_A      18
#define EC_B      4

static TaskHandle_t encoderTask_Handle;

static int encoder_diff;
static int encoder_count = 0;
static int encoder_flag = 0;
static SemaphoreHandle_t MuxSem_Handle = NULL;

static ButtonEvent EncoderPush(3000);
        
 
static void Encoder_PushHandler(ButtonEvent* btn, int event)
{
    if (event == ButtonEvent::EVENT_PRESSED)
    {
        HAL::Buzz_Tone(500, 20);
    } else if (event == ButtonEvent::EVENT_RELEASED)
    {
        HAL::Buzz_Tone(700, 20);
    } else if (event == ButtonEvent::EVENT_LONG_PRESSED)
    {
        HAL::Audio_PlayMusic("Shutdown");
        HAL::Power_Shutdown();
    }
}

static void Encoder_EventHandler()
{
  if(digitalRead(EC_A) == 0)
  {
    xSemaphoreGiveFromISR( MuxSem_Handle, NULL);//给出互斥量
  }
}

#if 0
static void calGyroY(float gy)
{
  const int mpu_delay_ms = 500;  
  static int getDataFlag;
  static unsigned long preInterval;

  if(1 ==  getDataFlag)
  {
    unsigned long temp = millis() - preInterval;

    if(abs((float)temp) < mpu_delay_ms)
    {
      return;
    }

    getDataFlag = 0;
  }
  

  if(gy > 350)
  {
    getDataFlag = 1;
    preInterval = millis();
    encoder_diff = -1;
    Serial.printf("encoder_diff ----> -1\n");
  }
  else if(gy < -350)
  {
     getDataFlag = 1;
     preInterval = millis();
     encoder_diff = 1;
     Serial.printf("encoder_diff ----> 1\n");
  }
}
#endif

static void encoder_task(void *param)
{
    (void)param;

    MuxSem_Handle = xSemaphoreCreateBinary();
    if(NULL == MuxSem_Handle)
    {
        Serial.printf("MuxSem_Handle = NULL!!\n");
        return;
    }

    Serial.printf("encoder_task start to run\n");

    while (1)
    {
    xSemaphoreTake(MuxSem_Handle,/* 互斥量句柄 */ 
                        portMAX_DELAY); /* 等待时间 */

    int dir;
    
    delay(10);
    if(0 == digitalRead(EC_B))
    {
      dir = -1;
    }
    else{
      dir = 1;
    }

    encoder_flag = 1;
    encoder_count += dir;

    encoder_diff += dir;
                        
#if 0
        calGyroY(mpu6050.getGyroY());
        vTaskDelay(pdMS_TO_TICKS(10));
#endif
        
    }
}

void HAL::Encoder_Init()
{
    pinMode(EC_A, INPUT_PULLUP);
    pinMode(EC_B, INPUT_PULLUP);
    pinMode(EC_K, INPUT_PULLUP);
    attachInterrupt(EC_A, Encoder_EventHandler, FALLING);   

    EncoderPush.EventAttach(Encoder_PushHandler);

     xTaskCreate(
        encoder_task,         /* 任务函数 */
        "encoder_task",       /* 任务名 */
        4 * 1024,             /* 任务栈大小，根据需要自行设置*/
        NULL,                 /* 参数，入参为空 */
        7,                    /* 优先级 */
        &encoderTask_Handle); /* 任务句柄 */                     
}

bool HAL::Encoder_GetIsPush()
{
    return (0 == digitalRead(EC_K));
}

int16_t HAL::Encoder_GetDiff()
{
    int ret = encoder_diff; 

    encoder_diff = 0;

    return ret;
}

void HAL::Encoder_Update()
{
  EncoderPush.EventMonitor(Encoder_GetIsPush());
}