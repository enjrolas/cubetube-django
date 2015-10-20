
#include "beta-cube-library.h"

Cube cube = Cube();
int frame = 0;
int c = 0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.background(black);

  for(int i=0; i<8; i++)
  	for(int j=0; j<8; j++)
  	  for(int k=0; k<8; k++)
 		cube.setVoxel(i, j, k, cube.colorMap(i*10, j*10, k*10));
  
  cube.show();
  
  frame++;
  if(frame % 10 == 0)
    c++;
}