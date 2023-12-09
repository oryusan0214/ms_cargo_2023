#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\gyro.h"
#include "common.h"
#include <Arduino.h>
#include <Wire.h>
/* -------------------------------------------------------------------------- */ 
/*プロトタイプ宣言                                                            */
/* -------------------------------------------------------------------------- */
void gyro_init();
void gyro_get(*uint16_t);
