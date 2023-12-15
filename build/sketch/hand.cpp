#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\hand.cpp"
/* -------------------------------------------------------------------------- */
/* hand.cpp																  */
/* HANDモーター制御に関わる処理												  */
/* 出力系はHANDと同じ処理方法で対応すると良い								  */
/* -------------------------------------------------------------------------- */
/* 番号		更新履歴								日付		氏名		  */
/* -------------------------------------------------------------------------- */
/* 000000	新規作成								2013/05/08	桝井　隆治	  */
/* 000001	機能追加								2023/12/06	筈尾　辰也		 */
/* -------------------------------------------------------------------------- */
#define _CRT_SECURE_NO_WARNINGS

/* -------------------------------------------------------------------------- */
/* includeファイル															  */
/* -------------------------------------------------------------------------- */
#include <stdio.h>								/* 基本的なＩ／Ｏ情報		  */
#include <string.h>								/* 初期化関連				  */
#include <stdarg.h>								/* システムログ				  */
#include <stdlib.h>								/* メモリ操作				  */
#include <stdint.h>								/* uint周り					 */
#include <Wire.h>
#include <PCA9685.h>                            /* PCA9685用ヘッダーファイル     */
#include <PID_v1.h>								/*  */
#include "time.h"								/* 時間に関するヘッダ		  */
#include "log.h"								/* ログに関わるヘッダ		  */
#include "hand.h"								/* HANDに関わるヘッダ		  */

/* -------------------------------------------------------------------------- */
/* 構造体定義（ローカル）													  */
/* -------------------------------------------------------------------------- */
typedef struct {
	SLNG timerid;								/* タイマーID				  */
	UCHR busyflg;								/* ビジーフラグ(HAND個数分)	 */
	SSHT oldstate;							    /* 前回設定開閉情報たち 		  */
} HAND_MNG;

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言(ローカル)												  */
/* -------------------------------------------------------------------------- */
void msHANDInitRecord(HAND_MNG* mng);


/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														  */
/* -------------------------------------------------------------------------- */
HAND_MNG h_Mng;									/* HAND管理データ				  */
PCA9685 hPwm		= PCA9685(0x43);    		/* DCのI2Cアドレス		 */

/* -------------------------------------------------------------------------- */
/* 関数名		：msHANDInit												  */
/* 機能名		：HANDモータの初期化処理										  */
/* 機能概要		：グローバル変数等の初期化を行います						  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msHANDInit(void)
{
	/* プログラム起動時に一度だけ走る初期化処理 */
	msHANDInitRecord(&h_Mng);

	/* Pin Assign */
	pinMode(HAND_PIN, OUTPUT);
	pinMode(HAND_DIR_PIN, OUTPUT);

	/* pwm setup */
	hPwm.begin();					            /* 初期設定 (アドレス0x40用) */
  	Wire.setClock(400000);            			/* Clock設定               */
	hPwm.setPWMFreq(1000);			        	/* PWM周期を60Hzに設定 (アドレス0x40用) */
	hPwm.setPWM(0, 0, 0);              		/* PWM設定                 */

	/* 進行方向の初期設定 */
	digitalWrite( HAND_DIR_PIN, 1 ); 		/* 正転                    */
	
	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msHANDInitRecord											  */
/* 機能名		：HANDモータの初期化処理(１レコード)							  */
/* 機能概要		：															  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msHANDInitRecord(HAND_MNG* mng)
{
	if (mng == NULL) {
		msLog("これもう無理やで");
		return;
	}
	/* １レコード初期化 */
	mng->timerid = 0;
	mng->busyflg = HAND_READY;
	mng->oldstate = 0;
	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msHANDGetBusy											  */
/* 機能名		：HANDビジー状態取得										  */
/* 機能概要		：現在のHANDモーターが動作中か否かを返却します。			  */
/*				：本開発でのHANDは1個固定なので、引数データも1個用意する事	*/
/* 引数			：UCHR*		: busyflags	 ：[OUT] フラグ(1個のみ動作)		  */
/* 戻り値		：SLNG		: MS_HAND_OK	：正常終了						  */
/*				：			: MS_HAND_PARAM：引数異常						  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
SLNG msHANDGetBusy(UCHR* busyflags)
{
	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if (busyflags == NULL) {
		msLog("引数エラー");
		return HAND_PARAM;
	}

	/* ビジーデータコピーして返却 */
	*busyflags = h_Mng.busyflg;

	return HAND_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msHANDGetSet												  */
