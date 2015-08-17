
#include "beta-cube-library.h"

/*
Based on illuminatingly clear code, by John Ericksen c2013: https://github.com/johncarl81/neopixelplasma/blob/master/neopixelplasma.ino
Modified into 3D by Shawn Frayne for a LyteLab cube by Looking Glass, www.lytelab.com
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
 
You should have received a copy of the GNU General Public License
along with this program. If not, see http://www.gnu.org/licenses/.
*/
 
#include <math.h>  
//#include "Adafruit_mfGFX/Adafruit_mfGFX.h"   // Core graphics library
//#include "RGBmatrixPanel/RGBmatrixPanel.h" // Hardware-specific library
#include "application.h"
#include "neopixel/neopixel.h"
 
 
// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D0
#define PIXEL_COUNT 512
#define PIXEL_TYPE WS2812B
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
 
struct Point {
float x;
float y;
float z;
};
 
float phase = 0.0;
float phaseIncrement = 0.035; // Controls the speed of the moving points. Higher == faster
float colorStretch = 0.23; // Higher numbers will produce tighter color bands
float brightness = 0.3;
 
SYSTEM_MODE(SEMI_AUTOMATIC);  //don't connect to the internet on boot
#define BUTTON D2 //press this button to connect to the internet
#define MODE D3
bool onlinePressed=false;
bool lastOnline=true;
 
void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
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
    if(onlinePressed)
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
   
    if((!onlinePressed)&&(lastOnline))  //marked as 'online'
    {
        lastOnline=onlinePressed;
        Spark.connect();
    }    
 
    else if((onlinePressed)&&(!lastOnline))  //marked as 'offline'
    {
        lastOnline=onlinePressed;
        Spark.disconnect();
    }
 
    lastOnline=onlinePressed;
   
    if(!digitalRead(MODE))
        WiFi.listen();
}
void loop()
{    
    checkCloudButton();
        zplasma(); //this is the main routine being run
 
    }
   
 
 
 
void setPixel(int x, int y, int z, int r, int g, int b) {  //alex's helper function that lets us and the cube think in regular xyz coordinates and RGB colors
    int index = (z*64) + (x*8) + y;
    strip.setPixelColor(index,strip.Color(r,g,b));
}
 
 
 
 
 
void zplasma(){
 
phase += phaseIncrement;
// The two points move along Lissajious curves, see: http://en.wikipedia.org/wiki/Lissajous_curve
// We want values that fit the LED grid: x values between 0..8, y values between 0..8, z values between 0...8
// The sin() function returns values in the range of -1.0..1.0, so scale these to our desired ranges.
// The phase value is multiplied by various constants; I chose these semi-randomly, to produce a nice motion.
Point p1 = { (sin(phase*1.000)+1.0) * 4, (sin(phase*1.310)+1.0) * 4.0,  (sin(phase*1.380)+1.0) * 4.0};
Point p2 = { (sin(phase*1.770)+1.0) * 4, (sin(phase*2.865)+1.0) * 4.0,  (sin(phase*1.410)+1.0) * 4.0};
Point p3 = { (sin(phase*0.250)+1.0) * 4, (sin(phase*0.750)+1.0) * 4.0,  (sin(phase*0.380)+1.0) * 4.0};
 
byte row, col, dep;
 
// For each row
for(row=0; row<8; row++) {
float row_f = float(row); // Optimization: Keep a floating point value of the row number, instead of recasting it repeatedly.
 
// For each column
for(col=0; col<8; col++) {
float col_f = float(col); // Optimization.
 
// For each depth
for(dep=0; dep<8;dep++) {
float dep_f = float(dep); // Optimization.
 
// Calculate the distance between this LED, and p1.
Point dist1 = { col_f - p1.x, row_f - p1.y,  dep_f - p1.z }; // The vector from p1 to this LED.
float distance1 = sqrt( dist1.x*dist1.x + dist1.y*dist1.y + dist1.z*dist1.z);
 
// Calculate the distance between this LED, and p2.
Point dist2 = { col_f - p2.x, row_f - p2.y,  dep_f - p2.z}; // The vector from p2 to this LED.
float distance2 = sqrt( dist2.x*dist2.x + dist2.y*dist2.y + dist2.z*dist2.z);
 
// Calculate the distance between this LED, and p3.
Point dist3 = { col_f - p3.x, row_f - p3.y,  dep_f - p3.z}; // The vector from p3 to this LED.
float distance3 = sqrt( dist3.x*dist3.x + dist3.y*dist3.y + dist3.z*dist3.z);
 
// Warp the distance with a sin() function. As the distance value increases, the LEDs will get light,dark,light,dark,etc...
// You can use a cos() for slightly different shading, or experiment with other functions.
float color_1 = distance1; // range: 0.0...1.0
float color_2 = distance2;
float color_3 = distance3;
float color_4 = (sin( distance1 * distance2 * colorStretch )) + 2.0 * 0.5;
// Square the color_f value to weight it towards 0. The image will be darker and have higher contrast.
color_1 *= color_1 * color_4;
color_2 *= color_2 * color_4;
color_3 *= color_3 * color_4;
color_4 *= color_4;
// Scale the color up to 0..7 . Max brightness is 7.
//strip.setPixelColor(col + (8 * row), strip.Color(color_4, 0, 0) );
setPixel(row,col,dep,color_1*brightness, color_2*brightness, color_3*brightness);      
}
}
}
strip.show();  // display the new pixels in the cube
}