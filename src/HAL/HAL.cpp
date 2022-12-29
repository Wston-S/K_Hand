#include "HAL/HAL.h"


void HAL::Init()
{
    Serial.begin(115200);
    
    Backlight_Init();
}

void HAL::Update()
{
    
}