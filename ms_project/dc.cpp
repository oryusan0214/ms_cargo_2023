/* -------------------------------------------------------------------------- */
/* dc.cpp																  */
/* DCモーター制御に関わる処理												  */
/* 出力系はDCと同じ処理方法で対応すると良い								  */
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
#include "time.h"								/* 時間に関するヘッダ		  */
#include "log.h"								/* ログに関わるヘッダ		  */
#include "dc.h"									/* DCに関わるヘッダ		  */
#include <stdint.h>								/*  */
#include <PID_v1.h>								/*  */
#include <Wire.h>
#include <PCA9685.h>                            /* PCA9685用ヘッダーファイル     */

/* -------------------------------------------------------------------------- */
/* 構造体定義（ローカル）													  */
/* -------------------------------------------------------------------------- */
typedef struct {
	SLNG dcid;									/* DCモータ番号				  */
	SLNG timerid;								/* タイマーID				  */
	UCHR busyflg;								/* ビジーフラグ(DC個数分)	 */
	SSHT oldangles;								/* 前回設定角度たち			  */
} DC_MNG;

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言(ローカル)												  */
/* -------------------------------------------------------------------------- */
void msDCInitRecord(DC_MNG* mng,SLNG id);
void msLDCInterrupt();
void msRDCInterrupt();


/* global */
PCA9685 dcPwm		= PCA9685(0x43);    		/* DCのI2Cアドレス		 */

/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														  */
/* -------------------------------------------------------------------------- */
DC_MNG dc_Mng[MS_DC_MAX];						/* DC管理データ				  */
SINT	LAngle;									/* 左DCの角度				 */
SINT	RAngle;									/* 右DCの角度				 */
SINT	LCount;									/* 左のDCエンコのカウント	 */ 
SINT	RCount;									/* 右のDCエンコのカウント	 */
volatile SINT	LSpeed = 0;            			/* 出力値			       */
volatile SINT	RSpeed = 0;            			/* 出力値			       */
//
double Setpoint, Input, Output;     			/* PIDの関数に用いる変数	 */

/* Gain */
double Kp=4.00;
double Ki=0.00;
double Kd=0.20;

/* PIDの変数宣言 */
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

/* -------------------------------------------------------------------------- */
/* 関数名		：msDCInit												  */
/* 機能名		：DCモータの初期化処理										  */
/* 機能概要		：グローバル変数等の初期化を行います						  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msDCInit(void)
{
	SLNG dcCounter = 0;
	for (dcCounter = 0; dcCounter < ( sizeof(dc_Mng) / sizeof(dc_Mng[0])); dcCounter++) {
		msDCInitRecord(&dc_Mng[dcCounter],dcCounter);
	}

	LAngle = MS_DC_L_INIT;
	RAngle = MS_DC_R_INIT;
	LCount = 0;
	RCount = 0;

	pinMode(MS_DC_L_PIN, OUTPUT);
	pinMode(MS_DC_L_DIR_PIN, OUTPUT);
	pinMode(MS_DC_R_PIN, OUTPUT);
	pinMode(MS_DC_R_DIR_PIN, OUTPUT);	
	pinMode(MS_DC_L_END1_PIN, INPUT);
	pinMode(MS_DC_L_END2_PIN, INPUT);
	pinMode(MS_DC_R_END1_PIN, INPUT);
	pinMode(MS_DC_R_END2_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(MS_DC_L_END1_PIN), msLDCInterrupt, CHANGE);
	attachInterrupt(digitalPinToInterrupt(MS_DC_R_END1_PIN), msRDCInterrupt, CHANGE);

	//pid2つ用意

	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msDCInitRecord											  */
/* 機能名		：DCモータの初期化処理(１レコード)							  */
/* 機能概要		：															  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msDCInitRecord(DC_MNG* mng,SLNG id)
{
	if (mng == NULL) {
		msLog("これもう無理やで");
		return;
	}
	/* １レコード初期化 */
	mng->dcid = id;
	mng->timerid = 0;
	mng->busyflg = MS_DC_READY;
	mng->oldangles = 0;
	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msDCGetBusy											  */
