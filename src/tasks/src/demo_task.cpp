#include <Arduino.h>
#include <Ticker.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "./../inc/demo_task.h"

#define USE_TICKER_DEMO           0
#define USE_TFT_DRAW_UI_DEMO      0
#define USE_HAREWARE_TIMER_DEMO   0

extern TFT_eSPI tft;

static TaskHandle_t demoTask_Handle;

static volatile SemaphoreHandle_t timerSemaphore;
static portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
static uint32_t cnt_ms = 0;
static volatile uint32_t isrCounter = 0;
static volatile uint32_t lastIsrAt = 0;
static hw_timer_t *timer = NULL; //定义存放定时器的指针

void draw_rect(int x, int y, int dis, int d, uint32_t color, int delay_us);

// 1ms callback
#if USE_HAREWARE_TIMER_DEMO
static void ARDUINO_ISR_ATTR onTimer()
{
  portENTER_CRITICAL_ISR(&timerMux);
  cnt_ms++;
  isrCounter++;
  lastIsrAt = millis();
  portEXIT_CRITICAL_ISR(&timerMux);
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
}
#endif

// ticker
#if USE_TICKER_DEMO

void tick_func()
{
  Serial.printf("tick_func: %d\n", millis());
}

Ticker ticker1(tick_func, 300, 0, MILLIS);

#endif


static void demo_task(void *param)
{
    (void)param;

    Serial.printf("demo_task start to run\n");

    while (1)
    {
#if USE_HAREWARE_TIMER_DEMO      
        if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
            uint32_t isrCount = 0, isrTime = 0;
            // Read the interrupt count and time
            portENTER_CRITICAL(&timerMux);
            isrCount = isrCounter;
            isrTime = lastIsrAt;
            portEXIT_CRITICAL(&timerMux);
            // Print it
            Serial.print("onTimer no. ");
            Serial.print(isrCount);
            Serial.print(" at ");
            Serial.print(isrTime);
            Serial.println(" ms");
        }
#endif

#if USE_TFT_DRAW_UI_DEMO
        draw_rect(0, 0, 1, 239, TFT_RED, 500);
        draw_rect(10, 10, 1, 20, TFT_DARKGREEN, 500);
        draw_rect(200, 100, 1, 50, TFT_MAROON, 500);
        draw_rect(30, 50, 1, 140, TFT_OLIVE, 500);
        draw_rect(0, 120, 1, 200, TFT_YELLOW, 500);
        draw_rect(30, 50, 1, 100, TFT_WHITE, 500);
        draw_rect(200, 50, 1, 200, TFT_GOLD, 500);
#endif

#if USE_TICKER_DEMO
        ticker1.update();
#endif

        vTaskDelay(30 / portTICK_RATE_MS);
    }
}

static void printESP(void)
{
  Serial.printf("ESP.getChipRevision:%d\n", ESP.getChipRevision());
  Serial.printf("ESP.getCpuFreqMHz:%d\n", ESP.getCpuFreqMHz());
  Serial.printf("ESP.getCycleCount:%d\n", ESP.getCycleCount());
  Serial.printf("ESP.getEfuseMac:%u\n", ESP.getEfuseMac());
  Serial.printf("ESP.getFlashChipMode:%d\n", ESP.getFlashChipMode());
  Serial.printf("ESP.getFlashChipSize:%d\n", ESP.getFlashChipSize());
  Serial.printf("ESP.getFlashChipSpeed:%d\n", ESP.getFlashChipSpeed());
  Serial.printf("ESP.getFreeHeap:%d\n", ESP.getFreeHeap());
  Serial.printf("ESP.getFreePsram:%d\n", ESP.getFreePsram());
  Serial.printf("ESP.getFreeSketchSpace:%d\n", ESP.getFreeSketchSpace());
  Serial.printf("ESP.getHeapSize:%d\n", ESP.getHeapSize());
  Serial.printf("ESP.getMaxAllocHeap:%d\n", ESP.getMaxAllocHeap());
  Serial.printf("ESP.getMaxAllocPsram:%d\n", ESP.getMaxAllocPsram());
  Serial.printf("ESP.getMinFreeHeap:%d\n", ESP.getMinFreeHeap());
  Serial.printf("ESP.getMinFreePsram:%d\n", ESP.getMinFreePsram());
  Serial.printf("ESP.getPsramSize:%d\n", ESP.getPsramSize());
  Serial.printf("ESP.getSdkVersion:%s\n", ESP.getSdkVersion());
  Serial.printf("ESP.getSketchMD5:%s\n", ESP.getSketchMD5());
  Serial.printf("ESP.getSketchSize:%d\n", ESP.getSketchSize());
}

static void demoTask_init(void)
{

#if USE_TICKER_DEMO
  ticker1.start();
#endif
  
#if USE_HAREWARE_TIMER_DEMO  
  // hareware timer init
  timerSemaphore = xSemaphoreCreateBinary();
  timer = timerBegin(0, 80, true); //设置定时器0，80分频，定时器是否上下计数
                                   //每个定时器都以APB_CLK(80MHz)作为基础时钟,80分频之后就是1Mhz
                                   // 1Mhz->定时器计数加一就是1us->timerAlarmWrite()的参数2单位就是us
  if (NULL != timer)
  {
    timerAttachInterrupt(timer, onTimer, true); //定时器地址指针，中断函数名称，中断边沿触发类型
    timerAlarmWrite(timer, 1000, true);         //操作那个定时器，定时时长单位us，是否自动重装载
    timerAlarmEnable(timer);                    //打开那个定时器
  }
  else
  {
    Serial.println("timer init failed\n");
  }
#endif

}

void demo_task_init(void)
{
    demoTask_init();

    xTaskCreate(
        demo_task,         /* 任务函数 */
        "demo_task",       /* 任务名 */
        4 * 1024,         /* 任务栈大小，根据需要自行设置*/
        NULL,             /* 参数，入参为空 */
        1,                /* 优先级 */
        &demoTask_Handle); /* 任务句柄 */
}



/*  
*       绘制：     螺旋矩形
*   x,y:左上角位置
*   dis:线条间距
*   d:矩形的宽
*   color颜色
*   delay_us 微秒延时动画
*
*/
void draw_rect(int x, int y, int dis, int d, uint32_t color, int delay_us)
{
    int i, xx = x, yy = y;

    //tft.fillRect(x, y, d, d, TFT_BLACK);

    int dir_cnt = 0;    // 取余 0:向右  1:向下  2:向左  3：向上
    int need_len = d;        // 需要前进距离
    int has_len = 0;

    while(1)
    {
        tft.drawPixel(xx, yy, color);
        if(0 != delay_us)
        {
            //delay(delay_us/1000<0?1:delay_us/1000);
            usleep(delay_us);
        }
        has_len++;

        if(dir_cnt%4 == 0) //向右
        {
            xx++;
        }

        if(dir_cnt%4 == 1) //向下
        {
            yy++;
        }

        if(dir_cnt%4 == 2) //向左
        {
            xx--;
        }

        if(dir_cnt%4 == 3) //向上
        {
            yy--;
        }

        if(has_len >= need_len)
        {
            dir_cnt++;
            has_len = 1;

            if(dir_cnt<=2)
            {
                need_len = d;
            } 
            else
            {
                need_len = need_len - dis - 1;
            }

            if(need_len <= 0)
            {
                break;
            }
        }
    }
}
