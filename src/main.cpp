#include <Arduino.h>

#include "HAL/HAL.h"

#include "tasks/inc/lvgl_task.h"
#include "tasks/inc/led_task.h"
#include "tasks/inc/encoder_task.h"
#include "tasks/inc/demo_task.h"
#include "tasks/inc/sensor_task.h"
#include "tasks/inc/net_task.h"

// setup
void setup()
{
  HAL::Init();

  led_task_init();
  lvgl_task_init();
  //sensor_task_init();
  encoder_task_init();
  net_task_init();
  

  //demo_task_init();

}

void loop()
{
  //nothing
}




