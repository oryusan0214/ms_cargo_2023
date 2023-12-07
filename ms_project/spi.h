/* -------------------------------------------------------------------------- */
/* start.ino																 																  */
/* 初期化機能												 												                  */
/* アプリケーションエントリーポイントです。(Arduinoではここが								  */
/*																					エントリポジションです。)					*/
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* 番号		更新履歴								日付　氏名																  */
/* -------------------------------------------------------------------------- */
/* 000000	新規作成								2023/12/04　山田龍之介		 								  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* includeファイル															  														*/
/* -------------------------------------------------------------------------- */
#pragma once
#include "common.h"
#include <Arduino.h>
#include <SPI.h>
#include <stdint.h>


/* -------------------------------------------------------------------------- */
/* Define定義															                                    */
/* -------------------------------------------------------------------------- */
#define FLOATSIZE sizeof(float) 
#define UINTSIZE sizeof(uint16_t)
#define UCHRSIZE sizeof(UCHR)

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言															                              */
/* -------------------------------------------------------------------------- */
void init_spi(void);

/* -------------------------------------------------------------------------- */
/* 構造体定義                                                                 */
/* -------------------------------------------------------------------------- */
struct send_data
{
  float xangle;
  float yangle;
  float zangle;
  UINT  weight;
  UCHR  error;
  UCHR  lf_neemotor;
  UCHR  lf_yaw_hipjointmotor;
  UCHR  lf_pitch_hipjointmotor;
  UCHR  lc_neemotor;
  UCHR  lc_yaw_hipjointmotor;
  UCHR  lc_pitch_hipjointmotor;
  UCHR  lb_kneemotor;
  UCHR  lb_yaw_hipjointmotor;
  UCHR  lb_pitch_hipjointmotor;
  UCHR  rf_neemotor;
  UCHR  rf_yaw_hipjointmotor;
  UCHR  rf_pitch_hipjointmotor;
  UCHR  rc_neemotor;
  UCHR  rc_yaw_hipjointmotor;
  UCHR  rc_pitch_hipjointmotor;
  UCHR  rb_kneemotor;
  UCHR  rb_yaw_hipjointmotor;
  UCHR  rb_pitch_hipjointmotor;
  UCHR  l_armmotor;
  UCHR  r_armmotor;
  UCHR  elevator;
  UCHR  hund; 
  
}typedef send_data;

struct catch_data
{
  uint16_t  lf_neemotor;
  uint16_t  lf_yaw_hipjointmotor;
  uint16_t  lf_pitch_hipjointmotor;
  uint16_t  lc_neemotor;
  uint16_t  lc_yaw_hipjointmotor;
  uint16_t  lc_pitch_hipjointmotor;
  uint16_t  lb_kneemotor;
  uint16_t  lb_yaw_hipjointmotor;
  uint16_t  lb_pitch_hipjointmotor;
  uint16_t  rf_neemotor;
  uint16_t  rf_yaw_hipjointmotor;
  uint16_t  rf_pitch_hipjointmotor;
  uint16_t  rc_neemotor;
  uint16_t  rc_yaw_hipjointmotor;
  uint16_t  rc_pitch_hipjointmotor;
  uint16_t  rb_kneemotor;
  uint16_t  rb_yaw_hipjointmotor;
  uint16_t  rb_pitch_hipjointmotor;
  uint16_t  l_armmotor;
  uint16_t  r_armmotor;
  uint16_t  elevator;
  CHAR  error;
  CHAR  hundopen;
  CHAR  calibration;
}typedef catch_data;

/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														                              */
/* -------------------------------------------------------------------------- */
send_data senderdata;
catch_data catcherdata;
