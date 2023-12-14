/* Scene.run---------------------------------------------------------------- */
/*  1シーンを実行する */

/*  */

/* ------------------------------------------------------------------------- */
#include "scene.h"
#include "servo.h"
#include "dc.h"
#include "hand.h"
#include "rod.h"
#include "log.h"
#include <string.h>


/* Scene.judge */
/* シーンを更新するかを判断すする */

void sceneInuput(Scene *scene){
  Scene inputscene[]={
  /* --- ここからシーンを動かしていくこと ---*/
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
  };

  scene = inputscene;
}

int sceneBusy(){
  uint8_t handbusy=0;
  uint8_t elevatorbusy=0;
  uint8_t armbusy[2]={0,0};
  uint8_t legbusy[SERVO_NUM]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int count = 0;

  msServoGetBusy(legbusy,sizeof(legbusy)/sizeof(legbusy[0]));
  msDCGetBusy(armbusy,sizeof(armbusy)/sizeof(armbusy[0]));
  msRODGetBusy(elevatorbusy);
  msHANDGetBusy(handbusy);

  if(
      handbusy==0 && 
      elevatorbusy==0 && 
      armbusy[0]==0 && armbusy[1]==0 && 
      legbusy[0]==0 && legbusy[1]==0 && legbusy[2]==0 && legbusy[3]==0 && legbusy[4]==0 && legbusy[5]==0 && legbusy[6]==0 && legbusy[7]==0 && legbusy[8]==0 && legbusy[9]==0 && legbusy[10]==0 && legbusy[11]==0 && legbusy[12]==0 && legbusy[13]==0 && legbusy[14]==0 && legbusy[15]==0 && legbusy[16]==0 && legbusy[17]==0
    ){
    return SCENE_OK;
  }
  else{
    return SCENE_NG;
  }
}
int scene(){
  static uint16_t scenecounter = 0;
  int returnvalue = 0;
  int sceneNext = SCENE_NG;
  Scene *scene;

  uint16_t angle[SERVO_NUM]=        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  uint16_t arm[ARM_NUM] =           {0,0};
  uint16_t elevator[ELEVATOR_NUM] = {0};
  uint16_t handopen[HANDOPEN_NUM] = {0};

  
  sceneNext = sceneBusy();//モータが動いている状態かを判断

  if (sceneNext == SCENE_OK)
  {
    msLog("--- scene ---\n");
    msLog("--now couter = %d --\n",scenecounter);
    sceneInuput(scene);//シーンデータの取り込み
    memcpy(angle,&scene[scenecounter],sizeof(angle)/sizeof(angle[0]));//servodataを抽出
    memcpy(arm,scene[scenecounter].l_armmotor,sizeof(arm)/sizeof(arm[0]));//armdataを抽出
    memcpy(elevator,scene[scenecounter].elevator,sizeof(elevator)/sizeof(elevator[0]));//elevatordataを抽出
    memcpy(handopen,scene[scenecounter].hand,sizeof(handopen)/sizeof(handopen[0]));//handopendataを抽出
    msServoSet(returnvalue,angle,SERVO_NUM);//サーボに指令とエンコーダがないためbusy時間の設定
    msDCSet   (returnvalue,arm,ARM_NUM);//アームに指令
    msRODSet  (returnvalue,elevator);//昇降機に指令
    msHANDSet (returnvalue,handopen);//ハンド開閉に指令とエンコーダがないためbusy時間の設定
    scenecounter = scenecounter + 1;//次のシーンに移る
  }
  else if (sceneNext == SCENE_NG)
  {
    msLog("--- sceneBusy() ---\n");
  }
  

}

