#include "test.h"
int test()
{
  SCHR c_test[4] = {0, 0, 0, 0};
  float f_test = 0.123456789;  
  SCHR c_test_output[10]={0,0,0,0,0,0,0,0,0,0};
  SCHR f_test_output[10]={0,0,0,0,0,0,0,0,0,0};
  

  Serial.println("\n--- START ----\n");
  Serial.print("--- float ---");
  sprintf(f_test_output,"%#x%#x%#x%#x",f_test);
  Serial.println(f_test_output);
  

  Serial.print("--- char ---");
  *(float*)&c_test = (char)f_test;

  sprintf(c_test_output,"%#x%#x%#x%#x",c_test[0],c_test[1],c_test[2],c_test[3]);
  Serial.println(c_test_output);
  Serial.println("\n--- END ----\n");
  return 0;
}