#include "test.h"
int test()
{
  SCHR c_test[4] = {0, 0, 0, 0};
  float f_test = 0.123456789;  
  SCHR c_test_output[10]={0,0,0,0,0,0,0,0,0,0};
  SCHR f_test_output[10]={0,0,0,0,0,0,0,0,0,0};
  

  Serial.println("--- START ----\n");
  Serial.print("--- float ---");
  sprintf(f_test_output,"%#x",f_test);
  Serial.println(f_test_output);
  

  Serial.print("--- char ---");
  *(float*)c_test = f_test;

  sprintf(c_test_output,"%#x",c_test);
  Serial.println(c_test_output);
  Serial.println("--- END ----\n");
  return 0;
}