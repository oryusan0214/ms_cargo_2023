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
#include <stdio.h>

/* Scene.judge */
/* シーンを更新するかを判断すする */

void sceneInuput(*Scene scene){
  Scene inputscene[]=
  /* --- ここからシーンを動かしていくこと ---*/
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  scene = &inputscene;
}

int sceneBusy(){
  SCHR handbusy[1]=[0];
  SCHR elevatorbusy[1]={0};
  SCHR armbusy[2]={0,0};
  SCHR legbusy[18]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int count = 0;

  msHANDGetBusy(handbusy);
  msRODGetBusy(elevatorbusy);
  msDCGetBusy(armbyus,sizeof(armbusy)/sizeof(armbusy[0]));
  msServoGetBusy(legbusy,sizeof(legbusy)/sizeof(legbusy[0]));

  if(handbusy[0]==0 && 
     elevatorbusy[0]==0 && 
     armbusy[0]==0 && armbusy[1]==0 && 
     legbusy[0]==0 && legbusy[1]==0 && legbusy[2]==0 && legbusy[3]==0 && legbusy[4]==0 && legbusy[5]==0 && legbusy[6]==0 && legbusy[7]==0 && legbusy[8]==0 && legbusy[9]==0 && legbusy[10]==0 && legbusy[11]==0 && legbusy[12]==0 && legbusy[13]==0 && legbusy[14]==0 && legbusy[15]==0 && legbusy[16]==0 && legbusy[17]==0){
    return SCENE_OK;
  }
  else{
    return SCENE_NG;
  }
}
int scene(){
  static uint16_t counter = 0;
  int returnvalue = 0;
  int sceneNext = SCENE_NG;
  Scene scene;
  uint16_t angle[18]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  sceneInuput(&scene);
  sceneNext = sceneBusy();
  memcpy(angle,scene,sizeof(angle)/sizeof(angle[0]));

  if (sceneNext == SCENE_OK)
  {
    msServoSet();
  }
  else if (sceneNext == SCENE_NG)
  {
    /* code */
  }
  

}

