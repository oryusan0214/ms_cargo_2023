#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\scene.cpp"
/* -------------------------------------------------------------------------- */
/* scene.cpp																                                  */
/* デモで動かすためのsceneをコントロールするソースコードです。								*/
/* sceneを関数で作成しています。これはメモリの削減をするためです。					  */
/* ・straightSceneInput：6足でまっすぐに歩くシーンを管理しています。				  */
/* ・lTurnSceneInput   ：6足で旋回をするシーンを管理しています。			        */
/* ・rTurnSceneInput   ：6足で旋回をするシーンを管理しています。			        */
/* ・InitSceneInput    ：6足の初期化をするシーンを管理しています。			      */
/* ・ArmSceneInput     ：アームを上げるシーンを管理しています。				        */
/* ・ArmDownSceneInput ：アームを下げるシーンを管理しています。				        */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* 番号		更新履歴								日付		氏名		                            */
/* -------------------------------------------------------------------------- */
/* 000001	新規作成					2023/12/15  山田　龍之介		 					            */
/* 000002	最終更新					2024/01/17  山田　龍之介		 					            */
/* -------------------------------------------------------------------------- */
#include "scene.h"
#include "servo.h"
#include "dc.h"
#include "hand.h"
#include "rod.h"
#include "log.h"
#include <string.h>
#include "time.h"


/* -------------------------------------------------------------------------- */
/* 関数名		：sceneInput													                            */
/* 機能概要	：sceneで設定したシナリオに合わせて実行するsceneを返す処理				*/
/* 引数			：uin8_t*		 ：checker	今のシーン・シナリオの状態を格納				  */
/* 戻り値		：Scene			 ：nowscene 今のシーンを返す										      */
/* 作成日		：2023/12/15	山田　龍之介	新規作成      					              */
/* -------------------------------------------------------------------------- */
Scene sceneInuput(uint8_t *checker)
{
  static uint16_t scenecounter = 0;
  static uint16_t scenariocounter = 0;
  uint8_t scenechecker = SCENE_OK;
  
  
  //シナリオの設定
  uint8_t scene[] = {
                      SCENE_INIT,
                       
                      SCENE_STRAIGHT,SCENE_STRAIGHT,SCENE_STRAIGHT,
                      SCENE_STRAIGHT,SCENE_STRAIGHT,
                      SCENE_ARM,
                      
                      SCENE_RTURN,SCENE_RTURN,SCENE_RTURN,
                      SCENE_RTURN,SCENE_RTURN,SCENE_RTURN,
                      SCENE_RTURN,SCENE_RTURN,SCENE_RTURN,
                      SCENE_RTURN,SCENE_RTURN,SCENE_RTURN,
                      SCENE_RTURN,

                      SCENE_RTURN,SCENE_RTURN,SCENE_RTURN,
                      SCENE_RTURN,SCENE_RTURN,SCENE_RTURN,
                      SCENE_RTURN,SCENE_RTURN,SCENE_RTURN,
                      SCENE_RTURN,SCENE_RTURN,SCENE_RTURN,
                      SCENE_RTURN,

                      SCENE_STRAIGHT,SCENE_STRAIGHT,SCENE_STRAIGHT,
                      SCENE_STRAIGHT,SCENE_STRAIGHT,
                      SCENE_ARMDOWN
                      
                    };
                      
                      
  
  //初期化用のシーン
  //uint8_t scene[] = {SCENE_INIT,SCENE_INIT};

  Scene nowscene;
  
  //現在のシナリオ数を表示
  msLog("--- scenariocounter ---");
  msLog(scenariocounter);
  //現在のシーン数を表示
  msLog("--- scenecounter ---");
  msLog(scenecounter);

  //シナリオの最後まで行ったら終了
  if (scenariocounter >= sizeof(scene) / sizeof(scene[0]))
  {
    scenariocounter = sizeof(scene) / sizeof(scene[0]);
    *checker = SCENE_END;
    msLog("--- checker ---");
    msLog(*checker);
    return;
  }

  //シナリオ振り分け
  switch (scene[scenariocounter])
  {
  case SCENE_STRAIGHT:
    nowscene = straightSceneInput(scenecounter, &scenechecker);
    break;
  case SCENE_LTURN:
    nowscene = lTurnSceneInput(scenecounter, &scenechecker);
    break;
  case SCENE_RTURN:
    nowscene = rTurnSceneInput(scenecounter, &scenechecker);
    break;
  case SCENE_INIT:
    nowscene = InitSceneInput(scenecounter, &scenechecker);
    break;
  case SCENE_ARM:
    nowscene = ArmSceneInput(scenecounter, &scenechecker);
    break;
  case SCENE_ARMDOWN:
    nowscene = ArmDownSceneInput(scenecounter, &scenechecker);
    break;
  default:
    break;
  }

  //シーンの引数が正常な場合
  if (scenechecker == SCENE_OK)
  {
    scenecounter++;
    *checker = SCENE_OK;
    msLog("--- checker ---");
    msLog(*checker);
    return nowscene;
  }
  //シーンの引数が異常な場合
  if (scenechecker == SCENE_NG)
  {
    msLog("--- sceneInuput scenechecker error ---\n");
    *checker = SCENE_NG;
    msLog("--- checker ---");
    msLog(*checker);
    return;
  }
  //シナリオに含まれるシーンが最後まで行った場合
  if (scenechecker == SCENE_END)
  {
    scenecounter = 0;
    scenariocounter++;
    msLog("--- Scene END ----");
    *checker = SCENE_END;
    return;
  }
  return;
}

