#include "./../inc/encoder_task.h"
#include <Arduino.h>
#include <OneButton.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "./../inc/sensor_task.h"

#define EC_K      9
#define EC_A      18
#define EC_B      4

static TaskHandle_t encoderTask_Handle;

static int encoder_diff;
static int encoder_count = 0;
static int encoder_flag = 0;
static OneButton button1(EC_K, true, true);
static SemaphoreHandle_t MuxSem_Handle = NULL;

static int test_encoder = 0;


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
#if 1
  //encoder_flag = 1;
  //xSemaphoreGiveFromISR( MuxSem_Handle, NULL);//给出互斥量

  if(digitalRead(EC_A) == 0)
  {
    //int dir;
    
    // if(0 == digitalRead(EC_B))
    // {
    //   test_encoder--;
    // }
    // else{
    //   test_encoder++;
    // }

    xSemaphoreGiveFromISR( MuxSem_Handle, NULL);//给出互斥量
  }
#endif

#if 0
  if(digitalRead(EC_A) == 0)
  {
    int dir;
    
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
  }  
#endif
}

int enc_pressed(void)
{
  return (0 == digitalRead(EC_K));
}

int16_t getencoder_diff(void)
{
  int ret = encoder_diff; 

  encoder_diff = 0;

  return ret;
}

void calGyroY(float gy)
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
    //Serial.printf("encoder_diff ----> -1\n");
  }
  else if(gy < -350)
  {
     getDataFlag = 1;
     preInterval = millis();
     encoder_diff = 1;
     //Serial.printf("encoder_diff ----> 1\n");
  }
}

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

    // if(digitalRead(EC_A) == 0)
    // {
    //   if(0 == digitalRead(EC_B))
    //   {
    //     test_encoder--;
    //   }
    //   else{
    //     test_encoder++;
    //   }
    // }
    
    //Serial.printf("test_encoder=%d\n", test_encoder);
#endif
                        
#if 0
        calGyroY(mpu6050.getGyroY());
        vTaskDelay(pdMS_TO_TICKS(10));
#endif
#if 0
        if(1 == encoder_flag)
        {
          encoder_flag = 0;
          //Serial.printf("encoder_count = %d\n", encoder_count);
          //Serial.printf("encoder_diff = %d\n", encoder_diff);
        }

        vTaskDelay(200 / portTICK_RATE_MS);
#endif
#if 0
        xSemaphoreTake(MuxSem_Handle,/* 互斥量句柄 */ 
                            portMAX_DELAY); /* 等待时间 */ 
                            
        Serial.printf("xSemaphoreTake --- \n");

        if(digitalRead(EC_A) == 0)
        {
          int dir;
          
          if(0 == digitalRead(EC_B))
          {
            dir = -1;
          }
          else{
            dir = 1;
          }

          encoder_count += dir;

          encoder_diff += dir;

          Serial.printf("count = %d\n", encoder_count);
        }    

        //vTaskDelay(pdMS_TO_TICKS(10));
        xSemaphoreTake(MuxSem_Handle, 0); //clear
#endif
#if 1
      button1.tick();
      vTaskDelay(10 / portTICK_RATE_MS);
#endif
        
    }
}

static void encoderTask_init(void)
{
    button1.attachClick(click1);
    button1.attachDoubleClick(doubleclick1);
    button1.attachMultiClick(MultiClickPress1);

    pinMode(EC_A, INPUT_PULLUP);
    pinMode(EC_B, INPUT_PULLUP);
    attachInterrupt(EC_A, Encoder_EventHandler, FALLING);
}

void encoder_task_init(void)
{
    encoderTask_init();

    xTaskCreate(
        encoder_task,         /* 任务函数 */
        "encoder_task",       /* 任务名 */
        4 * 1024,             /* 任务栈大小，根据需要自行设置*/
        NULL,                 /* 参数，入参为空 */
        7,                    /* 优先级 */
        &encoderTask_Handle); /* 任务句柄 */
}