/* 機能名		：DCビジー状態取得										  */
/* 機能概要		：現在のDCモーターが動作中か否かを返却します。			  */
/*				：本開発でのDCは4個固定なので、引数データも4個用意する事	*/
/* 引数			：UCHR*		: busyflags	 ：[OUT] フラグ配列(4個のみ動作)	  */
/*				：USHT		: max		 ：[I N] 第一引数の配列数			  */
/* 戻り値		：SLNG		: MS_DC_OK	：正常終了						  */
/*				：			: MS_DC_PARAM：引数異常						  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
SLNG msDCGetBusy(UCHR* busyflags, USHT max)
{
	SLNG dcIndex = 0;

	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if ((busyflags == NULL) || (max != MS_DC_MAX)) {
		msLog("引数エラー");
		return MS_DC_PARAM;
	}

	/* ビジーデータコピーして返却 */
	for (dcIndex = 0; dcIndex < MS_DC_MAX; dcIndex++) {
		busyflags[dcIndex] = dc_Mng[dcIndex].busyflg;
	}
	return MS_DC_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msDCGetSet												  */
/* 機能名		：DC角度設定												  */
/* 機能概要		：DCモーターの角度設定を行います。							  */
/*	##			：１度角度を移動するのに〇〇秒要すると定義し、現在の角度に対し*/
/*				：相対移動量×〇〇秒でタイマーをセットし、管理する。		  */
/*				：busyフラグが立っている所は設定せず、それ以外のみ設定する。  */
/*				：事前に必ず仕様を良く理解して使う事。						  */
/* 引数			：SLNG*		: returns	 ：[OUT] 各DCの設定が完了可否		  */
/*				：			:			 ： MS_DC_OK    :設定成功		  */
/*				：			:			 ： MS_DC_BUSY  :ビジーの為失敗    */
/*				：			:			 ： MS_DC_PARAM :角度がおかしい    */
/*				：uint16_t*	: angles	 ：[I N] 各DCの設定角度			  */
/*				：			:			 ：何もしない場合はMS_DC_NOSET	  */
/*				：USHT		: max		 ：[I N] 第一・第二引数の配列数(18個) */
/* 戻り値		：SLNG		: MS_DC_OK	：正常終了						  */
/*				：			: MS_DC_PARAM：引数異常						  */
/* ■■注意■■ ：正常終了を返しても、第一引数のreturnsはエラーの場合がある為 */
/*				：必ずチェックする事！										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
SLNG msDCSet(SLNG* returns, uint16_t* angles, USHT max)
{
	SLNG dcCounter = 0;
	SLNG dcRet = MS_DC_OK;
	uint8_t dcAng = 0;

	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if ((returns == NULL) || (angles == NULL) || (max != MS_DC_MAX)) {
		msLog("引数エラー");
		return MS_DC_PARAM;
	}

	/* DCの個数分ループして各種設定 */
	for (dcCounter == 0; dcCounter < MS_DC_MAX; dcCounter++) {
		/* DCがビジー時は上位層の設定ミス */
		if (dc_Mng[dcCounter].busyflg == MS_DC_BUSY) {
			returns[dcCounter] = MS_DC_BUSY;
			continue;
		}
		/* ##要確認：DCの角度範囲がおかしい場合はパラメータエラー */
		if ((angles[dcCounter] < MS_DC_ANG_MIN) || ((angles[dcCounter] > MS_DC_ANG_MAX) && (angles[dcCounter] != MS_DC_NOSET))) {
			returns[dcCounter] = MS_DC_PARAM;
			continue;
		}
		/* DCモーター設定可能と判断 --------------------------------------*/
		
		/* 指定角度からDC移動に必要な時間を算出 */
		SSHT dTmpAngle = 0;
		bool dcUD = false;						/* 回転方向を判断する				 */
												/* 初期は逆転(マイナス方向)			 */
		dTmpAngle = dc_Mng[dcCounter].oldangles - angles[dcCounter];
		/* マイナス角度をプラスに補正 */
		if (dTmpAngle < 0) {
			dTmpAngle = dTmpAngle * -1;
			dcUD = true;						/* 正転に変更					 */
		}
		/* ##define値を確認 移動予定角度から時間へ変換 */
		dTmpAngle = dTmpAngle * MS_DC_MOVETIME;

		/* タイマー計算＆コールバック設定 */
		dcRet = msSetTimer(dTmpAngle, &dc_Mng[dcCounter], msDCTimerCallback);
		if ((dcRet == MS_TIME_FULL) || (dcRet == MS_TIME_PARAM)) {
			msLog("タイマー関連エラー: %d", dcRet);
			return MS_DC_NG;
		}
		/* タイマーIDを保管 */
		dc_Mng[dcCounter].timerid = dcRet;

		/* 指定角度を古くしておく */
		dc_Mng[dcCounter].oldangles = angles[dcCounter];

		/* ##サーボモーターのレジスタ設定 */
		/* 角度（0～270）をPWMのパルス幅（150～600）に変換 パルス幅要変更 */
		dcAng = map(dc_Mng[dcCounter].oldangles, MS_DC_ANG_MIN, MS_DC_ANG_MAX, DCMIN, DCMAX);

		switch(dcCounter){
		case MS_DC_L :
			
			if(dcUD == true) {///
				digitalWrite(MS_DC_L_DIR_PIN,HIGH);
				dcPwm.setPWM(dcCounter, 0, dcAng);
			}else{
				digitalWrite(MS_DC_L_DIR_PIN,LOW);
				dcPwm.setPWM(dcCounter, 0, dcAng);
			}
			/* 必要ならディレイ */
  			// delay(1);
			break;

		case MS_DC_R :
			
			if(dcUD == true) {
				digitalWrite(MS_DC_R_DIR_PIN,HIGH);
				dcPwm.setPWM(dcCounter, 0, dcAng);
			}else{
				digitalWrite(MS_DC_R_DIR_PIN,LOW);
				dcPwm.setPWM(dcCounter, 0, dcAng);
			}
			/* 必要ならディレイ */
  			// delay(1);
			break;

		default:
			printf("おかしい\n");
			break;
		}
	}
	return MS_DC_OK;
}
/* -------------------------------------------------------------------------- */
/* 関数名		：msDCTimerCallback										  */
/* 機能名		：サーボモータが指定角度まで動いた（はず）					  */
/* 機能概要		：モーターのbusy状態を解除します。							  */
/* 				：優先度を付ける必要がある場合は、ここでソート処理を行うこと。*/
/* 引数			：void			 ：無し										  */
/* 戻り値		：void			 ：無し										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msDCTimerCallback(void* addr)
{
	DC_MNG* ptr = (DC_MNG*)addr;
	SSHT tmpAngle = 0;
	/* 該当のタイマーカット */
	msTimeKill(ptr->timerid);

	if(ptr->dcid == MS_DC_L){
		dcPwm.setPWM(MS_DC_L_PIN, 0, 0);
	}else{
		dcPwm.setPWM(MS_DC_R_PIN, 0, 0);
	}

	/* 角度情報を逃がす */
	tmpAngle = ptr->oldangles;

	/* 設定情報クリア(Ready状態に戻す) */
	msDCInitRecord(ptr,ptr->dcid);

	/* 角度情報を戻す */
	ptr->oldangles = tmpAngle;

	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msDCInterrupt 											  */
