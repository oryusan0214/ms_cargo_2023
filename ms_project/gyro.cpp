#pragma once
#include "gyro.h"

void gyro_init(){
  const int MPU_addr=0x68;  // I2C address of the MPU-6050
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
}
void gyro_get(uint16_t *gyro_data ){
  const int MPU_addr=0x68;  // I2C address of the MPU-6050
  int16_t AcX,AcY,AcZ,GyX,GyY,GyZ;
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
  gyro_data[0]=AcX;
  gyro_data[1]=AcY;
  gyro_data[2]=AcZ;
  gyro_data[3]=GyX;
  gyro_data[4]=GyY;
  gyro_data[5]=GyZ;
}
