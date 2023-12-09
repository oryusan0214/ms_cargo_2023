/* -------------------------------------------------------------------------- */
/* hand.h																  */
/* ＣＬ２３・ＳＤ３２・ＲＤ４Ｃ												  */
/* HAND制御に係るヘッダ															  */
/* -------------------------------------------------------------------------- */
/* 番号		更新履歴								日付		氏名		  */
/* -------------------------------------------------------------------------- */
/* 000000	新規作成								2013/05/08	桝井　隆治	  */
/* 000001   機能追加                                2023/12/06  筈尾　辰也       */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* includeファイル															  */
/* -------------------------------------------------------------------------- */
#include "common.h"							    /* システム共通ヘッダ		  */

/* -------------------------------------------------------------------------- */
/* define宣言																  */
/* -------------------------------------------------------------------------- */
#define MS_HAND_OK            (    0 )			/* 成功						  */
#define MS_HAND_NG            (   -1 )			/* 失敗						  */
#define MS_HAND_PARAM         (   -2 )			/* 引数エラー				  */
#define MS_HAND_READY         (    0 )			/* HAND待機				  */
#define MS_HAND_BUSY          (   -3 )			/* HANDビジー				  */
#define MS_HAND_NOSET       ( 0xEFFF )			/* HAND設定無し			  */
#define MS_HAND_SPEED              255            /* HANDモータの出力値              */

#define MS_HAND_DST_MIN             0             /* HANDモータの最大距離          */
#define MS_HAND_DST_MAX           270             /* HANDモータの最小角距離         */

#define MS_HAND                      2            /* HANDのID                   */

#define MS_HAND_PIN                  2            /* HANDのピン                */

/* 不明 */
#define HANDMIN                   150             /* 最小パルス幅              */
#define HANDMAX                   600             /* 最大パルス幅              */

/* ##1度の移動時間は目検で調べるしかない */
#define MS_HAND_MOVETIME      (  1000)			/* HANDの移動に必要な時間(ms)  */

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言															  */
/* -------------------------------------------------------------------------- */
void msHANDInit(void);
SLNG msHANDGetBusy(UCHR* busyflags);
SLNG msHANDSet(SLNG* returns, SSHT* angles);
void msHANDTimerCallback(void* id);

/* -------------------------------------------------------------------------- */
/* 構造体定義																  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				  */
/* -------------------------------------------------------------------------- */