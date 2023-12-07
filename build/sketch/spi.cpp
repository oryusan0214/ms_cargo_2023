#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\spi.cpp"
#include "spi.h"
void init_spi(){
  
  SCHR catchdata[45];
  SCHR senddata[45];
  SCHR datacount = 0;
  SCHR sender;

  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE2);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  digitalWrite(55, LOW);
  *(float*)senddata[0]=data.xangle;
  *(float*)senddata[4]=data.yangle;
  *(float*)senddata[8]=data.zangle;
  *(uint16_t*)senddata[12]=data.weight;
  *(UCHR*)senddata[14]=data.error;
  *(UCHR*)senddata[15]=data.lf_neemotor;
  *(UCHR*)senddata[16]=data.lf_yaw_hipjointmotor;
  *(UCHR*)senddata[17]=data.lf_pitch_hipjointmotor;
  *(UCHR*)senddata[18]=data.lc_neemotor;
  *(UCHR*)senddata[19]=data.lc_yaw_hipjointmotor;
  *(UCHR*)senddata[20]=data.lc_pitch_hipjointmotor;
  *(UCHR*)senddata[21]=data.lb_kneemotor;
  *(UCHR*)senddata[22]=data.lb_yaw_hipjointmotor;
  *(UCHR*)senddata[23]=data.lb_pitch_hipjointmotor;
  *(UCHR*)senddata[24]=data.rf_neemotor;
  *(UCHR*)senddata[25]=data.rf_yaw_hipjointmotor;
  *(UCHR*)senddata[26]=data.rf_pitch_hipjointmotor;
  *(UCHR*)senddata[27]=data.rc_neemotor;
  *(UCHR*)senddata[28]=data.rc_yaw_hipjointmotor;
  *(UCHR*)senddata[29]=data.rc_pitch_hipjointmotor;
  *(UCHR*)senddata[30]=data.rb_kneemotor;
  *(UCHR*)senddata[31]=data.rb_yaw_hipjointmotor;
  *(UCHR*)senddata[32]=data.rb_pitch_hipjointmotor;
  
  for ( datacount = 0; datacount < 45; datacount++)
  {
    if (datacount<=32)
    {
      sender=senddata[datacount];
      catchdata[datacount]=SPI.transfer(sender);
    }
    else
    {
      sender=0x00;
      catchdata[datacount]=SPI.transfer(sender);
    }
    
  }

  data.xangle=*(const float*)catchdata[0];
  data.yangle=*(const float*)catchdata[4];
  data.zangle=*(const float*)catchdata[8];
  data.weight=*(const uint16_t*)catchdata[12];
  data.error=*(UCHR*)catchdata[14];
  data.lf_neemotor=*(UCHR*)catchdata[15];
  data.lf_yaw_hipjointmotor=*(UCHR*)catchdata[16];
  data.lf_pitch_hipjointmotor=*(UCHR*)catchdata[17];
  data.lc_neemotor=*(UCHR*)catchdata[18];
  data.lc_yaw_hipjointmotor=*(UCHR*)catchdata[19];
  data.lc_pitch_hipjointmotor=*(UCHR*)catchdata[20];
  data.lb_kneemotor=*(UCHR*)catchdata[21];
  data.lb_yaw_hipjointmotor=*(UCHR*)catchdata[22];
  data.lb_pitch_hipjointmotor=*(UCHR*)catchdata[23];
  data.rf_neemotor=*(UCHR*)catchdata[24];
  data.rf_yaw_hipjointmotor=*(UCHR*)catchdata[25];
  data.rf_pitch_hipjointmotor=*(UCHR*)catchdata[26];
  data.rc_neemotor=*(UCHR*)catchdata[27];
  data.rc_yaw_hipjointmotor=*(UCHR*)catchdata[28];
  data.rc_pitch_hipjointmotor=*(UCHR*)catchdata[29];
  data.rb_kneemotor=*(UCHR*)catchdata[30];
  data.rb_yaw_hipjointmotor=*(UCHR*)catchdata[31];
  data.rb_pitch_hipjointmotor=*(UCHR*)catchdata[32];
  digitalWrite(55, HIGH); 
}