/* -------------------------------------------------------------------------- */
/* 関数名		：straightSceneInput													                    */
/* 機能概要	：straightで設定した値に合わせてsceneを返す処理				            */
/* 引数			：uin16_t*	 ：counter	シーンの数を格納                				  */
/* 引数			：uin8_t*		 ：checker	今のシーンの状態を格納				            */
/* 戻り値		：Scene			 ：straight[counter] 今のシーンを返す									*/
/* 作成日		：2023/12/15	山田　龍之介	新規作成      					              */
/* -------------------------------------------------------------------------- */
Scene straightSceneInput(uint16_t counter, uint8_t *checker)
{
  Scene straight[] = {
      // 左前、左中、左後、右前、右中、右後
      // １回動かすごとに約４ｃｍ移動
      // 右前
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 155, 10, 60, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 120, 10, 60, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 120, 40, 60, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // 左中
      {140, 70, 120, 140, 10, 95, 140, 70, 65, 120, 40, 60, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 10, 110, 140, 70, 65, 120, 40, 60, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // 右後
      {140, 70, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 35, 90, 125, 10, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 35, 90, 145, 10, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 35, 90, 145, 43, 115, 30, 30, 00, 1},

      // 左前
      {150, 40, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 35, 90, 145, 43, 115, 30, 30, 00, 1},
      {115, 40, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 35, 90, 145, 43, 115, 30, 30, 00, 1},
      {115, 77, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 35, 90, 145, 43, 115, 30, 30, 00, 1},

      // 右中
      {115, 77, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 10, 90, 145, 43, 115, 30, 30, 00, 1},
      {115, 77, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 10, 75, 145, 43, 115, 30, 30, 00, 1},
      {115, 77, 120, 140, 35, 110, 140, 70, 65, 120, 40, 60, 140, 35, 75, 145, 43, 115, 30, 30, 00, 1},

      // 左後
      {115, 77, 120, 140, 35, 110, 140, 45, 65, 120, 40, 60, 140, 35, 75, 145, 43, 115, 30, 30, 00, 1},
      {115, 77, 120, 140, 35, 110, 155, 45, 65, 120, 40, 60, 140, 35, 75, 145, 43, 115, 30, 30, 00, 1},
      {115, 77, 120, 140, 35, 110, 155, 82, 65, 120, 40, 60, 140, 35, 75, 145, 43, 115, 30, 30, 00, 1},

      // 前進
      {120, 75, 120, 140, 35, 105, 150, 73, 65, 125, 38, 60, 140, 35, 80, 140, 34, 115, 30, 30, 00, 1},
      {125, 73, 120, 140, 35, 100, 145, 72, 65, 130, 36, 60, 140, 35, 85, 135, 33, 115, 30, 30, 00, 1},
      {130, 72, 120, 140, 35, 95, 140, 72, 65, 135, 35, 60, 140, 35, 90, 130, 33, 115, 30, 30, 00, 1}

  };
  msLog("--- straightSceneInput ---");
  msLog(counter);
  if (counter < 0)
  {
    msLog("--- straightSceneInput counter error ---\n");
    *checker = SCENE_NG;
    return;
  }
  if (counter >= sizeof(straight) / sizeof(straight[0]))
  {
    msLog("--- straightSceneInput counter over ---\n");
    *checker = SCENE_END;
    return;
  }
  checker = SCENE_OK;
  return straight[counter];
}

/* -------------------------------------------------------------------------- */
/* 関数名		：lTurnSceneInput													                        */
/* 機能概要	：LTurnで設定した値に合わせてsceneを返す処理				              */
/* 引数			：uin16_t*	 ：counter	シーンの数を格納                				  */
/* 引数			：uin8_t*		 ：checker	今のシーンの状態を格納				            */
/* 戻り値		：Scene			 ：LScene[counter] 今のシーンを返す										*/
/* 作成日		：2023/12/15	山田　龍之介	新規作成      					              */
/* -------------------------------------------------------------------------- */
Scene lTurnSceneInput(uint16_t counter, uint8_t *checker)
{
  Scene LTurn[] = {
      // 左前、左中、左後、右前、右中、右後
      // １２回で90度近く旋回
      // 右前
      {140, 70, 130, 140, 35, 95, 140, 70, 65, 140, 10, 60, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 130, 140, 35, 95, 140, 70, 65, 140, 10, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 130, 140, 35, 95, 140, 70, 65, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // 左中
      {140, 70, 130, 140, 10, 95, 140, 70, 65, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 130, 140, 10, 80, 140, 70, 65, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 130, 140, 35, 80, 140, 70, 65, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // 左後
      {140, 70, 130, 140, 35, 80, 140, 45, 65, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 130, 140, 35, 80, 140, 45, 50, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 130, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // 左前
      {140, 40, 130, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 40, 115, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 77, 115, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // // 右中
      {140, 77, 115, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 10, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 77, 115, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 10, 75, 125, 35, 115, 30, 30, 00, 1},
      {140, 77, 115, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 35, 75, 125, 35, 115, 30, 30, 00, 1},

      // // 右後
      {140, 77, 115, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 35, 75, 125, 10, 115, 30, 30, 00, 1},
      {140, 77, 115, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 35, 75, 125, 10, 100, 30, 30, 00, 1},
      {140, 77, 115, 140, 35, 80, 140, 82, 50, 140, 40, 45, 140, 35, 75, 125, 43, 100, 30, 30, 00, 1},

      // // 前進
      {140, 77, 120, 140, 35, 85, 140, 82, 55, 140, 40, 50, 140, 35, 80, 125, 43, 105, 30, 30, 00, 1},
      {140, 77, 125, 140, 35, 90, 140, 82, 60, 140, 40, 55, 140, 35, 85, 125, 43, 110, 30, 30, 00, 1},
      {140, 77, 130, 140, 35, 95, 140, 82, 65, 140, 40, 60, 140, 35, 90, 125, 43, 115, 30, 30, 00, 1},

  };
  msLog("--- lTurnSceneInput ---");
  msLog(counter);
  if (counter < 0)
  {
    msLog("--- lTurnSceneInput counter error ---\n");
    *checker = SCENE_NG;
    return;
  }
  if (counter >= sizeof(LTurn) / sizeof(LTurn[0]))
  {
    msLog("--- lTurnSceneInput counter over ---\n");
    *checker = SCENE_END;
    return;
  }
  return LTurn[counter];
}

/* -------------------------------------------------------------------------- */
/* 関数名		：rTurnSceneInput													                        */
/* 機能概要	：RTurnで設定した値に合わせてsceneを返す処理				              */
/* 引数			：uin16_t*	 ：counter	シーンの数を格納                				  */
/* 引数			：uin8_t*		 ：checker	今のシーンの状態を格納				            */
/* 戻り値		：Scene			 ：RScene[counter] 今のシーンを返す										*/
/* 作成日		：2023/12/15	山田　龍之介	新規作成      					              */
/* -------------------------------------------------------------------------- */
Scene rTurnSceneInput(uint16_t counter, uint8_t *checker)
{
  Scene RTurn[] = {
      // １２回で90度近く旋回
      // 右前
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 10, 60, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 10, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // 左中
      {140, 70, 120, 140, 10, 95, 140, 70, 65, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 10, 110, 140, 70, 65, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 110, 140, 70, 65, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // 左後
      {140, 70, 120, 140, 35, 110, 140, 45, 65, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 110, 140, 45, 80, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 70, 120, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // 左前
      {140, 40, 120, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 40, 135, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 77, 135, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 35, 90, 125, 35, 115, 30, 30, 00, 1},

      // 右中
      {140, 77, 135, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 10, 90, 125, 35, 115, 30, 30, 00, 1},
      {140, 77, 135, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 10, 105, 125, 35, 115, 30, 30, 00, 1},
      {140, 77, 135, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 35, 105, 125, 35, 115, 30, 30, 00, 1},

      // 右後
      {140, 77, 135, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 35, 105, 125, 10, 115, 30, 30, 00, 1},
      {140, 77, 135, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 35, 105, 125, 10, 130, 30, 30, 00, 1},
      {140, 77, 135, 140, 35, 110, 140, 82, 80, 140, 40, 75, 140, 35, 105, 125, 43, 130, 30, 30, 00, 1},

      // 右旋回
      {140, 77, 130, 140, 35, 105, 140, 82, 75, 140, 40, 70, 140, 35, 100, 125, 43, 125, 30, 30, 00, 1},
      {140, 77, 125, 140, 35, 100, 140, 82, 70, 140, 40, 65, 140, 35, 95, 125, 43, 120, 30, 30, 00, 1},
      {140, 77, 120, 140, 35, 95, 140, 82, 65, 140, 40, 60, 140, 35, 90, 125, 43, 115, 30, 30, 00, 1},
      };
  msLog("--- rTurnSceneInput ---");
  msLog(counter);
  if (counter < 0)
  {
    msLog("--- rTurnSceneInput counter error ---\n");
    *checker = SCENE_NG;
    return;
  }
  if (counter >= sizeof(RTurn) / sizeof(RTurn[0]))
  {
    msLog("--- rTurnSceneInput counter over ---\n");
    *checker = SCENE_END;
    return;
  }
  return RTurn[counter];
}

/* -------------------------------------------------------------------------- */
/* 関数名		：InitSceneInput												                          */
/* 機能概要	：Initで設定した値に合わせてsceneを返す処理				                */
/* 引数			：uin16_t*	 ：counter	シーンの数を格納                				  */
/* 引数			：uin8_t*		 ：checker	今のシーンの状態を格納				            */
/* 戻り値		：Scene			 ：Init[counter] 今のシーンを返す										  */
/* 作成日		：2023/12/15	山田　龍之介	新規作成      					              */
/* -------------------------------------------------------------------------- */
Scene InitSceneInput(uint16_t counter, uint8_t *checker)
{
  Scene Init[] = {
      // 初期位置
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 30, 30, 0, 0}};
  msLog("--- InitSceneInput ---");
  msLog(counter);
  if (counter < 0)
  {
    msLog("--- InitSceneInput counter error ---\n");
    *checker = SCENE_NG;
    return;
  }
  if (counter >= sizeof(Init) / sizeof(Init[0]))
  {
    msLog("--- InitSceneInput counter over ---\n");
    *checker = SCENE_END;
    return;
  }
  return Init[counter];
}

/* -------------------------------------------------------------------------- */
/* 関数名		：ArmSceneInput												                            */
/* 機能概要	：Armで設定した値に合わせてsceneを返す処理				                */
/* 引数			：uin16_t*	 ：counter	シーンの数を格納                				  */
/* 引数			：uin8_t*		 ：checker	今のシーンの状態を格納				            */
/* 戻り値		：Scene			 ：Arm[counter] 今のシーンを返す										  */
/* 作成日		：2023/12/15	山田　龍之介	新規作成      					              */
/* -------------------------------------------------------------------------- */
Scene ArmSceneInput(uint16_t counter, uint8_t *checker)
{
  Scene Arm[] = {
      // 初期位置
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 270, 270,   0, 0},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 270, 270, 500, 0},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 270, 270, 500, 1},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 270, 270,   0, 1},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115,   29,   3,   0, 1},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115,   29,   3,   0, 1}
      

  };
  msLog("--- ArmSceneInput ---");
  msLog(counter);
  if (counter < 0)
  {
    msLog("--- InitSceneInput counter error ---\n");
    *checker = SCENE_NG;
    return;
  }
  if (counter >= sizeof(Arm) / sizeof(Arm[0]))
  {
    msLog("--- InitSceneInput counter over ---\n");
    *checker = SCENE_END;
    return;
  }
  return Arm[counter];
}

