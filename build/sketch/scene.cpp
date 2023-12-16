#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\scene.cpp"
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
#include "time.h"



volatile static Scene inputscene[]={
  /* --- ここからシーンを動かしていくこと ---*/
  //初期位置
  {140  ,70 ,140  ,140  ,35 ,95 ,145  ,70 ,44 ,140  ,35 ,50 ,140  ,35 ,90 ,140  ,35 ,132  ,00 ,00 ,00 ,00},
  //
  {140  ,70 ,140  ,140  ,35 ,95 ,145  ,70 ,44 ,140  ,10 ,50 ,140  ,35 ,90 ,140  ,35 ,132  ,00 ,00 ,00 ,00},
  {140  ,70 ,140  ,140  ,35 ,95 ,145  ,70 ,44 ,120  ,10 ,50 ,140  ,35 ,90 ,140  ,35 ,132  ,00 ,00 ,00 ,00},
  {140  ,70 ,140  ,140  ,35 ,95 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,140  ,35 ,132  ,00 ,00 ,00 ,00},

  {140  ,70 ,140  ,140  ,10 ,95 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,140  ,35 ,132  ,00 ,00 ,00 ,00},
  {140  ,70 ,140  ,140  ,10 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,140  ,35 ,132  ,00 ,00 ,00 ,00},
  {140  ,70 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,140  ,35 ,132  ,00 ,00 ,00 ,00},

  {140  ,70 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,140  ,10 ,132  ,00 ,00 ,00 ,00},
  {140  ,70 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,145  ,10 ,132  ,00 ,00 ,00 ,00},
  {140  ,70 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,145  ,43 ,132  ,00 ,00 ,00 ,00},

  {140  ,40 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,145  ,43 ,132  ,00 ,00 ,00 ,00},
  {115  ,40 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,145  ,43 ,132  ,00 ,00 ,00 ,00},
  {115  ,77 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,90 ,145  ,43 ,132  ,00 ,00 ,00 ,00},

  {115  ,77 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,10 ,90 ,145  ,43 ,132  ,00 ,00 ,00 ,00},
  {115  ,77 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,10 ,75 ,145  ,43 ,132  ,00 ,00 ,00 ,00},
  {115  ,77 ,140  ,140  ,35 ,110 ,145  ,70 ,44 ,120  ,40 ,50 ,140  ,35 ,75 ,145  ,43 ,132  ,00 ,00 ,00 ,00},

  {115  ,77 ,140  ,140  ,35 ,110 ,145  ,45 ,44 ,120  ,40 ,50 ,140  ,35 ,75 ,145  ,43 ,132  ,00 ,00 ,00 ,00},
  {115  ,77 ,140  ,140  ,35 ,110 ,155  ,45 ,44 ,120  ,40 ,50 ,140  ,35 ,75 ,145  ,43 ,132  ,00 ,00 ,00 ,00},
  {115  ,77 ,140  ,140  ,35 ,110 ,155  ,82 ,44 ,120  ,40 ,50 ,140  ,35 ,75 ,145  ,43 ,132  ,00 ,00 ,00 ,00},
  //
  {120  ,75 ,140  ,140  ,35 ,105 ,150  ,73 ,44 ,125  ,38 ,50 ,140  ,35 ,80 ,140  ,34 ,132  ,00 ,00 ,00 ,00},
  {125  ,73 ,140  ,140  ,35 ,100 ,145  ,72 ,44 ,130  ,36 ,50 ,140  ,35 ,85 ,135  ,33 ,132  ,00 ,00 ,00 ,00},
  {130  ,72 ,140  ,140  ,35 ,95 ,140  ,72 ,44 ,135  ,35 ,50 ,140  ,35 ,90 ,130  ,33 ,132  ,00 ,00 ,00 ,00},
  {135  ,72 ,140  ,140  ,35 ,90 ,135  ,73 ,44 ,140  ,35 ,50 ,140  ,35 ,95 ,125  ,34 ,132  ,00 ,00 ,00 ,00},
  {140  ,73 ,140  ,140  ,35 ,85 ,130  ,75 ,44 ,145  ,36 ,50 ,140  ,35 ,100 ,120  ,36 ,132  ,00 ,00 ,00 ,00},
  {145  ,82 ,140  ,140  ,35 ,80 ,125  ,77 ,44 ,150  ,45 ,50 ,140  ,35 ,105 ,115  ,38 ,132  ,00 ,00 ,00 ,00},

  //初期位置
  {140  ,70 ,140  ,140  ,35 ,95 ,145  ,70 ,44 ,140  ,35 ,50 ,140  ,35 ,90 ,140  ,35 ,132  ,00 ,00 ,00 ,00}

  };

/* Scene.judge */
/* シーンを更新するかを判断すする */

int sceneInuput(Scene *scene){
  /*
  volatile static Scene inputscene[]={
  /* --- ここからシーンを動かしていくこと ---*//*
  {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22},
  {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10},
  {20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20},
  {30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30}
  };*/

  //scene = inputscene;
  return sizeof(inputscene)/sizeof(inputscene[0]);
}

