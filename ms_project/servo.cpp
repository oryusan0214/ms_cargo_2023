/* -------------------------------------------------------------------------- */
/* servo.cpp																  */
/* サーボモーター制御に関わる処理											  */
/* 出力系はサーボと同じ処理方法で対応すると良い								  */
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
#include <Wire.h>
#include <PCA9685.h>                            /* PCA9685用ヘッダーファイル     */
#include "time.h"								/* 時間に関するヘッダ		  */
#include "log.h"								/* ログに関わるヘッダ		  */
#include "servo.h"								/* サーボに関わるヘッダ		  */

/* -------------------------------------------------------------------------- */
/* 構造体定義（ローカル）													  */
/* -------------------------------------------------------------------------- */
typedef struct {
	SLNG servoid;								/* サーボモータ番号			  */
	SLNG timerid;								/* タイマーID				  */
	UCHR busyflg;								/* ビジーフラグ(サーボ個数分) */
	SSHT oldangles;								/* 前回設定角度たち			  */
} SERVO_MNG;

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言(ローカル)												  */
/* -------------------------------------------------------------------------- */
void msServoInitRecord(SERVO_MNG* mng,SLNG id);


/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														  */
/* -------------------------------------------------------------------------- */
SERVO_MNG g_Mng[SERVO_MAX];						/* サーボ管理データ			  */
PCA9685 leftPwm		= PCA9685(0x41);    		/* 左サーボのI2Cアドレス		 */
PCA9685 rightPwm 	= PCA9685(0x42);   			/* 左サーボのI2Cアドレス		 */
uint16_t ang_min[SERVO_MAX] = {115,40,65,125,0,80,125,40,44,120,10,50,125,5,75,115,10,60};
uint16_t ang_max[SERVO_MAX] = {180,82,140,180,37,110,180,82,120,180,50,120,180,35,105,180,43,132};
uint16_t ang_ini[SERVO_MAX] = {140,70,140,140,35,95,145,70,44,140,35,50,140,35,90,140,35,132};

/* -------------------------------------------------------------------------- */
/* 関数名		：msServoInit												  */
/* 機能名		：サーボモータの初期化処理									  */
/* 機能概要		：グローバル変数等の初期化を行います						  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msServoInit(void)
{
	/* プログラム起動時に一度だけ走る初期化処理 */
	SLNG slCounter = 0;
	for (slCounter = 0; slCounter < ( sizeof(g_Mng) / sizeof(g_Mng[0])); slCounter++) {
		msServoInitRecord(&g_Mng[slCounter],slCounter);
		g_Mng[slCounter].oldangles = ang_ini[slCounter];
	}

	/* PWMの設定 */
	leftPwm.begin();
  	rightPwm.begin();

	pinMode(SDA,INPUT);
	pinMode(SCL,INPUT);
	Wire.setClock(100000);            			/* Clock設定               */

		leftPwm.begin();
  	rightPwm.begin();
		pinMode(SDA,INPUT);
    pinMode(SCL,INPUT);
    Wire.setClock(100000);                      /* Clock設定               */
  	leftPwm.setPWMFreq(100);
  	rightPwm.setPWMFreq(100);
	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msServoInitRecord											  */
/* 機能名		：サーボモータの初期化処理(１レコード)						  */
/* 機能概要		：															  */
/* 引数			：void		: 無し											  */
/* 戻り値		：void		: 無し											  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msServoInitRecord(SERVO_MNG* mng,SLNG id)
{
	/* 汎用初期化処理 */
	if (mng == NULL || id < 0 || SERVO_MAX <= id) {
		msLog("これもう無理やで");
		return;
	}
	/* １レコード初期化 */
	mng->servoid = id;
	mng->timerid = 0;
	mng->busyflg = SERVO_READY;
	mng->oldangles = 0;
	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msServoGetBusy											  */
/* 機能名		：サーボビジー状態取得										  */
/* 機能概要		：現在のサーボモーターが動作中か否かを返却します。			  */
/*				：本開発でのサーボは18個固定なので、引数データも18個用意する事*/
/* 引数			：UCHR*		: busyflags	 ：[OUT] フラグ配列(18個のみ動作)	  */
/*				：USHT		: max		 ：[I N] 第一引数の配列数			  */
/* 戻り値		：SLNG		: MS_SERVO_OK	：正常終了						  */
/*				：			: MS_SERVO_PARAM：引数異常						  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
SLNG msServoGetBusy(UCHR* busyflags, USHT max)
{
	SLNG slIndex = 0;

	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if ((busyflags == NULL) || (max != SERVO_MAX)) {
		msLog("引数エラー６");
		return SERVO_PARAM;
	}

	/* ビジーデータコピーして返却 */
	for (slIndex = 0; slIndex < SERVO_MAX; slIndex++) {
		//Serial.println("busy base");
    	//Serial.println(g_Mng[slIndex].busyflg);
		busyflags[slIndex] = g_Mng[slIndex].busyflg;
	}
	return SERVO_OK;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msServoGetSet												  */
