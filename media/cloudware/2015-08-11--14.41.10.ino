
#include "beta-cube-library.h"

// This #include statement was automatically added by the Spark IDE.
#include "FastLED/FastLED.h"
FASTLED_USING_NAMESPACE;
 
// Derivative work to: http://pastebin.com/h95EgTkT
// Thanks again to Mark Kriegsman and FastLED
 
const uint8_t kCubeSize = 8;
const uint8_t kFracs = 16;
 
#define DATA_PIN D0
#define NUM_LEDS kCubeSize * kCubeSize * kCubeSize
#define MAX_POS (kCubeSize-1)*kFracs
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define MASTER_BRIGHTNESS   255
#define FADE 230
#define NUM_PIXELS 3
CRGB leds[NUM_LEDS];
 
typedef struct {
  int     F16posX = 0; // x position of the pixel
  int     F16posY = 0; // y position of the pixel
  int     F16posZ = 0; // z position of the pixel
  uint8_t hue = 0;
  uint8_t xPosMod = 1;
  uint8_t yPosMod = 1;
  uint8_t zPosMod = 1;
} pixel;
 
pixel pixels[NUM_PIXELS];
 
void initPixelsTaz()
{
  // NUM_PIXELS should be 3 to use only these two values
  pixels[0].hue = beatsin8(19,HUE_RED,HUE_YELLOW);
  pixels[0].xPosMod = 143;
  pixels[0].yPosMod = 91;
  pixels[0].zPosMod = 74;
 
  pixels[1].hue = beatsin8(3,HUE_AQUA,HUE_PURPLE);
  pixels[1].xPosMod = 21;
  pixels[1].yPosMod = 17;
  pixels[1].zPosMod = 15;
  
  pixels[2].hue = beatsin8(3,HUE_AQUA,HUE_ORANGE);
  pixels[2].xPosMod = 0; 
  pixels[2].yPosMod = 0; 
  pixels[2].zPosMod = 15; 
}
 
void initPixelsRandom()
{
  for (int i=0; i<NUM_PIXELS; i++)
  {
    pixels[i].hue = random8();
    pixels[i].xPosMod = random8();
    pixels[i].yPosMod = random8();
    pixels[i].zPosMod = random8();
  }
}
 
void movePixels()
{
  for (int i=0; i<NUM_PIXELS; i++)
  {
    pixels[i].F16posX = beatsin16(pixels[i].xPosMod, 0, MAX_POS);
    pixels[i].F16posY = beatsin16(pixels[i].yPosMod, 0, MAX_POS);
    pixels[i].F16posZ = beatsin16(pixels[i].zPosMod, 0, MAX_POS);
    pixels[i].hue++;
  }
}
 
void setup() {
  delay(3000); // setup guard
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(MASTER_BRIGHTNESS);
  initPixelsTaz();
  //initPixelsRandom();
}
 
uint16_t XYZ( uint8_t x, uint8_t y, uint8_t z)
{
  uint16_t i;
 
  i = kCubeSize * ((y * kCubeSize) + x) + z;
 
  if ((i >= 0) && (i <= 512))
    return i;
  else
    return 0;
}
 
// Draw a pixel on a matrix using fractions of light. Positions are measured in
// sixteenths of a pixel.  Fractional positions are
// rendered using 'anti-aliasing' of pixel brightness.
void drawPixels()
{
  for (int i = 0; i < NUM_PIXELS; i++)
  {
    int hue = pixels[i].hue;
   
    uint8_t x = pixels[i].F16posX / kFracs; // convert from pos to raw pixel number
    uint8_t y = pixels[i].F16posY / kFracs; // convert from pos to raw pixel number
    uint8_t z = pixels[i].F16posZ / kFracs; // convert from pos to raw pixel number
    uint8_t fracX = pixels[i].F16posX & 0x0F; // extract the 'factional' part of the position
    uint8_t fracY = pixels[i].F16posY & 0x0F; // extract the 'factional' part of the position
    uint8_t fracZ = pixels[i].F16posZ & 0x0F; // extract the 'factional' part of the position
 
    uint8_t px = 255 - (fracX * kFracs);
    uint8_t py = 255 - (fracY * kFracs);
    uint8_t pz = 255 - (fracZ * kFracs);
   
    leds[XYZ(x,y,z)] += CHSV( hue, 255, scale8(px, scale8(py, pz)));
    leds[XYZ(x,y+1,z)] += CHSV( hue, 255, scale8(px, scale8((255-py), pz)));
    leds[XYZ(x,y,z+1)] += CHSV( hue, 255, scale8(px, scale8(py, (255-pz))));
    leds[XYZ(x,y+1,z+1)] += CHSV( hue, 255, scale8(px, scale8((255-py), (255-pz))));
    leds[XYZ(x+1,y,z)] += CHSV( hue, 255, scale8((255-px), scale8(py, pz)));
    leds[XYZ(x+1,y+1,z)] += CHSV( hue, 255, scale8((255-px), scale8((255-py), pz)));
    leds[XYZ(x+1,y,z+1)] += CHSV( hue, 255, scale8((255-px), scale8(py, (255-pz))));
    leds[XYZ(x+1,y+1,z+1)] += CHSV( hue, 255, scale8((255-px), scale8((255-py), (255-pz))));
  }
}
 
void loop()
{
  // Dim everything
  nscale8(leds, NUM_LEDS, FADE);
 
  movePixels();
  drawPixels();
   
  FastLED.show();
  delay(20);
}