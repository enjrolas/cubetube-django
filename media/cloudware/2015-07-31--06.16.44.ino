
#include "beta-cube-library.h"

// This #include statement was automatically added by the Spark IDE.
#include "neopixel/neopixel.h"

#include <math.h>  //for the sqrt and pow() functions



int x, y, z;
unsigned char red, green, blue;


void setup() {
    cube.begin();

}

//sets up the online/offline switch


void loop() {
    
  uint16_t i, j;

  for(j=0; j<256*5; j+=2) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    cube.show();
//    delay(1);
  }
  

  
}



void setPixel(int x, int y, int z, int r, int g, int b)
{
    int index = (z*64) + (x*8) + y;
    strip.setPixelColor(index,strip.Color(r,g,b));
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos, 85 - WheelPos, 0);
  }
  else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(85 - WheelPos, 0, WheelPos);
  } 
  else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos, 85 - WheelPos);
  }
}