/* 機能名		：HAND角度設定												  */
/* 機能概要		：HANDモーターの角度設定を行います。							  */
/*	##			：１度角度を移動するのに〇〇秒要すると定義し、現在の角度に対し*/
/*				：相対移動量×〇〇秒でタイマーをセットし、管理する。		  */
/*				：busyフラグが立っている所は設定せず、それ以外のみ設定する。  */
/*				：事前に必ず仕様を良く理解して使う事。						  */
/* 引数			：SLNG*		: returns	 ：[OUT] 各HANDの設定が完了可否		  */
/*				：			:			 ： MS_HAND_OK    :設定成功		  */
/*				：			:			 ： MS_HAND_BUSY  :ビジーの為失敗    */
/*				：			:			 ： MS_HAND_PARAM :角度がおかしい    */
/*				：bool		: setting	 ：[I N] 各HANDの設定   			  */
/*				：			:			 ：何もしない場合はMS_HAND_NOSET	  */
/* 戻り値		：SLNG		: MS_HAND_OK	：正常終了						  */
/*				：			: MS_HAND_PARAM：引数異常						  */
/* ■■注意■■ ：正常終了を返しても、第一引数のreturnsはエラーの場合がある為 */
/*				：必ずチェックする事！										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
SLNG msHANDSet(SLNG* returns, bool setting)
{
	SLNG handRet = HAND_OK;

	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if (returns == NULL) {
		msLog("引数エラー");
		return HAND_PARAM;
	}

	/* HANDがビジー時は上位層の設定ミス */
	if (h_Mng.busyflg == HAND_BUSY) {
		return HAND_BUSY;
	}
	/* ##要確認：HANDの角度範囲がおかしい場合はパラメータエラー */
	if ((setting <false) || (true < setting) && (setting != HAND_NOSET)) {
		return HAND_PARAM;
	}
	/* HANDモーター設定可能と判断 --------------------------------------*/


	/* 指定角度からHAND移動に必要な時間を算出 */
	uint8_t handUD = 0;  					/* 回転方向を判断する				 */
											/* 初期は逆転(マイナス方向)			 */

    /* 開閉方向判断 */
	/* 同じ→変化なし(1)、Low → High 開く(1)、High → Low 閉じる(2) */
    if(h_Mng.oldstate == setting){
        handUD = 0;
    }else if(h_Mng.oldstate < setting){
        handUD = 1;
    }else{
        handUD = 2;
    }
	/* タイマー計算＆コールバック設定 */
	handRet = msSetTimer(HAND_MOVETIME, &h_Mng, msHANDTimerCallback);
	if ((handRet == MS_TIME_FULL) || (handRet == MS_TIME_PARAM)) {
		msLog("タイマー関連エラー: %d", handRet);
		return HAND_NG;
	}
	/* タイマーIDを保管 */
	h_Mng.timerid = handRet;

	/* 指定角度を古くしておく */
	h_Mng.oldstate = setting;

	/* ##HANDモーターのレジスタ設定 */
	if(handUD == 0) {
		hPwm.setPWM(HAND_PIN, 0, 0);
	}else if(handUD == 1){
		digitalWrite(HAND_DIR_PIN,HIGH);
		hPwm.setPWM(HAND_PIN, 0, HAND_SPEED);
	}else{
        digitalWrite(HAND_DIR_PIN,LOW);
		hPwm.setPWM(HAND_PIN, 0, HAND_SPEED);
    }
	/* 必要ならディレイ */
  	// delay(1);
	
	return HAND_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msHANDTimerCallback										  */
/* 機能名		：サーボモータが指定角度まで動いた（はず）					  */
/* 機能概要		：モーターのbusy状態を解除します。							  */
/* 				：優先度を付ける必要がある場合は、ここでソート処理を行うこと。*/
/* 引数			：void			 ：無し										  */
/* 戻り値		：void			 ：無し										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msHANDTimerCallback(void* addr)
{
	HAND_MNG* ptr = (HAND_MNG*)addr;
	SSHT tmpDistance = 0;
	/* 該当のタイマーカット */
	msTimeKill(ptr->timerid);

	/* モータの停止 */
	hPwm.setPWM(HAND_PIN, 0, 0);

	/* 角度情報を逃がす */
	tmpDistance = ptr->oldstate;

	/* 設定情報クリア(Ready状態に戻す) */
	msHANDInitRecord(ptr);

	/* 角度情報を戻す */
	ptr->oldstate = tmpDistance;

	return;
}
/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				  */
/* -------------------------------------------------------------------------- */