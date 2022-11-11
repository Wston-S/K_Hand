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
        Serial.print(mpu6050.getGyroAngleX());    //������������ת��,����getAccAngleX,Ҳͦ׼��,
        Serial.print("\t");                       //�����Կ�����ʼ����ʱ���λ��Ϊԭʼ���,����͸�getAccAngleX������ͬ��
        Serial.print(mpu6050.getGyroAngleY());    //
        Serial.print("\t");
        Serial.println(mpu6050.getGyroAngleZ()); 
#endif

#if 0
        Serial.print("AccAngle X Y: \t");        //������̬!!!
        Serial.print(mpu6050.getAccAngleX());   //����ǳ�׼ȷ�ó�x��y��ĽǶ�,������0��180�Լ�0�� -180�㶼Ӧ,���������޾ͻ���ʾ����
        Serial.print("\t");                     //����getAngleX���������쳣 
        Serial.println(mpu6050.getAccAngleY()); //����������ʵ�ʵĽǶ�,��һ���getGyroAngleX��һ��,����ʱ������һ��
#endif
        

#if 0  
        Serial.print("angle X Y Z: \t");   
        Serial.print(mpu6050.getAngleX()); //X�������ĽǶ�,��ֱ��ʱ����90��,������ͨ�����ٶ��������
        Serial.print("\t");                //��ʱ��˦��̫�͵�ʱ��,����ǶȻ��쳣,���ǻ��ǻ������ÿ���ʵ��ֵ
        Serial.print(mpu6050.getAngleY());
        Serial.print("\t");
        Serial.println(mpu6050.getAngleZ()); 
#endif

#if 0
        Serial.print("RawAcc X Y Z T: \t");
        Serial.print(mpu6050.getRawAccX());  //x����ԭʼ���ٶȵ�ֵ,�������Ǹ�ֵ,����������ֵ
        Serial.print("\t");                  //��Сֵ -32768,Ŀǰz���ԭʼֵ��17800,��ǰ����Ϊ2g
        Serial.print(mpu6050.getRawAccY());  //���� 2g*(17800/32768)=1.08g(��������)
        Serial.print("\t");
        Serial.print(mpu6050.getRawAccZ()); 
        Serial.print("\n");  

        Serial.print("acc X Y Z T: \t");
        Serial.print(mpu6050.getAccX());   //�������xyz��ļ��ٶ�,ƽ�ŵ�ʱ�ֱ�Ϊ:0.06 -0.02  1.08
        Serial.print("\t");                //��Ϊz���յ��˵�������,������1.08g
        Serial.print(mpu6050.getAccY());
        Serial.print("\t");
        Serial.print(mpu6050.getAccZ()); 
        Serial.print("\n\n");
#endif

//�����Ǽ��ٶ�,��ת���ٶ�
#if 0
        Serial.print("RawGyro X Y Z T: \t");
        Serial.print(mpu6050.getRawGyroX());  //ok ԭʼ���� 
        Serial.print("\t");
        Serial.print(mpu6050.getRawGyroY());
        Serial.print("\t");
        Serial.print(mpu6050.getRawGyroZ()); 
        Serial.print("\n");  

        Serial.print("Gyro X Y Z T: \t");
        Serial.print(mpu6050.getGyroX());  //����֮��ĽǼ��ٶ�,������ʱ��0.0��,Ŀǰ������� 500��/min
        Serial.print("\t");
        Serial.print(mpu6050.getGyroY());
        Serial.print("\t");
        Serial.print(mpu6050.getGyroZ()); 
        Serial.print("\n\n");
#endif

//���� SerialChart
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
        sensor_task,         /* ������ */
        "sensor_task",       /* ������ */
        4 * 1024,         /* ����ջ��С��������Ҫ��������*/
        NULL,             /* ���������Ϊ�� */
        4,                /* ���ȼ� */
        &sensorTask_Handle); /* ������ */
}