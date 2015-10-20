
#include "beta-cube-library.h"

Cube cube=Cube();

float x=0;
float xInc;
void setup() {
  cube.begin();
}

void loop() {
    cube.background(black);
  	
	cube.shell(Point(cube.center.x, cube.center.y, cube.center.z),5.2,Color(3,3,1));
	cube.shell(Point(cube.center.x, cube.center.y, cube.center.z),2,Color(50,50,0));
  	
    cube.show();
}