/* 機能名		：サーボ角度設定											  */
/* 機能概要		：サーボモーターの角度設定を行います。						  */
/*	##			：１度角度を移動するのに〇〇秒要すると定義し、現在の角度に対し*/
/*				：相対移動量×〇〇秒でタイマーをセットし、管理する。		  */
/*				：busyフラグが立っている所は設定せず、それ以外のみ設定する。  */
/*				：事前に必ず仕様を良く理解して使う事。						  */
/* 引数			：SLNG*		: returns	 ：[OUT] 各サーボの設定が完了可否	  */
/*				：			:			 ： MS_SERVO_OK    :設定成功		  */
/*				：			:			 ： MS_SERVO_BUSY  :ビジーの為失敗    */
/*				：			:			 ： MS_SERVO_PARAM :角度がおかしい    */
/*				：uint16_t*	: angles	 ：[I N] 各サーボの設定角度			  */
/*				：			:			 ：何もしない場合はMS_SERVO_NOSET	  */
/*				：USHT		: max		 ：[I N] 第一・第二引数の配列数(18個) */
/* 戻り値		：SLNG		: MS_SERVO_OK	：正常終了						  */
/*				：			: MS_SERVO_PARAM：引数異常						  */
/* ■■注意■■ ：正常終了を返しても、第一引数のreturnsはエラーの場合がある為 */
/*				：必ずチェックする事！										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* 更新日		：2023/12/13	山田　龍之介	anglesを仕様に合わせてuint16_tに変更					  */
/* -------------------------------------------------------------------------- */
SLNG msServoSet(SLNG* returns, uint16_t* angles, USHT max)
{
	SLNG slCounter	= 0;
	SLNG slRet		= SERVO_OK;
	SINT slAng	= 0;
	//Serial.println("--- (1) ---\n");
	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if ((returns == NULL) || (angles == NULL) || (max != SERVO_MAX)) {
		msLog("引数エラー７");
		return SERVO_PARAM;
	}
	//Serial.println("--- (2) ---\n");
	/* サーボの個数分ループして各種設定 */
	for (slCounter == 0; slCounter < SERVO_MAX; slCounter++) {
		/* サーボがビジー時は上位層の設定ミス */
		if (g_Mng[slCounter].busyflg == SERVO_BUSY) {
			Serial.println("--- (8) ---\n");
			returns[slCounter] = SERVO_BUSY;
			continue;
		}
		/* ##要確認：サーボの角度範囲がおかしい場合はパラメータエラー */
		if ((angles[slCounter] < ang_min[slCounter]) || ((angles[slCounter] > ang_max[slCounter]) && (angles[slCounter] != SERVO_NOSET))) {
			Serial.println("--- (9) ---\n");
			Serial.println(slCounter);
			Serial.println(angles[slCounter]);
			returns[slCounter] = SERVO_PARAM;
			continue;
		}
		//Serial.println("--- (3) ---\n");
		/* サーボモーター設定可能と判断 --------------------------------------*/
		/* 指定角度からサーボ移動に必要な時間を算出 */
		SSHT sTmpAngle = 0;
		sTmpAngle = g_Mng[slCounter].oldangles - angles[slCounter];
		/* マイナス角度をプラスに補正 */
		if (sTmpAngle < 0) {
			sTmpAngle = sTmpAngle * -1;
		}
		/* ##define値を確認 移動予定角度から時間へ変換 */
		sTmpAngle = sTmpAngle * SERVO_MOVETIME;
		//Serial.println("--- (4) ---\n");
		if(sTmpAngle == 0){
			sTmpAngle++;
		}
	
		/* タイマー計算＆コールバック設定 */
		slRet = msSetTimer(sTmpAngle, &g_Mng[slCounter], msServoTimerCallback);
		if ((slRet == MS_TIME_FULL) || (slRet == MS_TIME_PARAM)) {
			msLog("タイマー関連エラー: %d", slRet);
			return SERVO_NG;
		}
		g_Mng[slCounter].busyflg = SERVO_BUSY;
		/* タイマーIDを保管 */
		g_Mng[slCounter].timerid = slRet;

		/* 指定角度を古くしておく */
		g_Mng[slCounter].oldangles = angles[slCounter];
		slAng = g_Mng[slCounter].oldangles;

		/* ##サーボモーターのレジスタ設定 */
		//Serial.println("--- (5) ---\n");
		/* 角度（0～270）をPWMの1パルス幅内でのHIGHの時間(ms)に変換 */
		/* 1,7は特殊サーボの為角度270に設定する */
    	if(slCounter == 1 || slCounter == 7){
      		slAng = map(g_Mng[slCounter].oldangles, 0, 270, 0, 2000);
    	}else{
		  	slAng = map(g_Mng[slCounter].oldangles, 0, 180, 0, 2000);
    	}
		//Serial.println("--- (6) ---\n");
		/* 500ms - 2500msが範囲の為 +500 */
    	slAng = slAng + 500;
    	/* 10進数を12進数に変換（？） */
    	slAng = map(slAng, 0, 10000, 0, 4095);
		Serial.println("koko kiteruyo");
		/* idに対応したドライバにpwmセット */
		if(slCounter < (SERVO_MAX / 2) ){
  			leftPwm.setPWM(slCounter, 0, slAng);
		}else{
			rightPwm.setPWM(slCounter - (SERVO_MAX / 2), 0, slAng);
		}
		//Serial.println("--- (7) ---\n");
		/* 必要ならディレイ */
  		// delay(1);
	}
	return SERVO_OK;
}
/* -------------------------------------------------------------------------- */
/* 関数名		：msServoTimerCallback										  */
/* 機能名		：サーボモータが指定角度まで動いた（はず）					  */
/* 機能概要		：モーターのbusy状態を解除します。							  */
/* 				：優先度を付ける必要がある場合は、ここでソート処理を行うこと。*/
/* 引数			：void			 ：無し										  */
/* 戻り値		：void			 ：無し										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msServoTimerCallback(void* addr)
{
	SERVO_MNG* ptr = (SERVO_MNG*)addr;
	SSHT tmpAngle = 0;
	/* 該当のタイマーカット */
	msTimeKill(ptr->timerid);

	/* 角度情報を逃がす */
	tmpAngle = ptr->oldangles;

	/* 設定情報クリア(Ready状態に戻す) */
	msServoInitRecord(ptr,ptr->servoid);

	/* 角度情報を戻す */
	ptr->oldangles = tmpAngle;

	return;
}