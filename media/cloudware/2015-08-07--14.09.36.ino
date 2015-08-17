
#include "beta-cube-library.h"

Cube cube=Cube();
float x, xInc;

void setup() {

  	cube.begin();
  	while(Time.second()!=0){}
	x=0;
	xInc=-.05;
}

void loop() {
    cube.background(black);
	cube.shell(Point(cube.center.x, cube.center.y, cube.center.z),5.2,Color(3,3,1));
	cube.shell(Point(cube.center.x+x, cube.center.y, cube.center.z),2,Color(0,50,50));
	x+=xInc;
  	if(x<-2)
      xInc=2;
 	if(x>2)
      xInc=-.05;
    cube.show();
}



