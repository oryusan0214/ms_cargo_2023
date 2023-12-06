/* -------------------------------------------------------------------------- */
/* servo.h																  */
/* ＣＬ２３・ＳＤ３２・ＲＤ４Ｃ												  */
/* サーボ制御に係るヘッダ															  */
/* -------------------------------------------------------------------------- */
/* 番号		更新履歴								日付		氏名		  */
/* -------------------------------------------------------------------------- */
/* 000000	新規作成								2013/05/08	桝井　隆治	  */
/* 000001   機能追加                                2023/12/06  筈尾　辰也       */
/* -------------------------------------------------------------------------- */




/* -------------------------------------------------------------------------- */
/* includeファイル															  */
/* -------------------------------------------------------------------------- */
#include "system.h"							    /* システム共通ヘッダ		  */

/* -------------------------------------------------------------------------- */
/* define宣言																  */
/* -------------------------------------------------------------------------- */
#define MS_SERVO_MAX          (   18 )			/* サーボ個数				  */
#define MS_SERVO_OK           (    0 )			/* 成功						  */
#define MS_SERVO_NG           (   -1 )			/* 失敗						  */
#define MS_SERVO_PARAM        (   -2 )			/* 引数エラー				  */
#define MS_SERVO_READY        (    0 )			/* サーボ待機				  */
#define MS_SERVO_BUSY         (   -3 )			/* サーボビジー				  */
#define MS_SERVO_NOSET      ( 0xEFFF )			/* サーボ設定無し			  */
#define SERVOMIN 150                            /* 最小パルス幅              */
#define SERVOMAX 600                            /* 最大パルス幅              */

/* ##1度の移動時間は目検で調べるしかない */
#define MS_SERVO_MOVETIME     (   10 )			/* サーボ１度移動に必要な時間(ms) */

/* -------------------------------------------------------------------------- */
/* プロトタイプ宣言															  */
/* -------------------------------------------------------------------------- */
void msServoInit(void);
SLNG msServoGetBusy(UCHR* busyflags, USHT max);
SLNG msServoSet(SLNG* returns, SSHT* angles, USHT max);
void msServoTimerCallback(void* id);

/* -------------------------------------------------------------------------- */
/* 構造体定義																  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 				Copyright HAL College of Technology & Design				  */
/* -------------------------------------------------------------------------- */
