
#include "beta-cube-library.h"


int x, y, z;

void setup() {
    cube.begin();

}


void loop() {
    
  uint16_t i, j;

  for(j=0; j<256*5; j+=2) { // 5 cycles of all colors on wheel
    for(i=0; i< 512; i++) {
      cube.setPixelColor(i, Wheel(((i / 2) + j) & 255));
    }
    cube.show();
//    delay(1);
  }
  

  
}



uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return cube.Color(WheelPos, 85 - WheelPos, 0);
  }
  else if(WheelPos < 170) {
   WheelPos -= 85;
   return cube.Color(85 - WheelPos, 0, WheelPos);
  } 
  else {
   WheelPos -= 170;
   return cube.Color(0, WheelPos, 85 - WheelPos);
  }
}