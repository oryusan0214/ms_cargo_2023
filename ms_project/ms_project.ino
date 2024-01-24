/* -------------------------------------------------------------------------- */
/* ms_project.ino																 															*/
/* シ  ス  テ  ム  メ  イ  ン												 												  */
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
#include "Arduino.h"
#include <SPI.h>
#include "test.h"
#include "time.h"
#include "log.h"
#include "servo.h"
#include "dc.h"
#include "rod.h"
#include "hand.h"
/* ------------------------------------------------------------------------ */
/* ArduinoMegaでの変数のサイズです */
//int=1byte
//short=2byte
//long=4byte

void setup()
{
	Serial.begin(115200);
	//SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE2);
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
  msTimeInit();
  msLogInit();
  msServoInit();
  msDCInit();
  msHANDInit();
  msRODInit();
  SPI.begin();
  digitalWrite(SS, HIGH);
  msLog("System Start");
  delay(1);
}

void loop()
{
	scene();
  msTimeCallbackExecute();

}
