#include "HAL/HAL.h"


static void Power_ADC_Init()
{
    // BATTERY ADC INIT
    analogReadResolution(12);
    analogSetClockDiv(1);
    analogSetAttenuation(ADC_11db);
    adcAttachPin(34);
}

/**
  * @brief  电源初始化
  * @param  无
  * @retval 无
  */
void HAL::Power_Init()
{
    /*电池检测*/
    Power_ADC_Init();
}

/**
  * @brief  执行关机
  * @param  无
  * @retval 无
  */
void HAL::Power_Shutdown()
{
    Backlight_SetGradual(0, 500);
}


uint32_t HAL::Power_GetBatteryVol()
{
    uint32_t analog_value = analogReadMilliVolts(34);
    uint32_t battery_vol = (147*analog_value)/47;

    return battery_vol; 
}
