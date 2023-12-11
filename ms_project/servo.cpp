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
#include "time.h"								/* 時間に関するヘッダ		  */
#include "log.h"								/* ログに関わるヘッダ		  */
#include "servo.h"								/* サーボに関わるヘッダ		  */
#include <Wire.h>
#include <PCA9685.h>                            /* PCA9685用ヘッダーファイル     */

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
SERVO_MNG g_Mng[MS_SERVO_MAX];					/* サーボ管理データ			  */
PCA9685 leftPwm		= PCA9685(0x41);    		/* 左サーボのI2Cアドレス		 */
PCA9685 rightPwm 	= PCA9685(0x42);   			/* 左サーボのI2Cアドレス		 */

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
	SLNG slCounter = 0;
	for (slCounter = 0; slCounter < ( sizeof(g_Mng) / sizeof(g_Mng[0])); slCounter++) {
		msServoInitRecord(&g_Mng[slCounter],slCounter);
	}
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
	if (mng == NULL) {
		msLog("これもう無理やで");
		return;
	}
	/* １レコード初期化 */
	mng->servoid = id;
	mng->timerid = 0;
	mng->busyflg = MS_SERVO_READY;
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
	if ((busyflags == NULL) || (max != MS_SERVO_MAX)) {
		msLog("引数エラー");
		return MS_SERVO_PARAM;
	}

	/* ビジーデータコピーして返却 */
	for (slIndex = 0; slIndex < MS_SERVO_MAX; slIndex++) {
		busyflags[slIndex] = g_Mng[slIndex].busyflg;
	}
	return MS_SERVO_OK;
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
/*				：SSHT*		: angles	 ：[I N] 各サーボの設定角度			  */
/*				：			:			 ：何もしない場合はMS_SERVO_NOSET	  */
/*				：USHT		: max		 ：[I N] 第一・第二引数の配列数(18個) */
/* 戻り値		：SLNG		: MS_SERVO_OK	：正常終了						  */
/*				：			: MS_SERVO_PARAM：引数異常						  */
/* ■■注意■■ ：正常終了を返しても、第一引数のreturnsはエラーの場合がある為 */
/*				：必ずチェックする事！										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
SLNG msServoSet(SLNG* returns, SSHT* angles, USHT max)
{
	SLNG slCounter	= 0;
	SLNG slRet		= MS_SERVO_OK;
	uint8_t slAng	= 0;

	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if ((returns == NULL) || (angles == NULL) || (max != MS_SERVO_MAX)) {
		msLog("引数エラー");
		return MS_SERVO_PARAM;
	}

	/* サーボの個数分ループして各種設定 */
	for (slCounter == 0; slCounter < MS_SERVO_MAX; slCounter++) {
		/* サーボがビジー時は上位層の設定ミス */
		if (g_Mng[slCounter].busyflg == MS_SERVO_BUSY) {
			returns[slCounter] = MS_SERVO_BUSY;
			continue;
		}
		/* ##要確認：サーボの角度範囲がおかしい場合はパラメータエラー */
		if ((angles[slCounter] < MS_SERVO_DST_MIN) || ((angles[slCounter] > MS_SERVO_DST_MAX) && (angles[slCounter] != MS_SERVO_NOSET))) {
			returns[slCounter] = MS_SERVO_PARAM;
			continue;
		}

		/* サーボモーター設定可能と判断 --------------------------------------*/
		/* 指定角度からサーボ移動に必要な時間を算出 */
		SSHT sTmpAngle = 0;
		sTmpAngle = g_Mng[slCounter].oldangles - angles[slCounter];
		/* マイナス角度をプラスに補正 */
		if (sTmpAngle < 0) {
			sTmpAngle = sTmpAngle * -1;
		}
		/* ##define値を確認 移動予定角度から時間へ変換 */
		sTmpAngle = sTmpAngle * MS_SERVO_MOVETIME;

		/* タイマー計算＆コールバック設定 */
		slRet = msSetTimer(sTmpAngle, &g_Mng[slCounter], msServoTimerCallback);
		if ((slRet == MS_TIME_FULL) || (slRet == MS_TIME_PARAM)) {
			msLog("タイマー関連エラー: %d", slRet);
			return MS_SERVO_NG;
		}
		/* タイマーIDを保管 */
		g_Mng[slCounter].timerid = slRet;

		/* 指定角度を古くしておく */
		g_Mng[slCounter].oldangles = angles[slCounter];

		/* ##サーボモーターのレジスタ設定 */

		/* 角度（0～270）をPWMのパルス幅（150～600）に変換 パルス幅要変更 */
		slAng = map(g_Mng[slCounter].oldangles, MS_SERVO_DST_MIN, MS_SERVO_DST_MAX, SERVOMIN, SERVOMAX);

		if(slCounter < (MS_SERVO_MAX / 2) ){
  			leftPwm.setPWM(slCounter, 0, slAng);
		}else{
  			rightPwm.setPWM(slCounter - (MS_SERVO_MAX / 2), 0, slAng);
		}
		/* 必要ならディレイ */
  		// delay(1);
	}
	return MS_SERVO_OK;
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

/* -------------------------------------------------------------------------- */
/* 関数名		：msServoInterrupt											  */
/* 機能名		：##Aruduinoから来る割り込み通知							  */
/* 機能概要		：デューティー比の変更を行う								  */
/* 引数			：void			 ：無し										  */
/* 戻り値		：void			 ：無し										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
//void msServoInterrupt(void)
//{
	/* 関数名は仮なので事由にどうぞ */
	/* ここで必要ならデューティ比を変更する */
	/* モータードライバが全部自動てやってくれたら最高なんだけどな・・ */
//}



/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				  */
/* -------------------------------------------------------------------------- */
