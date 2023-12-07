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

  catcherdata.lf_neemotor=*(uint16_t*)catchdata[0];
  catcherdata.lf_yaw_hipjointmotor=*(uint16_t*)catchdata[2];
  catcherdata.lf_pitch_hipjointmotor=*(uint16_t*)catchdata[4];
  catcherdata.lc_neemotor=*(uint16_t*)catchdata[6];
  catcherdata.lc_yaw_hipjointmotor=*(uint16_t*)catchdata[8];
  catcherdata.lc_pitch_hipjointmotor=*(uint16_t*)catchdata[10];
  catcherdata.lb_kneemotor=*(uint16_t*)catchdata[12];
  catcherdata.lb_yaw_hipjointmotor=*(uint16_t*)catchdata[14];
  catcherdata.lb_pitch_hipjointmotor=*(uint16_t*)catchdata[16];
  catcherdata.rf_neemotor=*(uint16_t*)catchdata[18];
  catcherdata.rf_yaw_hipjointmotor=*(uint16_t*)catchdata[20];
  catcherdata.rf_pitch_hipjointmotor=*(uint16_t*)catchdata[22];
  catcherdata.rc_neemotor=*(uint16_t*)catchdata[24];
  catcherdata.rc_yaw_hipjointmotor=*(uint16_t*)catchdata[26];
  catcherdata.rc_pitch_hipjointmotor=*(uint16_t*)catchdata[28];
  catcherdata.rb_kneemotor=*(uint16_t*)catchdata[30];
  catcherdata.rb_yaw_hipjointmotor=*(uint16_t*)catchdata[32];
  catcherdata.rb_pitch_hipjointmotor=*(uint16_t*)catchdata[34];
  catcherdata.l_armmotor=*(uint16_t*)catchdata[36];
  catcherdata.r_armmotor=*(uint16_t*)catchdata[38];
  catcherdata.elevator=*(uint16_t*)catchdata[40];
  catcherdata.error=*(UCHR*)catchdata[42];
  catcherdata.hundopen=*(UCHR*)catchdata[43];
  catcherdata.calibration=*(UCHR*)catchdata[44];
  

  digitalWrite(55, HIGH); 
}