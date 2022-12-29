#include "HAL/HAL.h"
#include "App/Config/Version.h""

void HAL::Init()
{
    Serial.begin(115200);
    Serial.println(VERSION_FIRMWARE_NAME);
    Serial.println("Version: " VERSION_SOFTWARE);
    Serial.println("Author: " VERSION_AUTHOR_NAME);

    HAL::Backlight_Init();
    HAL::Led_Init();
    HAL::Encoder_Init();
    HAL::Buzz_Init();
    HAL::Audio_Init();

    Audio_PlayMusic("Startup");

}

void HAL::Update()
{
    HAL::Audio_Update();
}