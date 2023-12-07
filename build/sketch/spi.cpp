#line 1 "C:\\WORK\\学校\\未来創造展\\ms_project\\ms_project\\spi.cpp"
#include "spi.h"
void init_spi(){
  SCHR sendformat[] = 
  {
    FLOATSIZE,FLOATSIZE,FLOATSIZE,UINTSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,
  };
  SCHR catchformat[] = 
  {
    UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UINTSIZE,UCHRSIZE,UCHRSIZE,UCHRSIZE,
  };
  SCHR sendcount = 0; 
  SCHR sendformatcount = 0;
  SCHR catchcount = 0;
  SCHR catchformatcount = 0;
  SCHR datacount = 0;
  spi.setBitOrder(MSBFIRST);
  spi.setDataMode(SPI_MODE2);
  spi.setClockDivider(SPI_CLOCK_DIV2);
  spi.begin();
  digitalWrite(55, LOW);
  for ( datacount = 0; datacount < 44; datacount++)
  {

    for ( sendcount = 0; sendcount < sizeof(sendformat)/sizeof(SCHR); sendcount++)
    {
      
    }
    
    for ( catchcount = 0; catchcount < sizeof(catchcount)/sizeof(SCHR); catchcount++)
    {
      
    }
    
  }
  

  
}
void set_data(unsigned char data){
  spi.transfer(data);
}