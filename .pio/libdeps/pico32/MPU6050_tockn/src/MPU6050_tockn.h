#ifndef MPU6050_TOCKN_H
#define MPU6050_TOCKN_H

#include "Arduino.h"
#include "Wire.h"

#define MPU6050_ADDR         0x68
#define MPU6050_SMPLRT_DIV   0x19  //�����ǲ����ʷ�Ƶ�Ĵ���
#define MPU6050_CONFIG       0x1a  //���üĴ���,��Ҫ�ļ��ٶȺͽ��ٶȴ���,��ʱ,������  
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

  int16_t getRawAccX(){ return rawAccX; }; //ok ԭʼ��ֵ -32768��С
  int16_t getRawAccY(){ return rawAccY; }; //ok
  int16_t getRawAccZ(){ return rawAccZ; }; //ok

  int16_t getRawTemp(){ return rawTemp; };

  int16_t getRawGyroX(){ return rawGyroX; }; //ok ԭʼ���� 
  int16_t getRawGyroY(){ return rawGyroY; };
  int16_t getRawGyroZ(){ return rawGyroZ; };

  float getTemp(){ return temp; };   //оƬ�¶�

  float getAccX(){ return accX; }; //ok ��������ֵ,���ٸ�g
  float getAccY(){ return accY; }; //ok
  float getAccZ(){ return accZ; }; //ok

  float getGyroX(){ return gyroX; }; //����֮��ĽǼ��ٶ�,������ʱ��0.0��
  float getGyroY(){ return gyroY; };
  float getGyroZ(){ return gyroZ; };

	void calcGyroOffsets(bool console = false, uint16_t delayBefore = 1000, uint16_t delayAfter = 3000);

  float getGyroXoffset(){ return gyroXoffset; };
  float getGyroYoffset(){ return gyroYoffset; };
  float getGyroZoffset(){ return gyroZoffset; };

  void update();

  float getAccAngleX(){ return angleAccX; };  //ok,׼ȷ��x�Ƕ�
  float getAccAngleY(){ return angleAccY; };

  float getGyroAngleX(){ return angleGyroX; }; //��Ϊ����Ϊ0,��ת��,����getAccAngleX,Ҳͦ׼��
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
