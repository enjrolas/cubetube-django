
#include "beta-cube-library.h"

Cube cube=Cube();
Color col=Color(70,0,155);  //this is a yellow color, but I deliberately keep it at ~20% brightness
						   //at full brightness, i.e. (255,255,0), the LEDs will try to draw too much power, and the
						   //cube's internal current-limiting circuitry will kick in.  
void setup()
{
  cube.begin();
}

void loop()
{
  cube.background(black);   //start with a cleared cube
  for(int x=0;x<cube.size;x++)
    	for(int y=0;y<cube.size;y++)
          	for(int z=0;z<cube.size;z++)
            {
              	cube.setVoxel(x,y,z,col);
              	cube.show();  //display the cube with this current voxel (and all the voxels before it) colored yellow
              	delay(10);   //wait 0.01 seconds before advancing to the next voxel
            }
  
}