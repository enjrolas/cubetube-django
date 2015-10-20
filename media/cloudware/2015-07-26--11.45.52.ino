
#include "beta-cube-library.h"

#include <math.h>

Cube cube = Cube();
int maxSize = 8;
Color drawColor=Color(255,150,100);
int Z;
int maxBrightness=100;
float phase=0;
float phaseInc=0.1;

float distance(float x, float y, float z, float x1, float y1, float z1)
{
  return(sqrt(pow(x-x1,2)+pow(y-y1,2)+pow(z-z1,2)));
}

float map(float input, double inMin, double inMax, double outMin, double outMax){
    float out;
    out = (input-inMin)/(inMax-inMin)*(outMax-outMin) + outMin;
    return out;
}


void setup()
{
  cube.begin();
}

void loop()
{
  cube.background(black);  //clear the cube for each frame
    

  for(int x = 0; x < maxSize; x++){
    for(int y = 0; y < maxSize; y++){
        Z = sin(phase + sqrt(pow(map(x,0,maxSize-1,-PI,PI),2) + pow(map(y,0,maxSize-1,-PI,PI),2)));
        Z = round(map(Z,-1,1,0,maxSize-1));
        cube.setVoxel(x,y,Z, drawColor);
    }
}
  phase += phaseInc; 
  cube.show();  //send the data to the cube
  //delay(15); // keep the cube from disconnecting
}
