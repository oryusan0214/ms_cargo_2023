#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\dc.cpp"
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
#include <stdint.h>								/* uint周り					 */
#include <Wire.h>
#include <PCA9685.h>                            /* PCA9685用ヘッダーファイル     */
#include <PID_v1.h>								/*  */
#include "time.h"								/* 時間に関するヘッダ		  */
#include "log.h"								/* ログに関わるヘッダ		  */
#include "dc.h"									/* DCに関わるヘッダ		  */

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

/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														  */
/* -------------------------------------------------------------------------- */
DC_MNG 	dc_Mng[DC_MAX];						/* DC管理データ				  */
SINT	LAngle;									/* 左DCの角度				 */
SINT	RAngle;									/* 右DCの角度				 */
SINT	LCount;									/* 左のDCエンコのカウント	 */ 
SINT	RCount;									/* 右のDCエンコのカウント	 */

double LSetpoint, LInput, LOutput;     			/* PIDの関数に用いる変数L	 */
double RSetpoint, RInput, ROutput;     			/* PIDの関数に用いる変数R	 */

volatile SINT	LSpeed = 0;            			/* 出力値			       */
volatile SINT	RSpeed = 0;            			/* 出力値			       */

/* Gain */
double LKp=4.00;
double LKi=0.00;
double LKd=0.20;
double RKp=4.00;
double RKi=0.00;
double RKd=0.20;

/* PIDの変数宣言 */
PID LmyPID(&LInput, &LOutput, &LSetpoint, LKp, LKi, LKd, DIRECT);
PID RmyPID(&RInput, &ROutput, &RSetpoint, RKp, RKi, RKd, DIRECT);

