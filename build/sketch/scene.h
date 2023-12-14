#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\scene.h"
#define SCENE_OK (  0)
#define SCENE_NG ( -1)
#include <stdint.h>



typedef struct  Scene
{
  uint16_t  lf_neemotor;
  uint16_t  lf_yaw_hipjointmotor;
  uint16_t  lf_pitch_hipjointmotor;
  uint16_t  lc_neemotor;
  uint16_t  lc_yaw_hipjointmotor;
  uint16_t  lc_pitch_hipjointmotor;
  uint16_t  lb_kneemotor;
  uint16_t  lb_yaw_hipjointmotor;
  uint16_t  lb_pitch_hipjointmotor;
  uint16_t  rf_neemotor;
  uint16_t  rf_yaw_hipjointmotor;
  uint16_t  rf_pitch_hipjointmotor;
  uint16_t  rc_neemotor;
  uint16_t  rc_yaw_hipjointmotor;
  uint16_t  rc_pitch_hipjointmotor;
  uint16_t  rb_kneemotor;
  uint16_t  rb_yaw_hipjointmotor;
  uint16_t  rb_pitch_hipjointmotor;
  uint16_t  l_armmotor;
  uint16_t  r_armmotor;
  uint16_t  elevator;
  uint16_t  arm;
  uint16_t  hand;
} Scene;

void sceneInuput(*Scene);
int sceneBusy();
int scene();