int sceneBusy(){
  uint8_t handbusy=0;
  uint8_t elevatorbusy=0;
  uint8_t armbusy[2]={0,0};
  uint8_t legbusy[SERVO_NUM]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int count = 0;

  msServoGetBusy(legbusy,sizeof(legbusy)/sizeof(legbusy[0]));
  for(int i = 0;i < SERVO_MAX;i++){
    //Serial.println("busy");
    //Serial.println(legbusy[i]);
  }
  //msDCGetBusy(armbusy,sizeof(armbusy)/sizeof(armbusy[0]));
  //msRODGetBusy(&elevatorbusy);
  //msHANDGetBusy(&handbusy);

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
  SLNG returnvalue[18];
  int sceneNext = SCENE_NG;
  uint16_t scene_num = 0;
  Scene *scene;

  uint16_t angle[SERVO_NUM]=        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  uint16_t arm[ARM_NUM] =           {0,0};
  uint16_t elevator[ELEVATOR_NUM] = {0};
  SSHT handopen[HANDOPEN_NUM] = {0};

  
  sceneNext = sceneBusy();//モータが動いている状態かを判断
 
  

  if (sceneNext == SCENE_OK)
  {
    
    Serial.println("--- scene ---\n");
    
    //Serial.println("--now couter  --");
    
    //Serial.println(scenecounter);
    //delay(1);
    
    Serial.println("--- Scene Input Start ---");
    scene = inputscene;
    scene_num=sceneInuput(scene);//シーンデータの取り込み

    if (scene_num == 0)
    {
      Serial.println("WARNING --- NON SCENE ---\n");
    
      return SCENE_END;
    }
    if (scene_num <= scenecounter)
    {
      Serial.println("--- scene counter over ---");
      return SCENE_END;
    }
  
    //memcpy(angle,&scene[scenecounter],sizeof(angle)/sizeof(angle[0]));//servodataを抽出
    angle[0] =  scene[scenecounter].lf_neemotor;            //servodataを抽出
    angle[1] =  scene[scenecounter].lf_pitch_hipjointmotor; //servodataを抽出
    angle[2] =  scene[scenecounter].lf_yaw_hipjointmotor;   //servodataを抽出
    angle[3] =  scene[scenecounter].lc_neemotor;            //servodataを抽出
    angle[4] =  scene[scenecounter].lc_pitch_hipjointmotor; //servodataを抽出
    angle[5] =  scene[scenecounter].lc_yaw_hipjointmotor;   //servodataを抽出
    angle[6] =  scene[scenecounter].lb_kneemotor;           //servodataを抽出
    angle[7] =  scene[scenecounter].lb_pitch_hipjointmotor; //servodataを抽出
    angle[8] =  scene[scenecounter].lb_yaw_hipjointmotor;   //servodataを抽出
    angle[9] =  scene[scenecounter].rf_neemotor;            //servodataを抽出
    angle[10] = scene[scenecounter].rf_pitch_hipjointmotor; //servodataを抽出
    angle[11] = scene[scenecounter].rf_yaw_hipjointmotor;   //servodataを抽出
    angle[12] = scene[scenecounter].rc_neemotor;            //servodataを抽出
    angle[13] = scene[scenecounter].rc_pitch_hipjointmotor; //servodataを抽出
    angle[14] = scene[scenecounter].rc_yaw_hipjointmotor;   //servodataを抽出
    angle[15] = scene[scenecounter].rb_kneemotor;           //servodataを抽出
    angle[16] = scene[scenecounter].rb_pitch_hipjointmotor; //servodataを抽出
    angle[17] = scene[scenecounter].rb_yaw_hipjointmotor;   //servodataを抽出
    Serial.println("-----------ang koko --------------");
    Serial.println(scenecounter);
    Serial.println(angle[16]);
    Serial.println(angle[17]);
    //memcpy(arm,scene[scenecounter].l_armmotor,sizeof(arm)/sizeof(arm[0]));
    arm[0]=scene[scenecounter].l_armmotor;                  //armdataを抽出
    arm[1]=scene[scenecounter].r_armmotor;                  //armdataを抽出

    elevator[0]=scene[scenecounter].elevator;                  //elevatordataを抽出

    handopen[0]=scene[scenecounter].hand;                      //handopendataを抽出
    
    Serial.println("--- angle check ---");
    Serial.println(angle[0]);
    
    msServoSet(returnvalue,angle,SERVO_NUM);//サーボに指令とエンコーダがないためbusy時間の設定
    msDCSet   (returnvalue,arm,ARM_NUM);//アームに指令
    msRODSet  (returnvalue,elevator);//昇降機に指令
    msHANDSet (returnvalue,handopen);//ハンド開閉に指令とエンコーダがないためbusy時間の設定
    Serial.println(scenecounter);
    //delay(1);
    scenecounter = scenecounter + 1;//次のシーンに移る
    return SCENE_OK;
  }
  else if (sceneNext == SCENE_NG)
  {
    Serial.println("--- sceneBusy() ---\n");
    
    return SCENE_NG;
  }
  

}

