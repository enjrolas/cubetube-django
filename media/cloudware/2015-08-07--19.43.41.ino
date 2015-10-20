
#include "beta-cube-library.h"

Cube cube = Cube();
int frame=0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void letI()
{
	cube.setVoxel(3,0,7,blue);
    cube.setVoxel(4,0,7,blue);
    cube.setVoxel(5,0,7,blue);
    cube.setVoxel(6,0,7,blue);

  	cube.setVoxel(4,1,7,blue);
    cube.setVoxel(5,1,7,blue);

  	cube.setVoxel(4,2,7,blue);
    cube.setVoxel(5,2,7,blue);

  	cube.setVoxel(4,3,7,blue);
    cube.setVoxel(5,3,7,blue);

  	cube.setVoxel(4,4,7,blue);
    cube.setVoxel(5,4,7,blue);

  	cube.setVoxel(4,5,7,blue);
    cube.setVoxel(5,5,7,blue);

  	cube.setVoxel(4,6,7,blue);
    cube.setVoxel(5,6,7,blue);
  
  	cube.setVoxel(3,7,7,blue);
    cube.setVoxel(4,7,7,blue);
    cube.setVoxel(5,7,7,blue);
    cube.setVoxel(6,7,7,blue);
}


/*

________
_XXXXXX_
___XX___
___XX___
___XX___
___XX___
___XX___
___XX___
_XXXXXX_

*/

void loop()
{
  cube.background(black);
  if (frame%20>10) {
    	letI();
        //cube.setVoxel(2,0,7,blue);
  }
    
  cube.show();
  frame++;
}