#ifndef MPU6050_TOCKN_H
#define MPU6050_TOCKN_H

#include "Arduino.h"
#include "Wire.h"

#define MPU6050_ADDR         0x68
#define MPU6050_SMPLRT_DIV   0x19  //陀螺仪采样率分频寄存器
#define MPU6050_CONFIG       0x1a  //配置寄存器,主要的加速度和角速度带宽,延时,采样率  
#define MPU6050_GYRO_CONFIG  0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6b
#define MPU6050_TEMP_H       0x41
#define MPU6050_TEMP_L       0x42

class MPU6050{
  public:

  MPU6050(TwoWire &w);
  MPU6050(TwoWire &w, float aC, float gC);

  void begin();

  void setGyroOffsets(float x, float y, float z);

  void writeMPU6050(byte reg, byte data);
  byte readMPU6050(byte reg);

  int16_t getRawAccX(){ return rawAccX; }; //ok 原始数值 -32768最小
  int16_t getRawAccY(){ return rawAccY; }; //ok
  int16_t getRawAccZ(){ return rawAccZ; }; //ok

  int16_t getRawTemp(){ return rawTemp; };

  int16_t getRawGyroX(){ return rawGyroX; }; //ok 原始数据 
  int16_t getRawGyroY(){ return rawGyroY; };
  int16_t getRawGyroZ(){ return rawGyroZ; };

  float getTemp(){ return temp; };   //芯片温度

  float getAccX(){ return accX; }; //ok 计算后的数值,多少个g
  float getAccY(){ return accY; }; //ok
  float getAccZ(){ return accZ; }; //ok

  float getGyroX(){ return gyroX; }; //计算之后的角加速度,不动的时候0.0几
  float getGyroY(){ return gyroY; };
  float getGyroZ(){ return gyroZ; };

	void calcGyroOffsets(bool console = false, uint16_t delayBefore = 1000, uint16_t delayAfter = 3000);

  float getGyroXoffset(){ return gyroXoffset; };
  float getGyroYoffset(){ return gyroYoffset; };
  float getGyroZoffset(){ return gyroZoffset; };

  void update();

  float getAccAngleX(){ return angleAccX; };  //ok,准确的x角度
  float getAccAngleY(){ return angleAccY; };

  float getGyroAngleX(){ return angleGyroX; }; //以为开机为0,旋转角,类似getAccAngleX,也挺准的
  float getGyroAngleY(){ return angleGyroY; };
  float getGyroAngleZ(){ return angleGyroZ; };

  float getAngleX(){ return angleX; }; //ok
  float getAngleY(){ return angleY; };
  float getAngleZ(){ return angleZ; };

  private:

  TwoWire *wire;

  int16_t rawAccX, rawAccY, rawAccZ, rawTemp,
  rawGyroX, rawGyroY, rawGyroZ;

  float gyroXoffset, gyroYoffset, gyroZoffset;

  float temp, accX, accY, accZ, gyroX, gyroY, gyroZ;

  float angleGyroX, angleGyroY, angleGyroZ,
  angleAccX, angleAccY, angleAccZ;

  float angleX, angleY, angleZ;

  float interval;
  long preInterval;

  float accCoef, gyroCoef;
};

#endif
