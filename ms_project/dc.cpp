/* -------------------------------------------------------------------------- */
/* dc.cpp																  */
/* DCモーター制御に関わる処理												  */
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
#include "dc.h"								/* サーボに関わるヘッダ		  */

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言(ローカル)												  */
/* -------------------------------------------------------------------------- */
void msDCInitRecord(DC_MNG* mng);

/* -------------------------------------------------------------------------- */
/* 構造体定義（ローカル）													  */
/* -------------------------------------------------------------------------- */
typedef struct {
	SLNG dcid;									/* サーボモータ番号			  */
	SLNG timerid;								/* タイマーID				  */
	UCHR busyflg;								/* ビジーフラグ(サーボ個数分) */
	SSHT oldangles;								/* 前回設定角度たち			  */
} DC_MNG;

/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														  */
/* -------------------------------------------------------------------------- */
DC_MNG g_Mng[MS_DC_MAX];						/* DC管理データ				  */

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
	for (dcCounter = 0; dcCounter < ( sizeof(g_Mng) / sizeof(g_Mng[0])); dcCounter++) {
		msDCInitRecord(&g_Mng[dcCounter]);
	}
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
void msDCInitRecord(DC_MNG* mng)
{
	if (mng == NULL) {
		msLog("これもう無理やで");
		return;
	}
	/* １レコード初期化 */
	mng->dcid = 0;
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
		busyflags[dcIndex] = g_Mng[dcIndex].busyflg;
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
/*				：SSHT*		: angles	 ：[I N] 各DCの設定角度			  */
/*				：			:			 ：何もしない場合はMS_DC_NOSET	  */
/*				：USHT		: max		 ：[I N] 第一・第二引数の配列数(18個) */
/* 戻り値		：SLNG		: MS_DC_OK	：正常終了						  */
/*				：			: MS_DC_PARAM：引数異常						  */
/* ■■注意■■ ：正常終了を返しても、第一引数のreturnsはエラーの場合がある為 */
/*				：必ずチェックする事！										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
SLNG msDCSet(SLNG* returns, SSHT* angles, USHT max)
{
	SLNG dcCounter = 0;
	SLNG dcRet = MS_DC_OK;
	UINT_8t dcAng = 0;

	/* 引数チェック(OnjectはNULLを許可する)---------------------------------- */
	if ((returns == NULL) || (angles == NULL) || (max != MS_DC_MAX)) {
		msLog("引数エラー");
		return MS_DC_PARAM;
	}

	/* DCの個数分ループして各種設定 */
	for (dcCounter == 0; dcCounter < MS_DC_MAX; dcCounter++) {
		/* DCがビジー時は上位層の設定ミス */
		if (g_Mng[dcCounter].busyflg == MS_DC_BUSY) {
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
		dTmpAngle = g_Mng[dcCounter].oldangles - angles[dcCounter];
		/* マイナス角度をプラスに補正 */
		if (dTmpAngle < 0) {
			dTmpAngle = dTmpAngle * -1;
			dcUD = true;						/* 正転に変更					 */
		}
		/* ##define値を確認 移動予定角度から時間へ変換 */
		dTmpAngle = dTmpAngle * MS_DC_MOVETIME;

		/* タイマー計算＆コールバック設定 */
		dcRet = msSetTimer(dTmpAngle, &g_Mng[dcCounter], msDCTimerCallback);
		if ((dcRet == MS_TIME_FULL) || (dcRet == MS_TIME_PARAM)) {
			msLog("タイマー関連エラー: %d", dcRet);
			return MS_DC_NG;
		}
		/* タイマーIDを保管 */
		g_Mng[dcCounter].timerid = dcRet;

		/* 指定角度を古くしておく */
		g_Mng[dcCounter].oldangles = angles[dcCounter];

		/* ##サーボモーターのレジスタ設定 */
		switch(dcCounter){
		case MS_DC_L :
			/* 現在角度の取得 */
			//UINT dcNowAng = getAngle

			/* 目標角度の比較 到達してたらifに入る */
			if(dcUD == false? (dcNowAng < g_Mng[dcCounter].oldangles) : (dcNowAng > g_Mng[dcCounter].oldangles)){
				/* PWMを止める */
				analogWrite(MS_DC_L_PIN, 0);
			}else if(dcUD == true) {
				analogWrite(MS_DC_L_PIN, MS_DC_SPEED);
			}else{
				analogWrite(MS_DC_L_PIN, -MS_DC_SPEED);
			}
			/* 必要ならディレイ */
  			// delay(1);
			break;

		case MS_DC_R :
			/* 現在角度の取得 */
			//UINT dcNowAng = getAngle

			/* 目標角度の比較 到達してたらifに入る */
			if(dcUD == false? (dcNowAng < g_Mng[dcCounter].oldangles) : (dcNowAng > g_Mng[dcCounter].oldangles)){
				/* PWMを止める */
				analogWrite(MS_DC_R_PIN, 0);
			}else if(dcUD == true) {
				analogWrite(MS_DC_R_PIN, MS_DC_SPEED);
			}else{
				analogWrite(MS_DC_R_PIN, -MS_DC_SPEED);
			}
			/* 必要ならディレイ */
  			// delay(1);
			break:

		case MS_DC_ROD :
			/* 現在距離の取得 */
			//UINT dcNowDis = getDistance

			/* 目標距離の比較 到達してたらifに入る */
			if(dcNowDis == g_Mng[dcCounter].oldangles){
				/* PWMを止める */
				analogWrite(MS_DC_ROD_PIN, 0);
			}else if(dcUD == true) {
				analogWrite(MS_DC_ROD_PIN, MS_DC_SPEED);
			}else{
				analogWrite(MS_DC_ROD_PIN, -MS_DC_SPEED);
			}
			/* 必要ならディレイ */
  			// delay(1);
			break;

		case MS_DC_HAND :
			/* 現在時間の取得 */
			//UINT dcNowTime = getTime

			/* 目標時間の比較 到達してたらifに入る */
			if(dcNowTime > MS_DC_LIMIT_TIME){
				/* PWMを止める */
				analogWrite(MS_DC_HAND_PIN, 0);
			}else if(dcUD == true) {
				analogWrite(MS_DC_HAND_PIN, MS_DC_SPEED);
			}else{
				analogWrite(MS_DC_HAND_PIN, -MS_DC_SPEED);
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

	/* 角度情報を逃がす */
	tmpAngle = ptr->oldangles;

	/* 設定情報クリア(Ready状態に戻す) */
	msDCInitRecord(ptr);

	/* 角度情報を戻す */
	ptr->oldangles = tmpAngle;

	return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：msDCInterrupt_hl											  */
/* 機能名		：##Aruduinoから来る割り込み通知 対応ピンがハイからロウに		  */
/* 機能概要		：エンコーダのピンを読みその値を基に角度をインクリメントする	  */
/* 引数			：void			 ：無し										  */
/* 戻り値		：void			 ：無し										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msDCInterrupt_hl(void)
{
	bool msEncordRet = analogRead(pin);

	if(msEncordRet == true){
		//setAng(++);
	}
}
/** -------------------------------------------------------------------------- */
/* 関数名		：msDCInterrupt_lh											  */
/* 機能名		：##Aruduinoから来る割り込み通知 対応ピンがロウからハイに		  */
/* 機能概要		：エンコーダのピンを読みその値を基に角度をでクリメントする	  */
/* 引数			：void			 ：無し										  */
/* 戻り値		：void			 ：無し										  */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					  */
/* -------------------------------------------------------------------------- */
void msDCInterrupt(void)
{
	bool msEncordRet = analogRead(pin);

	if(msEncordRet == false){
		//setAng(--);
	}
}



/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				  */
/* -------------------------------------------------------------------------- */