/* -------------------------------------------------------------------------- */
/* 関数名		：ArmDownSceneInput												                        */
/* 機能概要	：Armで設定した値に合わせてsceneを返す処理				                */
/* 引数			：uin16_t*	 ：counter	シーンの数を格納                				  */
/* 引数			：uin8_t*		 ：checker	今のシーンの状態を格納				            */
/* 戻り値		：Scene			 ：Arm[counter] 今のシーンを返す										  */
/* 作成日		：2023/12/15	山田　龍之介	新規作成      					              */
/* -------------------------------------------------------------------------- */
Scene ArmDownSceneInput(uint16_t counter, uint8_t *checker)
{
  Scene Arm[] = {
      // 初期位置
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 350, 270,   0, 1},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 350, 270, 500, 1},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 350, 270, 500, 0},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 350, 270,   0, 0},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 109,   3,   0, 0},
      {140, 70, 120, 140, 35, 95, 140, 70, 65, 140, 35, 60, 140, 35, 90, 125, 35, 115, 109,   3,   0, 0}
      

  };
  msLog("--- ArmSceneInput ---");
  msLog(counter);
  if (counter < 0)
  {
    msLog("--- InitSceneInput counter error ---\n");
    *checker = SCENE_NG;
    return;
  }
  if (counter >= sizeof(Arm) / sizeof(Arm[0]))
  {
    msLog("--- InitSceneInput counter over ---\n");
    *checker = SCENE_END;
    return;
  }
  return Arm[counter];
}

