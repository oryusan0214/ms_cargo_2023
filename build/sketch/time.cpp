#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\time.cpp"
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
#include "time.h"


char commonTimer(){
  timerCount++;
}

unsigned long get_commonTimer(){
  unsigned long timerCountCopy = 0;
  timerCountCopy = timerCount;
  return timerCountCopy;
}

char initTimer(short microtime){
	Timer1.initialize(microtime);
	Timer1.attachInterrupt(commonTimer);
}
char set_Timer(short microtime){
  
}
char delateTimer(){
  
}
