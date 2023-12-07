#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\spi.cpp"
#include "spi.h"
void init_spi(){
  
  SCHR catchdata[45];
  SCHR senderdata[45];
  SCHR datacount = 0;
  SCHR sender;

  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE2);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  digitalWrite(55, LOW);
  *(float*)senderdata[0]=senderdata.xangle;
  *(float*)senderdata[4]=senderdata.yangle;
  *(float*)senderdata[8]=senderdata.zangle;
  *(uint16_t*)senderdata[12]=senderdata.weight;
  *(UCHR*)senderdata[14]=senderdata.error;
  *(UCHR*)senderdata[15]=senderdata.lf_neemotor;
  *(UCHR*)senderdata[16]=senderdata.lf_yaw_hipjointmotor;
  *(UCHR*)senderdata[17]=senderdata.lf_pitch_hipjointmotor;
  *(UCHR*)senderdata[18]=senderdata.lc_neemotor;
  *(UCHR*)senderdata[19]=senderdata.lc_yaw_hipjointmotor;
  *(UCHR*)senderdata[20]=senderdata.lc_pitch_hipjointmotor;
  *(UCHR*)senderdata[21]=senderdata.lb_kneemotor;
  *(UCHR*)senderdata[22]=senderdata.lb_yaw_hipjointmotor;
  *(UCHR*)senderdata[23]=senderdata.lb_pitch_hipjointmotor;
  *(UCHR*)senderdata[24]=senderdata.rf_neemotor;
  *(UCHR*)senderdata[25]=senderdata.rf_yaw_hipjointmotor;
  *(UCHR*)senderdata[26]=senderdata.rf_pitch_hipjointmotor;
  *(UCHR*)senderdata[27]=senderdata.rc_neemotor;
  *(UCHR*)senderdata[28]=senderdata.rc_yaw_hipjointmotor;
  *(UCHR*)senderdata[29]=senderdata.rc_pitch_hipjointmotor;
  *(UCHR*)senderdata[30]=senderdata.rb_kneemotor;
  *(UCHR*)senderdata[31]=senderdata.rb_yaw_hipjointmotor;
  *(UCHR*)senderdata[32]=senderdata.rb_pitch_hipjointmotor;
  
  for ( datacount = 0; datacount < 45; datacount++)
  {
    if (datacount<=32)
    {
      sender=senderdata[datacount];
      catchdata[datacount]=SPI.transfer(sender);
    }
    else
    {
      sender=0x00;
      catchdata[datacount]=SPI.transfer(sender);
    }
    
  }

  senderdata.xangle=*(const float*)catchdata[0];
  senderdata.yangle=*(const float*)catchdata[4];
  senderdata.zangle=*(const float*)catchdata[8];
  senderdata.weight=*(const uint16_t*)catchdata[12];
  senderdata.error=*(UCHR*)catchdata[14];
  senderdata.lf_neemotor=*(UCHR*)catchdata[15];
  senderdata.lf_yaw_hipjointmotor=*(UCHR*)catchdata[16];
  senderdata.lf_pitch_hipjointmotor=*(UCHR*)catchdata[17];
  senderdata.lc_neemotor=*(UCHR*)catchdata[18];
  senderdata.lc_yaw_hipjointmotor=*(UCHR*)catchdata[19];
  senderdata.lc_pitch_hipjointmotor=*(UCHR*)catchdata[20];
  senderdata.lb_kneemotor=*(UCHR*)catchdata[21];
  senderdata.lb_yaw_hipjointmotor=*(UCHR*)catchdata[22];
  senderdata.lb_pitch_hipjointmotor=*(UCHR*)catchdata[23];
  senderdata.rf_neemotor=*(UCHR*)catchdata[24];
  senderdata.rf_yaw_hipjointmotor=*(UCHR*)catchdata[25];
  senderdata.rf_pitch_hipjointmotor=*(UCHR*)catchdata[26];
  senderdata.rc_neemotor=*(UCHR*)catchdata[27];
  senderdata.rc_yaw_hipjointmotor=*(UCHR*)catchdata[28];
  senderdata.rc_pitch_hipjointmotor=*(UCHR*)catchdata[29];
  senderdata.rb_kneemotor=*(UCHR*)catchdata[30];
  senderdata.rb_yaw_hipjointmotor=*(UCHR*)catchdata[31];
  senderdata.rb_pitch_hipjointmotor=*(UCHR*)catchdata[32];
  digitalWrite(55, HIGH); 
}