/* -------------------------------------------------------------------------- */
/* dc.h																  */
/* ＣＬ２３・ＳＤ３２・ＲＤ４Ｃ												  */
/* DC制御に係るヘッダ															  */
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
#define MS_DC_MAX           (    4 )			/* サーボ個数				  */
#define MS_DC_OK            (    0 )			/* 成功						  */
#define MS_DC_NG            (   -1 )			/* 失敗						  */
#define MS_DC_PARAM         (   -2 )			/* 引数エラー				  */
#define MS_DC_READY         (    0 )			/* サーボ待機				  */
#define MS_DC_BUSY          (   -3 )			/* サーボビジー				  */
#define MS_DC_NOSET       ( 0xEFFF )			/* サーボ設定無し			  */
#define MS_DC_SPEED              255            /* DCモータの出力値              */
#define MS_DC_LIMIT_TIME        1000            /* ハンド開閉時間 */

#define MS_DC_ANG_MIN             0             /* DCモータの最大角          */
#define MS_DC_ANG_MAX           270             /* DCモータの最小角          */

#define MS_DC_L                    0            /* 左肩のDC                  */
#define MS_DC_R                    1            /* 右肩のDC                  */
#define MS_DC_ROD                  2            /* 昇降機のDC                */
#define MS_DC_HAND                 3            /* ハンドのDC                */

#define MS_DC_L_PIN                0            /* 左肩のDCのピン             */
#define MS_DC_R_PIN                1            /* 右肩のDCのピン              */
#define MS_DC_ROD_PIN              2            /* 昇降機のDCのピン            */
#define MS_DC_HAND_PIN             3            /* ハンドのDCのピン            */

/* 不明 */
#define DCMIN                   150             /* 最小パルス幅              */
#define DCMAX                   600             /* 最大パルス幅              */

/* ##1度の移動時間は目検で調べるしかない */
#define MS_DC_MOVETIME     (   10 )			/* サーボ１度移動に必要な時間(ms) */

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言															  */
/* -------------------------------------------------------------------------- */
void msDCInit(void);
SLNG msDCGetBusy(UCHR* busyflags, USHT max);
SLNG msDCSet(SLNG* returns, SSHT* angles, USHT max);
void msDCTimerCallback(void* id);

/* -------------------------------------------------------------------------- */
/* 構造体定義																  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				  */
/* -------------------------------------------------------------------------- */
