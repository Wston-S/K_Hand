#ifndef __DISPLAY_H
#define __DISPALY_H

#include "lvgl.h" 
#include "TFT_eSPI.h"
#include "App/Configs/Config.h"

extern TaskHandle_t handleTaskLvgl;

void Port_Init();

#endif