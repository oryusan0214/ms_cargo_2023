#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\spi.cpp"

#include "spi.h"

/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														                              */
/* -------------------------------------------------------------------------- */
send_data senderdata;
catch_data catcherdata;

void spi(){
  
  SCHR catchdata[45];
  SCHR senddata[45];
  SCHR datacount = 0;
  SCHR sender;

  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE2);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
  digitalWrite(55, LOW);
  
  *(float*)senddata[0]=senderdata.xangle;
  *(float*)senddata[4]=senderdata.yangle;
  *(float*)senddata[8]=senderdata.zangle;
  *(uint16_t*)senddata[12]=senderdata.weight;
  *(UCHR*)senddata[14]=senderdata.error;
  *(UCHR*)senddata[15]=senderdata.lf_neemotor;
  *(UCHR*)senddata[16]=senderdata.lf_yaw_hipjointmotor;
  *(UCHR*)senddata[17]=senderdata.lf_pitch_hipjointmotor;
  *(UCHR*)senddata[18]=senderdata.lc_neemotor;
  *(UCHR*)senddata[19]=senderdata.lc_yaw_hipjointmotor;
  *(UCHR*)senddata[20]=senderdata.lc_pitch_hipjointmotor;
  *(UCHR*)senddata[21]=senderdata.lb_kneemotor;
  *(UCHR*)senddata[22]=senderdata.lb_yaw_hipjointmotor;
  *(UCHR*)senddata[23]=senderdata.lb_pitch_hipjointmotor;
  *(UCHR*)senddata[24]=senderdata.rf_neemotor;
  *(UCHR*)senddata[25]=senderdata.rf_yaw_hipjointmotor;
  *(UCHR*)senddata[26]=senderdata.rf_pitch_hipjointmotor;
  *(UCHR*)senddata[27]=senderdata.rc_neemotor;
  *(UCHR*)senddata[28]=senderdata.rc_yaw_hipjointmotor;
  *(UCHR*)senddata[29]=senderdata.rc_pitch_hipjointmotor;
  *(UCHR*)senddata[30]=senderdata.rb_kneemotor;
  *(UCHR*)senddata[31]=senderdata.rb_yaw_hipjointmotor;
  *(UCHR*)senddata[32]=senderdata.rb_pitch_hipjointmotor;
  
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