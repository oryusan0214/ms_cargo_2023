#include "spi.h"
void init_spi(){
  spi.setBitOrder(MSBFIRST);
  spi.setDataMode(SPI_MODE2);
  spi.setClockDivider(SPI_CLOCK_DIV2);
  spi.begin();
  digitalWrite(55, LOW);
  
  
}
void set_data(unsigned char data){
  spi.transfer(data);
}