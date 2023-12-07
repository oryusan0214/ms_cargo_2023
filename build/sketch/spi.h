#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\spi.h"
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
struct spi_data
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
  
}typedef spi_data;

/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														                              */
/* -------------------------------------------------------------------------- */
spi_data data;
