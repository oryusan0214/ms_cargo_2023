#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\rod.cpp"
/* -------------------------------------------------------------------------- */
/* rod.cpp																  */
/* RODモーター制御に関わる処理												  */
/* 出力系はRODと同じ処理方法で対応すると良い								  */
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
#include "rod.h"									/* RODに関わるヘッダ		  */

/* -------------------------------------------------------------------------- */
/* 構造体定義（ローカル）													  */
/* -------------------------------------------------------------------------- */
typedef struct {
	SLNG timerid;								/* タイマーID				  */
	UCHR busyflg;								/* ビジーフラグ(ROD個数分)	 */
	SSHT olddistance;							/* 前回設定距離たち			  */
} ROD_MNG;

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言(ローカル)												  */
/* -------------------------------------------------------------------------- */
void msRODInitRecord(ROD_MNG* mng);
void msRODInterrupt();


/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														  */
/* -------------------------------------------------------------------------- */
ROD_MNG r_Mng;									/* ROD管理データ				  */
SINT	RODAngle;								/*  */
SINT	RODCount;

double RODSetpoint, RODInput, RODOutput;     			/* PIDの関数に用いる変数L	 */
volatile SINT	RODSpeed = 0;            			/* 出力値			       */

/* Gain */
double RODKp=4.00;
double RODKi=0.00;
double RODKd=0.20;

/* PIDの変数宣言 */
PID RODmyPID(&RODInput, &RODOutput, &RODSetpoint, RODKp, RODKi, RODKd, DIRECT);

PCA9685 rPwm		= PCA9685(0x43);    		/* DCのI2Cアドレス		 */
///

/* -------------------------------------------------------------------------- */
/* 関数名		：msRODInit												  */
/* 機能名		：RODモータの初期化処理										  */
/* 機能概要		：グローバル変数等の初期化を行います						  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msRODInit(void)
{
	msRODInitRecord(&r_Mng);

	RODAngle = ROD_INIT;
	RODCount = 0;
	RODInput = RODAngle;
	RODSetpoint = 0;

	/* Pin Assign */
	pinMode(ROD_PIN, OUTPUT);
	pinMode(ROD_DIR_PIN, OUTPUT);
	pinMode(ROD_END1_PIN, INPUT);
	pinMode(ROD_END2_PIN, INPUT);

	attachInterrupt(digitalPinToInterrupt(ROD_END1_PIN), msRODInterrupt, CHANGE);

	/* PID Setup */
	RODmyPID.SetMode(AUTOMATIC);
	RODmyPID.SetSampleTime(ROD_PERIOD_TIME);
	RODmyPID.SetOutputLimits( (-1 * ROD_SPEED), ROD_SPEED);

	/* pwm setup */
	rPwm.begin();					            /* 初期設定 (アドレス0x40用) */
  	pinMode(SDA,INPUT);
	pinMode(SCL,INPUT);
	Wire.setClock(200000);            			/* Clock設定               */
	rPwm.setPWMFreq(1000);			        	/* PWM周期を60Hzに設定 (アドレス0x40用) */
	rPwm.setPWM(0, 0, 0);              		/* PWM設定                 */

	/* 進行方向の初期設定 */
	digitalWrite( ROD_DIR_PIN, 1 ); 		/* 正転                    */
	
	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msRODInitRecord											  */
/* 機能名		：RODモータの初期化処理(１レコード)							  */
/* 機能概要		：															  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msRODInitRecord(ROD_MNG* mng)
{
	if (mng == NULL) {
		msLog("これもう無理やで");
		return;
	}
	/* １レコード初期化 */
	mng->timerid = 0;
	mng->busyflg = ROD_READY;
	mng->olddistance = 0;
	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msRODGetBusy											  */
/* 機能名		：RODビジー状態取得										  */
/* 機能概要		：現在のRODモーターが動作中か否かを返却します。			  */
/*				：本開発でのRODは1個固定なので、引数データも1個用意する事	*/
/* 引数			：UCHR*		: busyflags	 ：[OUT] フラグ(1個のみ動作)		  */
/* 戻り値		：SLNG		: MS_ROD_OK	：正常終了						  */
/*				：			: MS_ROD_PARAM：引数異常						  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
SLNG msRODGetBusy(UCHR* busyflags)
{
	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if (busyflags == NULL) {
		msLog("引数エラー４");
		return ROD_PARAM;
	}

	/* ビジーデータコピーして返却 */
	*busyflags = r_Mng.busyflg;

	return ROD_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msRODGetSet												  */
