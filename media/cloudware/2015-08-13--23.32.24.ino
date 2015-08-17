
#include "beta-cube-library.h"

//s#include "beta-cube-library.h"
#include "neopixel.h"



SYSTEM_MODE(MANUAL);  //don't connect to the internet on boot
#define BUTTON D2 //press this button to connect to the internet
#define MODE D3
bool onlinePressed=false;
bool lastOnline=true;

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D0
#define PIXEL_COUNT 512
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel leds = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() {
    leds.begin();
    leds.show(); 
}



void loop() {
    
  uint16_t i, j;

  for(j=0; j<256*5; j+=5) { // 5 cycles of all colors on wheel
    for(i=0; i< leds.numPixels(); i++) {
      leds.setPixelColor(i, Wheel(((i * 256 / leds.numPixels()) + j) & 255));
    }
    leds.show();
//    delay(1);
  }

}

void setPixel(int x, int y, int z, int r, int g, int b)
{
    int index = (z*64) + (x*8) + y;
    leds.setPixelColor(index, leds.Color(r,g,b));
}

uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return leds.Color(WheelPos, 85 - WheelPos, 0);
  }
  else if(WheelPos < 170) {
   WheelPos -= 85;
   return leds.Color(85 - WheelPos, 0, WheelPos);
  } 
  else {
   WheelPos -= 170;
   return leds.Color(0, WheelPos, 85 - WheelPos);
  }
}

