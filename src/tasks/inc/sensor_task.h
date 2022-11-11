#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include <MPU6050_tockn.h>
#include <Wire.h>

extern MPU6050 mpu6050;

void sensor_task_init(void);

#endif