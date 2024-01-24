#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\spi.cpp"

#include "spi.h"

/* -------------------------------------------------------------------------- */
/* グローバル変数宣言														                              */
/* -------------------------------------------------------------------------- */
send_data senderdata;
catch_data catcherdata;
SPISettings mySPISettings = SPISettings(8000000, MSBFIRST, SPI_MODE2);

void spi(){
  
  UCHR catchdata[48];
  UCHR senddata[40];
  UCHR datacount = 0;
  UCHR sender;

  //SPI.setBitOrder(MSBFIRST);
  //SPI.setDataMode(SPI_MODE2);
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
  
  SPI.beginTransaction(mySPISettings);
  digitalWrite(SS, LOW);
  
  memcpy(senddata, &senderdata, sizeof(senderdata));
  
  for ( datacount = 0; datacount < 48; datacount++)
  {
    
    if (datacount<40)
    {
      sender=senddata[datacount];
      msLog(sender,HEX);
      catchdata[datacount]=SPI.transfer(sender);
    }
    else
    {
      sender=0x00;
      msLog(sender,HEX);
      catchdata[datacount]=SPI.transfer(sender);
    }
    delayMicroseconds(100);
    
    
  }

  memcpy(&catcherdata, catchdata, sizeof(catcherdata));
  

  digitalWrite(SS, HIGH); 
}