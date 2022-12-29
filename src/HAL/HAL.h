#ifndef __HAL_H
#define __HAL_H

#include <Arduino.h>


namespace HAL
{
    void Init();
    void Update();

/* Encoder */
    void Encoder_Init();
    int16_t Encoder_GetDiff();
    bool Encoder_GetIsPush();

/* Buzzer */
    void Buzz_Init();
    void Buzz_Tone(int32_t _freq, int32_t _duration = 0);
    void Buzz_SetEnable(bool en);

/* LED */
    void Led_Init();
    void Led_SetMode(int32_t mode);

/* IMU */
    void IMU_Init();
    void IMU_Update();

/*  Backlight */
    void Backlight_Init();
    void Backlight_SetValue(int32_t val);
    int32_t Backlight_GetValue();


/* Audio */
    void Audio_Init();
    void Audio_Update();
    bool Audio_PlayMusic(const char* name);
    
} 


#endif