
#include "beta-cube-library.h"

#include <math.h>
#include "beta-cube-library.h"
#include "neopixel.h"
/**   A point in 3D space.  */



Point foo [5];// = { 3, 2, 1, 4, 5 };
Cube cube = Cube();
void setup() {
    cube.begin();
  foo[0] = {1,2,3};
  foo[1] = {2,2,3};
  foo[2] = {3,2,3};
  foo[3] = {4,2,3};
  foo[4] = {5,2,3};
  foo[5] = {6,2,3};
}


void loop() {
    cube.background(Color(0,0,0));
  //	cube.setVoxel(rand()%7,rand()%7,rand()%7,blue);
  	for(int i=0;i<=7;i++){
	 	cube.setVoxel(foo[i],blue);
	  	//cube.setVoxel(foo[i],i,i,color(255,255,255));
    }
    cube.show();
}