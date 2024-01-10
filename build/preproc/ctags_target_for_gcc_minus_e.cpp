# 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino"
/* -------------------------------------------------------------------------- */
/* ms_project.ino																 															*/
/* シ  ス  テ  ム  メ  イ  ン												 												  */
/* アプリケーションエントリーポイントです。(Arduinoではここが								  */
/*																					エントリポジションです。)					*/
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* 番号		更新履歴								日付　氏名																  */
/* -------------------------------------------------------------------------- */
/* 000000	新規作成								2023/12/04　山田龍之介		 								  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* includeファイル															  														*/
/* -------------------------------------------------------------------------- */
       
# 18 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2
# 19 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2
# 20 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2
# 21 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2
# 22 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2
# 23 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2

# 25 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2
# 26 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2
# 27 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2
# 28 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\ms_project.ino" 2
/* ------------------------------------------------------------------------ */
/* */
//int=1byte
//short=2byte
//long=4byte

void setup()
{
 Serial.begin(115200);
 //SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE2);
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
  msTimeInit();
  msLogInit();
  msServoInit();
  msDCInit();
  msHANDInit();
  msRODInit();
  SPI.begin();
  digitalWrite(SS, 0x1);
  Serial.println("System Start");
  delay(1);
}

void loop()
{
 test();
  msTimeCallbackExecute();

}
