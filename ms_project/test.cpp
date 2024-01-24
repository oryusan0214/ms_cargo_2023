#include "test.h"
extern send_data senderdata;
extern catch_data catcherdata;

typedef struct angle{
  float xangle;
  float yangle;
  float zangle;
}angle;
//testコードを実行する関数です。
int test()
{
  //msLog("\n--- TEST START ----");

  scene();
  /*
  send_data* senderPdata;
  uint8_t c_test[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  angle angle_test={0,0,0};
  uint8_t *uint8_test ;
  uint8_t *uint8_test2 ;
  float f_test = 0.123456789;  
  float f_test2 = 0.123456789;
  
  SCHR c_test_output[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  SCHR f_test_output[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  UCHR test = 0;
  UCHR transfer = 0;
  uint16_t test2 = 0;

  */
  //dummy data 作成
  /*
  senderdata.xangle = 0.123456789;
  senderdata.yangle = 0.123456789;
  senderdata.zangle = 0.123456789;
  senderdata.weight = 0x1234;
  senderdata.error = 0x12;
  senderdata.lf_neemotor = 0x12;
  senderdata.lf_yaw_hipjointmotor = 0x12;
  senderdata.lf_pitch_hipjointmotor = 0x12;
  senderdata.lc_neemotor = 0x12;
  senderdata.lc_yaw_hipjointmotor = 0x12;
  senderdata.lc_pitch_hipjointmotor = 0x12;
  senderdata.lb_kneemotor = 0x12;
  senderdata.lb_yaw_hipjointmotor = 0x12;
  senderdata.lb_pitch_hipjointmotor = 0x12;
  senderdata.rf_neemotor = 0x12;
  senderdata.rf_yaw_hipjointmotor = 0x12;
  senderdata.rf_pitch_hipjointmotor = 0x12;
  senderdata.rc_neemotor = 0x12;
  senderdata.rc_yaw_hipjointmotor = 0x12;
  senderdata.rc_pitch_hipjointmotor = 0x12;
  senderdata.rb_kneemotor = 0x12;
  senderdata.rb_yaw_hipjointmotor = 0x12;
  senderdata.rb_pitch_hipjointmotor = 0x12;
  senderdata.l_armmotor = 0x12;
  senderdata.r_armmotor = 0x12;
  senderdata.elevator = 0x12;
  senderdata.hund = 0x12;
  senderdata.reservation1 = 0x12;
  senderdata.reservation2 = 0x12;
  senderdata.reservation3 = 0x12;
  
  */
  
  /*
  senderPdata=&senderdata;
  memcpy(&angle_test,senderPdata,sizeof(angle_test));
  msLog(angle_test.xangle);
  msLog(angle_test.yangle);
  msLog(angle_test.zangle);
  memcpy(test2,&senderPdata->weight,sizeof(test2));
  msLog(test2);
  */

  //spi();
  /*
  msLog("--- float ---");
  sprintf(f_test_output,"%#x%#x",f_test);
  msLog(f_test_output);
  sprintf(f_test_output,"%#x%#x",f_test2);
  msLog(f_test_output);
  

  msLog("--- char ---");
  c_test[0] = (uint8_t) ((f_test & 0xff000000) >> 24);
  c_test[1] = (uint8_t) ((f_test & 0x00ff0000) >> 16);
  c_test[2] = (uint8_t) ((f_test & 0x0000ff00) >> 8);
  c_test[3] = (uint8_t) ((f_test & 0x000000ff) >> 0);

  c_test[4] = (uint8_t) ((f_test & 0xff000000) >> 24);
  c_test[5] = (uint8_t) ((f_test & 0x00ff0000) >> 16);
  c_test[6] = (uint8_t) ((f_test & 0x0000ff00) >> 8);
  c_test[7] = (uint8_t) ((f_test & 0x000000ff) >> 0);
  */
  /*
  uint8_test = (uint8_t *) &f_test;
  msLog(uint8_test[0],HEX);
  msLog(uint8_test[1],HEX);
  msLog(uint8_test[2],HEX);
  msLog(uint8_test[3],HEX);

  uint8_test = (uint8_t*) &f_test2;
  msLog(uint8_test[0],HEX);
  msLog(uint8_test[1],HEX);
  msLog(uint8_test[2],HEX);
  msLog(uint8_test[3],HEX);
  
  memccpy(c_test, &f_test, sizeof(f_test), sizeof(f_test));
  memccpy(c_test+4, &f_test2, sizeof(f_test2),sizeof(f_test2));
  msLog("--- char ---");
  msLog(c_test[0],HEX);
  msLog(c_test[1],HEX);
  msLog(c_test[2],HEX);
  msLog(c_test[3],HEX);
  msLog(c_test[4],HEX);
  msLog(c_test[5],HEX);
  msLog(c_test[6],HEX);
  msLog(c_test[7],HEX);
  */ 
  

  

  //msLog("\n--- END ----");





  return 0;
}