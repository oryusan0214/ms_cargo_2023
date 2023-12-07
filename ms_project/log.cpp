#include "log.h"
/* -------------------------------------------------------------------------- */
/* msLog.cpp																                                  */
/* ログに関する処理を纏めています。											                      */
/* ログにはレベルが存在し、以下の規約で運営します。							              */
/* ・トレースログ：コールスタックを制御するログ								                */
/* ・エラーログ  ：引数エラー・システムエラー時のデバッグ補助用ログ			      */
/* ・デバッグログ：デバッグ時に情報出力するログ								                */
/* 																			                                      */
/* ログ情報はリングバッファで保存されます。									                  */
/* デバッグ・解析等でログ保存数を増やす場合は、msLofDefine.hで調整して下さい。*/
/* -------------------------------------------------------------------------- */
/* 	主状態  ：機能単位での状態制御											                      */
/* 	副状態  ：機能内部での状態遷移											                      */
/* 	個別状態：１状態内部での状態遷移										                      */
/* -------------------------------------------------------------------------- */
/* 番号		更新履歴								日付		氏名		                            */
/* -------------------------------------------------------------------------- */
/* 000000	新規作成								2013/05/08	桝井　隆治	                    */
/* 000001	Arduino版に改修					2023/12/04  山田　龍之介		 					      */
/* -------------------------------------------------------------------------- */
#define _CRT_SECURE_NO_WARNINGS

/* -------------------------------------------------------------------------- */
/* includeファイル															                              */
/* -------------------------------------------------------------------------- */
#include <stdio.h>								/* 基本的なＩ／Ｏ情報		  */
#include <string.h>								/* 初期化関連				      */
#include <stdarg.h>								/* システムログ				    */
#include "log.h"								  /* ログに関するヘッダ		  */
#include "time.h"								  /* 時間に関係するヘッダ		*/

/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														                              */
/* -------------------------------------------------------------------------- */

/* ##今回はリングバッファは使用しない事とする。理由はmsLogInitを参照の事。*/
/* ログ・リングバッファ ----------------------------------------------------- */

/*MS_LOGMNG g_LogRingBuf[MS_LOG_RINGBUF_MAX];*/	/* リングバッファログ		    */
/*USHT      g_LogRingBufCounter;					   */ /* ログ保存先				        */
SCHR      g_LogInitFlg = MS_LOG_INIT_FLG_OFF;	/* 初期化フラグ(未初期化)	  */


/* -------------------------------------------------------------------------- */
/* 関数名		：msLogInit													                              */
/* 機能名		：ログ情報初期化											                            */
/* 機能概要	：リングバッファの初期化を行います。						                  */
/*				  ：システム起動時には必ず本関数をコールすること。			            */
/*				  ：ピンポイントでログを取得する際、本関数をコールして下さい。      */
/* 引数			：void			 ：無し										                            */
/* 戻り値		：void			 ：無し										                            */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					                    */
/* 作成日		：2023/12/06	山田　龍之介	Arduino版に改修					              */
/* -------------------------------------------------------------------------- */
void msLogInit( void )
{
  /*## 今回はデバッグでシリアル通信を使うのでリングバッファを使用しないが、
       液晶等出力が遅い場合はリングバッファが必要です。 */
  /*## ここから------------------------------------------------*/

	/* リングバッファ初期化 */
	/*memset( &g_LogRingBuf[0], 0, sizeof( g_LogRingBuf ));*/
	
	/* 保存先初期化 */
	/*g_LogRingBufCounter = 0;*/

	/*## ここまで------------------------------------------------*/

	/* 初期化フラグＯＮ */
	g_LogInitFlg = MS_LOG_INIT_FLG_ON;
	
  /* シリアル通信の初期化 */
  Serial.begin(9600);
	return;
}


/* -------------------------------------------------------------------------- */
/* 関数名		：msLogIsInit												                              */
/* 機能名		：ログ情報初期化チェック									                        */
/* 機能概要	：リングバッファの初期化が完了しているかチェックします。	        */
/* 引数			：void			 ：無し										                            */
/* 戻り値		：SCHR			 ：MS_LOG_INIT_FLG_ON	：初期化済み		                */
/*				  ：				   ：MS_LOG_INIT_FLG_OFF：未初期化			                */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					                    */
/* -------------------------------------------------------------------------- */
SCHR msLogIsInit( void )
{
	return g_LogInitFlg;
}


/* -------------------------------------------------------------------------- */
/* 関数名		：msLog														                                */
/* 機能名		：ログ情報取得												                            */
/* 機能概要	：ログ情報を取得します。									                        */
/* 引数			：char*			 ：Message		：[I N]ログ文章				                  */
/* 				  ：...			   ：可変長引数	：[I N]可変長情報			                  */
/* 戻り値		：void			 ：無し										                            */
/* 作成日		：2013/03/12	桝井　隆治		新規作成					                    */
/* 作成日		：2023/12/06	山田　龍之介	Arduino版に改修					              */
/* -------------------------------------------------------------------------- */
void msLog( const char* Message, ... )
{
	ULNG    ulSystemTime = 0;					/* 起動時間					              */
	SCHR    scStr[256];							  /* 可変長文字列格納用		          */
  SCHR    scStrCopy[256];           /* Copy用の可変長文字列格納用		  */
	va_list args;								      /* 可変長引数処理用			          */
	
	/* リングバッファ初期化チェック ----------------------------------------- */
	if( msLogIsInit( ) != MS_LOG_INIT_FLG_ON ) {
		Serial.println(" System not init ");
		return;
	}
	
	/* システム時間取得 ----------------------------------------------------- */
	ulSystemTime = msTimeGetSystemTime( );
	
	/* 可変長引数変換取得 --------------------------------------------------- */
	memset( scStr, 0, sizeof( scStr ));
	va_start( args, Message );					  /*  可変長引数処理開始		*/
	vsprintf( scStr, Message, args );			/*  可変長引数取得			  */
	va_end( args );								        /*  可変長引数処理終了 		*/
	
	
	/* T.B.D. 時間計測のシステムを入れる事 ---------------------------------- */
	/* リングバッファへ情報を格納すること */
  /* Serial.printlnのフォーマットに合わせてメモリにコピー */
  sprintf(scStrCopy, "[%010d] %s\n", ulSystemTime, scStr);
  /* 出力 */
	Serial.println(scStrCopy);

	return;
}


/* -------------------------------------------------------------------------- */
/* 				       Copyright HAL College of Technology & Design				          */
/* -------------------------------------------------------------------------- */
