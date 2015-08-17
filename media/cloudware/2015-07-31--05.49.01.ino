
#include "beta-cube-library.h"

// This #include statement was automatically added by the Spark IDE.
#include "neopixel/neopixel.h"

#include <math.h>  //for the sqrt and pow() functions



int x, y, z;
unsigned char red, green, blue;

SYSTEM_MODE(SEMI_AUTOMATIC);  //don't connect to the internet on boot
#define BUTTON D2 //press this button to connect to the internet
#define MODE D3
bool onlinePressed=false;
bool lastOnline=true;

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D0
#define PIXEL_COUNT 512
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() {
    strip.begin();
    strip.show(); 
    initCloudButton();
}

//sets up the online/offline switch
void initCloudButton()
{
  //set the input mode for the 'connect to cloud' button
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(MODE, INPUT_PULLUP);
    if(!digitalRead(MODE))
        WiFi.listen();
    //a.k.a. onlinePressed is HIGH when the switch is set to 'online' and LOW when the switch is set to 'offline'
    onlinePressed=digitalRead(BUTTON);
    if(!onlinePressed)
        Spark.connect();
}

//checks to see if the 'online/offline' switch is switched
void checkCloudButton()
{
    //if the 'connect to cloud' button is pressed, try to connect to wifi.  
    //otherwise, run the program
    //note -- how does this behave when there are no wifi credentials loaded on the spark?

    //onlinePressed is HIGH when the switch is _not_ connected and LOW when the switch is connected
    //a.k.a. onlinePressed is HIGH when the switch is set to 'online' and LOW when the switch is set to 'offline'
    onlinePressed=digitalRead(BUTTON);
    
    if((!onlinePressed)&&(lastOnline))  //marked as 'offline'
    {
        lastOnline=onlinePressed;
        Spark.disconnect();
    }    

    else if((onlinePressed)&&(!lastOnline))  //marked as 'online'
    {
        lastOnline=onlinePressed;
        Spark.connect();
    }

    lastOnline=onlinePressed;
    
    if(!digitalRead(MODE))
        WiFi.listen();
}

void loop() {
    
  uint16_t i, j;

  for(j=0; j<256*5; j+=2) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
//    delay(1);
  }
  
  checkCloudButton();
  
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