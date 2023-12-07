#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\time.h"
/* -------------------------------------------------------------------------- */
/* time.h																 																      */
/* 時間に関する管理を纏めたヘッダ。											                      */
/* 外部公開ヘッダ															                                */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* 番号		更新履歴								日付　氏名																  */
/* -------------------------------------------------------------------------- */
/* 000000	新規作成								2013/05/11	桝井　隆治	                    */
/* 000001	Arduino版に改修					2023/12/04  山田　龍之介		 					      */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* includeファイル															  														*/
/* -------------------------------------------------------------------------- */
#pragma once
#include "common.h"
#include "time.h"
#include "log.h"
#include <Arduino.h>
#include <TimerOne.h>

/* -------------------------------------------------------------------------- */
/* define定義															  														      */
/* -------------------------------------------------------------------------- */
#define MS_TIME_NUM_MAX          (         50 )	/* コールバック管理上限		    */
#define MS_TIME_OK               (          0 )	/* 正常終了					          */
#define MS_TIME_NG               (         -1 )	/* システムエラー			        */
#define MS_TIME_PARAM            (         -2 )	/* パラメータエラー			      */
#define MS_TIME_FULL             (         -3 )	/* タイマー設定上限エラー	    */


/* -------------------------------------------------------------------------- */
/* 構造体定義																                                  */
/* -------------------------------------------------------------------------- */

/* タイマーコールバックテーブル */
typedef struct _MS_TIMER_CALLBACK_TBL {
	ULNG  CallbackTime;							      /* タイマー満了時間			      */
	ULNG  TimerCounter;							      /* タイマーカウンター		      */
	void* object;								          /* タイマーコールバック引数   */
	void (*Callback)( void* object );			/* タイマーコールバック関数   */
	SLNG TimerID;								          /* 発行済みタイマーID		      */
} MS_TIMER_CALLBACK_TBL;

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言															                              */
/* -------------------------------------------------------------------------- */
void msTimeInit( void );
void msTimeExit( SLNG );
SLNG msTimeKill( SLNG );
ULNG msTimeGetSystemTime( void );
void msTimeCallbackExecute( void );
void msTimeInit( void );
SLNG msSetTimer( ULNG CallBackTime, void* Object, void (*Callback)(void* objectt ));

//unsigned long intにしないとオーバーフローする
//その場合は32bitの場合で49.71026961805556日持つ
volatile unsigned long timerCount = 0;
char commonTimer();
char setTimer();
char delateTimer();

/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				                */
/* -------------------------------------------------------------------------- */
