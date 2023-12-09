
#pragma once
#include "common.h"
#include <Arduino.h>
#include <Wire.h>

/* -------------------------------------------------------------------------- */ 
/*プロトタイプ宣言                                                            */
/* -------------------------------------------------------------------------- */
void gyro_init();
void gyro_get(uint16_t *gyro_data );
