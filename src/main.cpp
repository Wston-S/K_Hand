#include <Arduino.h>

#include "HAL/HAL.h"
#include "Port/Display.h"
#include "App/App.h"
#include "tasks/inc/demo_task.h"
#include "tasks/inc/sensor_task.h"
#include "tasks/inc/net_task.h"

// setup
void setup()
{
  HAL::Init();
  Port_Init();
  App_Init();

  // sensor_task_init();
  // net_task_init();
  // demo_task_init();
}

void loop()
{
  HAL::Update();

  delay(10);
}