/* i2c address */
PCA9685 dcPwm		= PCA9685(0x43);    		/* DCのI2Cアドレス		 */

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
	/* プログラム起動時に一度だけ走る初期化処理 */
	/* 構造体情報の初期化 */
	SLNG dcCounter = 0;
	for (dcCounter = 0; dcCounter < ( sizeof(dc_Mng) / sizeof(dc_Mng[0])); dcCounter++) {
		msDCInitRecord(&dc_Mng[dcCounter],dcCounter);
	}

	/* 変数の初期化 */
	LAngle = DC_L_INIT;
	RAngle = DC_R_INIT;
	LCount = 0;
	RCount = 0;
	LInput = LAngle;
	RInput = RAngle;
	LSetpoint = 0;
	RSetpoint = 0;

	/* Pin Assign */
	pinMode(DC_L_PIN, OUTPUT);
	pinMode(DC_L_DIR_PIN, OUTPUT);
	pinMode(DC_R_PIN, OUTPUT);
	pinMode(DC_R_DIR_PIN, OUTPUT);	
	pinMode(DC_L_END1_PIN, INPUT);
	pinMode(DC_L_END2_PIN, INPUT);
	pinMode(DC_R_END1_PIN, INPUT);
	pinMode(DC_R_END2_PIN, INPUT);

	/* 割り込み設定 */
    attachInterrupt(digitalPinToInterrupt(DC_L_END1_PIN), msLDCInterrupt, CHANGE);
	attachInterrupt(digitalPinToInterrupt(DC_R_END1_PIN), msRDCInterrupt, CHANGE);

	/* PID Setup */
	LmyPID.SetMode(AUTOMATIC);
	LmyPID.SetSampleTime(DC_PERIOD_TIME);
	LmyPID.SetOutputLimits( (-1 * DC_SPEED), DC_SPEED);
	RmyPID.SetMode(AUTOMATIC);
	RmyPID.SetSampleTime(DC_PERIOD_TIME);
	RmyPID.SetOutputLimits( (-1 * DC_SPEED), DC_SPEED);

	/* pwm setup */
	dcPwm.begin();					            /* 初期設定 (アドレス0x40用) */
  	Wire.setClock(400000);            			/* Clock設定               */
	dcPwm.setPWMFreq(1000);			        	/* PWM周期を60Hzに設定 (アドレス0x40用) */
	dcPwm.setPWM(0, 0, 0);              		/* PWM設定                 */

	/* 進行方向の初期設定 */
	digitalWrite( DC_L_DIR_PIN, 1 ); 		/* 正転                    */
	digitalWrite( DC_R_DIR_PIN, 1 ); 		/* 正転                    */

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
	/* 汎用初期化処理 */
	if (mng == NULL) {
		msLog("これもう無理やで");
		return;
	}
	/* １レコード初期化 */
	mng->dcid = id;
	mng->timerid = 0;
	mng->busyflg = DC_READY;
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
	if ((busyflags == NULL) || (max != DC_MAX)) {
		msLog("引数エラー");
		return DC_PARAM;
	}

	/* ビジーデータコピーして返却 */
	for (dcIndex = 0; dcIndex < DC_MAX; dcIndex++) {
		busyflags[dcIndex] = dc_Mng[dcIndex].busyflg;
	}
	return DC_OK;
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
	SLNG dcRet = DC_OK;
	int  speed = 0;

	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if ((returns == NULL) || (angles == NULL) || (max != DC_MAX)) {
		msLog("引数エラー");
		return DC_PARAM;
	}

	/* DCの個数分ループして各種設定 */
	for (dcCounter == 0; dcCounter < DC_MAX; dcCounter++) {
		/* DCがビジー時は上位層の設定ミス */
		if (dc_Mng[dcCounter].busyflg == DC_BUSY) {
			returns[dcCounter] = DC_BUSY;
			continue;
		}
		/* ##要確認：DCの角度範囲がおかしい場合はパラメータエラー */
		if ((angles[dcCounter] < DC_ANG_MIN) || ((angles[dcCounter] > DC_ANG_MAX) && (angles[dcCounter] != DC_NOSET))) {
			returns[dcCounter] = DC_PARAM;
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
		dTmpAngle = dTmpAngle * DC_MOVETIME;

		/* タイマー計算＆コールバック設定 */
		dcRet = msSetTimer(dTmpAngle, &dc_Mng[dcCounter], msDCTimerCallback);
		if ((dcRet == MS_TIME_FULL) || (dcRet == MS_TIME_PARAM)) {
			msLog("タイマー関連エラー: %d", dcRet);
			return DC_NG;
		}
		/* タイマーIDを保管 */
		dc_Mng[dcCounter].timerid = dcRet;

		/* 指定角度を古くしておく */
		dc_Mng[dcCounter].oldangles = angles[dcCounter];

		/* ##サーボモーターのレジスタ設定 */
		/* idに合わせて変数設定 */
		switch(dcCounter){
		case DC_L :
			/* PID設定 */
			LInput 		= LAngle;
			LSetpoint 	= dc_Mng[dcCounter].oldangles;
			LmyPID.Compute();						/* PID演算 */
			speed = abs((int)LOutput);				/* 出力値格納 */
			speed = map(speed,0,DC_SPEED,0,4095);/* パルス幅の値に変換 */
			
			/* 方向指示 + PWM設定 */
			if(dcUD == true) {
				digitalWrite(DC_L_DIR_PIN,HIGH);
				dcPwm.setPWM(dcCounter, 0, speed);
			}else{
				digitalWrite(DC_L_DIR_PIN,LOW);
				dcPwm.setPWM(dcCounter, 0, speed);
			}
			/* 必要ならディレイ */
  			// delay(1);
			break;

		case DC_R :
			/* PID設定 */
			RInput 		= RAngle;
			RSetpoint 	= dc_Mng[dcCounter].oldangles;
			RmyPID.Compute();						/* PID演算 */
			speed = abs((int)ROutput);				/* 出力値格納 */
			speed = map(speed,0,DC_SPEED,0,4095);/* パルス幅の値に変換 */
			
			/* 方向指示 + PWM設定 */
			if(dcUD == true) {
				digitalWrite(DC_R_DIR_PIN,HIGH);
				dcPwm.setPWM(dcCounter, 0, speed);
			}else{
				digitalWrite(DC_R_DIR_PIN,LOW);
				dcPwm.setPWM(dcCounter, 0, speed);
			}
			/* 必要ならディレイ */
  			// delay(1);
			break;

		default:
			msLog("おかしい\n");
			break;
		}
	}
	return DC_OK;
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

	/* PWMのカット */
	if(ptr->dcid == DC_L){
		dcPwm.setPWM(DC_L_PIN, 0, 0);
	}else{
		dcPwm.setPWM(DC_R_PIN, 0, 0);
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
	if(digitalRead(DC_L_END1_PIN) == HIGH){
		if(digitalRead(DC_L_END2_PIN) == LOW){
			LCount--;
		}else{
			LCount++;
		}
	}else{/* 変化がH→Lの場合 */
		if(digitalRead(DC_L_END2_PIN) == LOW){
			LCount++;
		}else{
			LCount--;
		}
	}

	/* カウンタが分解能数に達した時=一回転 */
	if(LCount > DC_ENC_PRT){
		LCount = LCount - DC_ENC_PRT;
		LAngle++;
	}else if(LCount < -DC_ENC_PRT){
		LCount = LCount + DC_ENC_PRT;
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
	if(digitalRead(DC_R_END1_PIN) == HIGH){
		if(digitalRead(DC_R_END2_PIN) == LOW){
			RCount--;
		}else{
			RCount++;
		}
	}else{/* 変化がH→Lの場合 */
		if(digitalRead(DC_R_END2_PIN) == LOW){
			RCount++;
		}else{
			RCount--;
		}
	}

	/* カウンタが分解能数に達した時=一回転 */
	if(RCount > DC_ENC_PRT){
		RCount = RCount - DC_ENC_PRT;
		RAngle++;
	}else if(RCount < -DC_ENC_PRT){
		RCount = RCount + DC_ENC_PRT;
		RAngle++;
	}
}

/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				  */
/* -------------------------------------------------------------------------- */