/* 機能名		：ROD角度設定												  */
/* 機能概要		：RODモーターの角度設定を行います。							  */
/*	##			：１度角度を移動するのに〇〇秒要すると定義し、現在の角度に対し*/
/*				：相対移動量×〇〇秒でタイマーをセットし、管理する。		  */
/*				：busyフラグが立っている所は設定せず、それ以外のみ設定する。  */
/*				：事前に必ず仕様を良く理解して使う事。						  */
/* 引数			：SLNG*		: returns	 ：[OUT] 各RODの設定が完了可否		  */
/*				：			:			 ： MS_ROD_OK    :設定成功		  */
/*				：			:			 ： MS_ROD_BUSY  :ビジーの為失敗    */
/*				：			:			 ： MS_ROD_PARAM :角度がおかしい    */
/*				：SSHT		: distance	 ：[I N] 各RODの設定距離			  */
/*				：			:			 ：何もしない場合はMS_ROD_NOSET	  */
/* 戻り値		：SLNG		: MS_ROD_OK	：正常終了						  */
/*				：			: MS_ROD_PARAM：引数異常						  */
/* ■■注意■■ ：正常終了を返しても、第一引数のreturnsはエラーの場合がある為 */
/*				：必ずチェックする事！										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
SLNG msRODSet(SLNG* returns, uint16_t* distance)
{
	SLNG rodRet = ROD_OK;
	int speed = 0;

	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if ((returns == NULL) || (distance == NULL)) {
		msLog("引数エラー５");
		return ROD_PARAM;
	}

	/* RODがビジー時は上位層の設定ミス */
	if (r_Mng.busyflg == ROD_BUSY) {
		*returns = ROD_BUSY;
	}
	/* ##要確認：RODの角度範囲がおかしい場合はパラメータエラー */
	if ((*distance < ROD_DST_MIN) || ((*distance > ROD_DST_MAX) && (*distance != ROD_NOSET))) {
		*returns = ROD_PARAM;
	}
	/* RODモーター設定可能と判断 --------------------------------------*/

	/* 指定角度からROD移動に必要な時間を算出 */
	SSHT dTmpDistance = 0;
	bool rodUD = false;						/* 回転方向を判断する				 */
											/* 初期は逆転(マイナス方向)			 */
	dTmpDistance = r_Mng.olddistance - *distance;
	/* マイナス角度をプラスに補正 */
	if (dTmpDistance < 0) {
		dTmpDistance = dTmpDistance * -1;
		rodUD = true;						/* 正転に変更					 */
	}
	/* ##define値を確認 移動予定角度から時間へ変換 */
	dTmpDistance = dTmpDistance * ROD_MOVETIME;
	if(dTmpDistance == 0){
			dTmpDistance++;
	}

	/* タイマー計算＆コールバック設定 */
	rodRet = msSetTimer(dTmpDistance, &r_Mng, msRODTimerCallback);
	if ((rodRet == MS_TIME_FULL) || (rodRet == MS_TIME_PARAM)) {
		msLog("タイマー関連エラー: %d", rodRet);
		return ROD_NG;
	}
	r_Mng.busyflg = ROD_BUSY;
	/* タイマーIDを保管 */
	r_Mng.timerid = rodRet;

	/* 指定角度を古くしておく */
	r_Mng.olddistance = *distance;

	/* ##サーボモーターのレジスタ設定 */
	/* PID設定 */
	/*
	RODInput 		= RODAngle;
	RODSetpoint 	= r_Mng.olddistance;
	RODmyPID.Compute();						/* PID演算 */
	/*speed = abs((int)RODOutput);				/* 出力値格納 */
	/*speed = map(speed,0,ROD_SPEED,0,4095);/* パルス幅の値に変換 */
	//Serial.println("---koko---");
	/* 方向指示 + PWM設定 */
	if(rodUD == true) {
		digitalWrite(ROD_DIR_PIN,HIGH);
		rPwm.setPWM(ROD_PIN, 0, ROD_SPEED);
	}else{
		digitalWrite(ROD_DIR_PIN,LOW);
		rPwm.setPWM(ROD_PIN, 0, ROD_SPEED);
	}
	/* 必要ならディレイ */
  	// delay(1);
	
	return ROD_OK;
}
/* -------------------------------------------------------------------------- */
/* 関数名		：msRODTimerCallback										  */
/* 機能名		：サーボモータが指定角度まで動いた（はず）					  */
/* 機能概要		：モーターのbusy状態を解除します。							  */
/* 				：優先度を付ける必要がある場合は、ここでソート処理を行うこと。*/
/* 引数			：void			 ：無し										  */
/* 戻り値		：void			 ：無し										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msRODTimerCallback(void* addr)
{
	ROD_MNG* ptr = (ROD_MNG*)addr;
	SSHT tmpDistance = 0;
	/* 該当のタイマーカット */
	msTimeKill(ptr->timerid);

	rPwm.setPWM(ROD_PIN, 0, 0);

	/* 角度情報を逃がす */
	tmpDistance = ptr->olddistance;

	/* 設定情報クリア(Ready状態に戻す) */
	msRODInitRecord(ptr);

	/* 角度情報を戻す */
	ptr->olddistance = tmpDistance;

	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msRODInterrupt 											  */
/* 機能名		：RODモータの割込処理の初期設定									  */
/* 機能概要		：						  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2023/12/08	筈尾　辰也		新規作成					  */
/* -------------------------------------------------------------------------- */
void msRODInterrupt(){
	/* 変化がL→Hの場合 */
	if(digitalRead(ROD_END1_PIN) == HIGH){
		if(digitalRead(ROD_END2_PIN) == LOW){
			RODCount--;
		}else{
			RODCount++;
		}
	}else{/* 変化がH→Lの場合 */
		if(digitalRead(ROD_END2_PIN) == LOW){
			RODCount++;
		}else{
			RODCount--;
		}
	}

	/* カウンタが分解能数に達した時=一回転 */
	if(RODCount > ROD_END_PRT){
		RODCount = RODCount - ROD_END_PRT;
		RODAngle++;
	}else if(RODCount < -ROD_END_PRT){
		RODCount = RODCount + ROD_END_PRT;
		RODAngle++;
	}
}

/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				  */
/* -------------------------------------------------------------------------- */