/* -------------------------------------------------------------------------- */
/* 関数名		：sceneBusy												                                */
/* 機能概要	：各モータが動いている最中かを判断する処理				                */
/* 引数			：なし	     ：なし                				                        */
/* 戻り値		：int			   ：OK (0) / NG (1)										                */
/* 作成日		：2023/12/15	山田　龍之介	新規作成      					              */
/* -------------------------------------------------------------------------- */
int sceneBusy()
{
  uint8_t handbusy = 0;
  uint8_t elevatorbusy = 0;
  uint8_t armbusy[2] = {0, 0};
  uint8_t legbusy[SERVO_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int count = 0;

  msServoGetBusy(legbusy, sizeof(legbusy) / sizeof(legbusy[0]));
  msDCGetBusy(armbusy, sizeof(armbusy) / sizeof(armbusy[0]));
  msRODGetBusy(&elevatorbusy);
  msHANDGetBusy(&handbusy);

  if (
      handbusy == 0 &&
      elevatorbusy == 0 &&
      armbusy[0] == 0 && armbusy[1] == 0 &&
      legbusy[0] == 0 && legbusy[1] == 0 && legbusy[2] == 0 && legbusy[3] == 0 && legbusy[4] == 0 && legbusy[5] == 0 && legbusy[6] == 0 && legbusy[7] == 0 && legbusy[8] == 0 && legbusy[9] == 0 && legbusy[10] == 0 && legbusy[11] == 0 && legbusy[12] == 0 && legbusy[13] == 0 && legbusy[14] == 0 && legbusy[15] == 0 && legbusy[16] == 0 && legbusy[17] == 0)
  {
    return SCENE_OK;
  }
  else
  {
    return SCENE_NG;
  }
}

/* -------------------------------------------------------------------------- */
/* 関数名		：scene												                                    */
/* 機能概要	：実際にインプットシーンでもらった数値を実行する処理				      */
/* 引数			：なし	     ：なし                				                        */
/* 戻り値		：int			   ：OK (0) / NG (1)										                */
/* 作成日		：2023/12/15	山田　龍之介	新規作成      					              */
/* -------------------------------------------------------------------------- */
int scene()
{
  static uint16_t scenecounter = 0;
  SLNG returnvalue[18];
  int sceneNext = SCENE_NG;
  uint16_t scene_num = 0;
  Scene *scene;
  Scene nowscene;
  uint8_t scenechecker = SCENE_OK;

  uint16_t angle[SERVO_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint16_t arm[ARM_NUM] = {0, 0};
  uint16_t elevator[ELEVATOR_NUM] = {0};
  SSHT handopen[HANDOPEN_NUM] = {0};

  sceneNext = sceneBusy(); // モータが動いている状態かを判断

  if (sceneNext == SCENE_OK)
  {

    msLog("--- Scene Input Start ---");
    delay(100);
    nowscene = sceneInuput(&scenechecker); // シーンデータの取り込み

    msLog("--- scenechecker ---");
    msLog(scenechecker);
    if (scenechecker == SCENE_NG)
    {
      msLog("--- *ERROR SceneInput PARAM *---");
      return SCENE_END;
    }
    if (scenechecker == SCENE_END)
    {
      msLog("--- scene counter over ---");
      return SCENE_END;
    }

    msLog("--- angle set ---");
    angle[0] = nowscene.lf_neemotor;             // servodataを抽出
    angle[1] = nowscene.lf_pitch_hipjointmotor;  // servodataを抽出
    angle[2] = nowscene.lf_yaw_hipjointmotor;    // servodataを抽出
    angle[3] = nowscene.lc_neemotor;             // servodataを抽出
    angle[4] = nowscene.lc_pitch_hipjointmotor;  // servodataを抽出
    angle[5] = nowscene.lc_yaw_hipjointmotor;    // servodataを抽出
    angle[6] = nowscene.lb_kneemotor;            // servodataを抽出
    angle[7] = nowscene.lb_pitch_hipjointmotor;  // servodataを抽出
    angle[8] = nowscene.lb_yaw_hipjointmotor;    // servodataを抽出
    angle[9] = nowscene.rf_neemotor;             // servodataを抽出
    angle[10] = nowscene.rf_pitch_hipjointmotor; // servodataを抽出
    angle[11] = nowscene.rf_yaw_hipjointmotor;   // servodataを抽出
    angle[12] = nowscene.rc_neemotor;            // servodataを抽出
    angle[13] = nowscene.rc_pitch_hipjointmotor; // servodataを抽出
    angle[14] = nowscene.rc_yaw_hipjointmotor;   // servodataを抽出
    angle[15] = nowscene.rb_kneemotor;           // servodataを抽出
    angle[16] = nowscene.rb_pitch_hipjointmotor; // servodataを抽出
    angle[17] = nowscene.rb_yaw_hipjointmotor;   // servodataを抽出
    arm[0] = nowscene.l_armmotor;                // armdataを抽出
    arm[1] = nowscene.r_armmotor;                // armdataを抽出
    elevator[0] = nowscene.elevator;             // elevatordataを抽出
    handopen[0] = nowscene.hand;                 // handopendataを抽出
    msServoSet(returnvalue, angle, SERVO_NUM);   // サーボに指令とエンコーダがないためbusy時間の設定
    msDCSet(returnvalue, arm, ARM_NUM);          // アームに指令
    msRODSet(returnvalue, elevator);             // 昇降機に指令
    msHANDSet(returnvalue, handopen);            // ハンド開閉に指令とエンコーダがないためbusy時間の設定

    return SCENE_OK;
  }
  else if (sceneNext == SCENE_NG)
  {
    //msLog("--- sceneBusy ---");
    return SCENE_NG;
  }
}
