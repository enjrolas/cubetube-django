
#include "beta-cube-library.h"

#include <math.h>

Cube cube = Cube();
int centerX, centerY, centerZ;
int launchX, launchZ;
Color fadedBlue=Color(0,0,10);
Color darkBlue=Color(0,0,1);
int r,g,b;
int maxBrightness=100;
float radius=0;
float speed = .5;
float xInc, yInc, zInc;
float rocketX, rocketY, rocketZ;
float launchTime;
int maxSize;
float drip;
float dripLocX;
float dripLocY;
bool firstTime;

float distance(float x, float y, float z, float x1, float y1, float z1)
{
  return(sqrt(pow(x-x1,2)+pow(y-y1,2)+pow(z-z1,2)));
}


void setup()
{
  dripLocX=rand()%8;
  dripLocY=rand()%8;
  drip = 7;
  firstTime = true;
  cube.begin();
}

void loop()
{
  cube.background(black);  //clear the cube for each frame
  //cube.shell(4, 4, 4, 3, red);
  //cube.line(2, 2, 2, 4, 4, 4, blue);
  for (int x=0; x<8; x++) {
    for (int z=0; z<8; z++) {
    	cube.setVoxel(x,0,z,fadedBlue);
    }
  }
  cube.setVoxel(dripLocX,drip,dripLocY,fadedBlue);
  cube.setVoxel(dripLocX,drip+1,dripLocY,darkBlue);
  cube.setVoxel(dripLocX,drip+2,dripLocY,darkBlue);
  if (drip>0) {
    	drip = drip - speed;
  } else {
    	drip = 7; 
    	dripLocX=rand()%8;
    	dripLocY=rand()%8;
  }
  cube.show();  //send the data to the cube
}




