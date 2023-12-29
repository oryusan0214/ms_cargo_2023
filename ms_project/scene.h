#define SCENE_OK      (      0)
#define SCENE_NG      (      1)
#define SCENE_END     (      2)

#define SERVO_NUM     (     18)
#define ARM_NUM       (      2)
#define ELEVATOR_NUM  (      1)
#define HANDOPEN_NUM  (      1)

#define SCENE_STRAIGHT (     1)
#define SCENE_LTURN    (     2)
#define SCENE_RTURN    (     3)
#define SCENE_INIT    (     4)

#include <stdint.h>



typedef struct  Scene
{
  uint16_t  lf_neemotor;
  uint16_t  lf_pitch_hipjointmotor;
  uint16_t  lf_yaw_hipjointmotor;
  uint16_t  lc_neemotor;
  uint16_t  lc_pitch_hipjointmotor;
  uint16_t  lc_yaw_hipjointmotor;
  uint16_t  lb_kneemotor;
  uint16_t  lb_pitch_hipjointmotor;
  uint16_t  lb_yaw_hipjointmotor;
  uint16_t  rf_neemotor;
  uint16_t  rf_pitch_hipjointmotor;
  uint16_t  rf_yaw_hipjointmotor;
  uint16_t  rc_neemotor;
  uint16_t  rc_pitch_hipjointmotor;
  uint16_t  rc_yaw_hipjointmotor;
  uint16_t  rb_kneemotor;
  uint16_t  rb_pitch_hipjointmotor;
  uint16_t  rb_yaw_hipjointmotor;
  uint16_t  l_armmotor;
  uint16_t  r_armmotor;
  uint16_t  elevator;
  uint16_t  hand;
} Scene;


int sceneBusy();
int scene();
Scene sceneInuput(uint8_t* checker);
Scene straightSceneInput(uint16_t counter, uint8_t* checker);
Scene lTurnSceneInput(uint16_t counter, uint8_t* checker);
Scene rTurnSceneInput(uint16_t counter, uint8_t* checker);
Scene InitSceneInput(uint16_t counter, uint8_t* checker);