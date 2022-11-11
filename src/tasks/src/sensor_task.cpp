#include <Arduino.h>
#include "./../inc/sensor_task.h"


#define BAT_ADC_IO 34

MPU6050 mpu6050(Wire);
static int analog_value = 0;
static TaskHandle_t sensorTask_Handle;

static void sensor_task(void *param)
{
    (void)param;

    Serial.printf("sensor_task start to run\n");

    while (1)
    {
        mpu6050.update();

#if 0
        Serial.print("GyroAngleX X Y Z: \t");   
        Serial.print(mpu6050.getGyroAngleX());    //这里是三个旋转角,类似getAccAngleX,也挺准的,
        Serial.print("\t");                       //它是以开机初始化的时候的位置为原始点的,这个就跟getAccAngleX有所不同了
        Serial.print(mpu6050.getGyroAngleY());    //
        Serial.print("\t");
        Serial.println(mpu6050.getGyroAngleZ()); 
#endif

#if 0
        Serial.print("AccAngle X Y: \t");        //飞行姿态!!!
        Serial.print(mpu6050.getAccAngleX());   //这里非常准确得出x轴y轴的角度,而且是0到180以及0到 -180°都应,第三四象限就会显示负数
        Serial.print("\t");                     //不像getAngleX那样会有异常 
        Serial.println(mpu6050.getAccAngleY()); //并且这里是实际的角度,这一点跟getGyroAngleX不一样,跟何时开机不一样
#endif
        

#if 0  
        Serial.print("angle X Y Z: \t");   
        Serial.print(mpu6050.getAngleX()); //X轴跟地面的角度,垂直的时候是90°,这里是通过加速度算出来的
        Serial.print("\t");                //有时候甩得太猛的时候,这个角度会异常,但是还是会慢慢得靠近实际值
        Serial.print(mpu6050.getAngleY());
        Serial.print("\t");
        Serial.println(mpu6050.getAngleZ()); 
#endif

#if 0
        Serial.print("RawAcc X Y Z T: \t");
        Serial.print(mpu6050.getRawAccX());  //x方向原始加速度的值,正方向是负值,反方向是正值
        Serial.print("\t");                  //最小值 -32768,目前z轴的原始值是17800,当前配置为2g
        Serial.print(mpu6050.getRawAccY());  //所以 2g*(17800/32768)=1.08g(地球重力)
        Serial.print("\t");
        Serial.print(mpu6050.getRawAccZ()); 
        Serial.print("\n");  

        Serial.print("acc X Y Z T: \t");
        Serial.print(mpu6050.getAccX());   //处理过的xyz轴的加速度,平放的时分别为:0.06 -0.02  1.08
        Serial.print("\t");                //因为z轴收到了地球重力,所以是1.08g
        Serial.print(mpu6050.getAccY());
        Serial.print("\t");
        Serial.print(mpu6050.getAccZ()); 
        Serial.print("\n\n");
#endif

//陀螺仪加速度,旋转加速度
#if 0
        Serial.print("RawGyro X Y Z T: \t");
        Serial.print(mpu6050.getRawGyroX());  //ok 原始数据 
        Serial.print("\t");
        Serial.print(mpu6050.getRawGyroY());
        Serial.print("\t");
        Serial.print(mpu6050.getRawGyroZ()); 
        Serial.print("\n");  

        Serial.print("Gyro X Y Z T: \t");
        Serial.print(mpu6050.getGyroX());  //计算之后的角加速度,不动的时候0.0几,目前配置最大 500°/min
        Serial.print("\t");
        Serial.print(mpu6050.getGyroY());
        Serial.print("\t");
        Serial.print(mpu6050.getGyroZ()); 
        Serial.print("\n\n");
#endif

//测试 SerialChart
#if 0
        Serial.print(mpu6050.getGyroX());
        Serial.print(",");
        Serial.print(mpu6050.getGyroY());
        Serial.print(",");
        Serial.print(mpu6050.getGyroZ()); 
        Serial.print("\n");
#endif


        //analog_value = analogReadMilliVolts(BAT_ADC_IO);
        //Serial.printf("analog_value = %dmV, Battery Vol = %dmV\n", analog_value, (147*analog_value)/47);

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

static void sensorTask_init(void)
{
    Wire.begin();
    mpu6050.begin();
    //mpu6050.calcGyroOffsets(true);
    mpu6050.setGyroOffsets(1.25, 1.62, 0.19);

    // BATTERY ADC INIT
    analogReadResolution(12);
    analogSetClockDiv(1);
    analogSetAttenuation(ADC_11db);
    adcAttachPin(BAT_ADC_IO);
}

void sensor_task_init(void)
{
    sensorTask_init();

    xTaskCreate(
        sensor_task,         /* 任务函数 */
        "sensor_task",       /* 任务名 */
        4 * 1024,         /* 任务栈大小，根据需要自行设置*/
        NULL,             /* 参数，入参为空 */
        4,                /* 优先级 */
        &sensorTask_Handle); /* 任务句柄 */
}