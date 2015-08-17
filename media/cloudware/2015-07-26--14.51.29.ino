
#include "beta-cube-library.h"

#include <math.h>

Cube cube = Cube();
int maxSize = 8;
Color drawColor=Color(255,150,100);
int z;
int maxBrightness=100;
float phase=0;
float phaseInc=0.1;
float PI=3.1415;
int lastSwap = millis();

float distance(float x, float y, float z, float x1, float y1, float z1)
{
  return(sqrt(pow(x-x1,2)+pow(y-y1,2)+pow(z-z1,2)));
}

float map(float input, float inMin, float inMax, float outMin, float outMax)
{
    float out;
    out = (input-inMin)/(inMax-inMin)*(outMax-outMin) + outMin;
    return out;
}

float toFloat(int x)
{
  return float(x);
}

void setup()
{
  cube.begin();
}

void loop()
{
  cube.background(black);  //clear the cube for each frame
   
  phase += (millis()-lastSwap)/100000; 

  for(int x = 0; x < maxSize; x++)
  {
    for(int y = 0; y < maxSize; y++)
    {
        z = sin(phase + sqrt(pow(map(toFloat(x),toFloat(0),toFloat(maxSize-1),-PI,PI),2) + pow(map(toFloat(y),toFloat(0), toFloat(maxSize-1),-PI,PI),2)));		  
        int zm = round(map(toFloat(z),toFloat(-1),toFloat(1),toFloat(0),toFloat(maxSize-1)));
        cube.setVoxel(x,3+2*z, y, blue);
    }
}
  
  cube.show();  //send the data to the cube
  //delay(15); // keep the cube from disconnecting
}
