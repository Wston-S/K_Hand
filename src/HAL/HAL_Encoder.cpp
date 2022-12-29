#include "HAL/HAL.h"
#include <OneButton.h>



#define EC_K      9
#define EC_A      18
#define EC_B      4

static TaskHandle_t encoderTask_Handle;

static int encoder_diff;
static int encoder_count = 0;
static int encoder_flag = 0;
static OneButton button1(EC_K, true, true);
static SemaphoreHandle_t MuxSem_Handle = NULL;


static void click1()
{
  Serial.println("Button 1 click.");
}

static void doubleclick1()
{
  Serial.println("Button 1 doubleclick.");
}

static void MultiClickPress1()
{
  Serial.println("MultiClickPress1");
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
#if 1
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
#endif
                        
#if 0
        calGyroY(mpu6050.getGyroY());
        vTaskDelay(pdMS_TO_TICKS(10));
#endif

#if 0
      button1.tick();
      vTaskDelay(10 / portTICK_RATE_MS);
#endif
        
    }
}

void HAL::Encoder_Init()
{
#if 0
    button1.attachClick(click1);
    button1.attachDoubleClick(doubleclick1);
    button1.attachMultiClick(MultiClickPress1);
#endif

    pinMode(EC_A, INPUT_PULLUP);
    pinMode(EC_B, INPUT_PULLUP);
    attachInterrupt(EC_A, Encoder_EventHandler, FALLING);   

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