/* 機能名		：DCモータの割込処理の初期設定									  */
/* 機能概要		：						  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2023/12/08	筈尾　辰也		新規作成					  */
/* -------------------------------------------------------------------------- */
void msLDCInterrupt(){
	/* 変化がL→Hの場合 */
	if(digitalRead(MS_DC_L_END1_PIN) == HIGH){
		if(digitalRead(MS_DC_L_END2_PIN) == LOW){
			LCount--;
		}else{
			LCount++;
		}
	}else{/* 変化がH→Lの場合 */
		if(digitalRead(MS_DC_L_END2_PIN) == LOW){
			LCount++;
		}else{
			LCount--;
		}
	}

	/* カウンタが分解能数に達した時=一回転 */
	if(LCount > MS_DC_ENC_PRT){
		LCount = LCount - MS_DC_ENC_PRT;
		LAngle++;
	}else if(LCount < -MS_DC_ENC_PRT){
		LCount = LCount + MS_DC_ENC_PRT;
		LAngle++;
	}
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msRDCInterrupt 											  */
/* 機能名		：DCモータの割込処理の初期設定									  */
/* 機能概要		：						  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2023/12/08	筈尾　辰也		新規作成					  */
/* -------------------------------------------------------------------------- */
void msRDCInterrupt(){
	/* 変化がL→Hの場合 */
	if(digitalRead(MS_DC_R_END1_PIN) == HIGH){
		if(digitalRead(MS_DC_R_END2_PIN) == LOW){
			RCount--;
		}else{
			RCount++;
		}
	}else{/* 変化がH→Lの場合 */
		if(digitalRead(MS_DC_R_END2_PIN) == LOW){
			RCount++;
		}else{
			RCount--;
		}
	}

	/* カウンタが分解能数に達した時=一回転 */
	if(RCount > MS_DC_ENC_PRT){
		RCount = RCount - MS_DC_ENC_PRT;
		RAngle++;
	}else if(RCount < -MS_DC_ENC_PRT){
		RCount = RCount + MS_DC_ENC_PRT;
		RAngle++;
	}
}

/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				  */
/* -------------------------------------------------------------------------- */
