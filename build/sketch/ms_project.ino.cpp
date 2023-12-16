#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino"
/* -------------------------------------------------------------------------- */
/* dummy.ino																 																  */
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
#include "init.h"
#include <SPI.h>
#include "test.h"
#include "time.h"
#include "log.h"
#include "servo.h"
/* ------------------------------------------------------------------------ */
/* */
//int=1byte
//short=2byte
//long=4byte

#line 31 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino"
void setup();
#line 46 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino"
void loop();
#line 31 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino"
void setup()
{
	Serial.begin(115200);
	//SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE2);
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
  msTimeInit();
  msLogInit();
  msServoInit();
  SPI.begin();
  digitalWrite(SS, HIGH);
  Serial.println("System Start");
  delay(1);
}

void loop()
{
	test();
  msTimeCallbackExecute();
	//Serial.println("loop");
	/*
	UCHR legbusy[18]={0};
  UCHR armbusy[2]={0};
  uint16_t gyro_data[6]={0,0,0,0,0,0};

  Serial.println("--- main start ---");
  */
  /*受信データを取得*/
  
  /*busy信号検出*/
	/*
  msServoGetBusy(legbusy,sizeof(legbusy)/sizeof(legbusy[0]));
  msDCGetBusy   (armbusy,sizeof(armbusy)/sizeof(armbusy[0]));
	*/

  /*ジャイロデータ取得*/

  //gyro_get(gyro_data);

  /*送信データを作成*/  
/*
  return 0;
*/
	
	/*main();*/
}

