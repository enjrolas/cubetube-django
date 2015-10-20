
#include "beta-cube-library.h"

Cube cube=Cube();
float x, xInc;

void setup() {

  	cube.begin();
	x=0;
	xInc=-.2;
}

void loop() {
    cube.background(black);
	cube.shell(Point(cube.center.x, cube.center.y, cube.center.z),5.2,Color(1,3,3));
	cube.shell(Point(cube.center.x+x, cube.center.y, cube.center.z),2,Color(50,50,0));
	x+=xInc;
  	if(x<-2)
      xInc=1;
 	if(x>2)
      xInx=-.2;
    cube.show();
}



