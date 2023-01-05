#ifndef __APP_H
#define __APP_H


#include "Port/Display.h"
#define INIT_DONE() \
do{                 \
    xTaskNotifyGive(handleTaskLvgl); \
}while(0) \

void App_Init();
void App_UnInit();

#endif