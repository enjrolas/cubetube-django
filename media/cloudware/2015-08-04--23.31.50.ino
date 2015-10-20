
#include "beta-cube-library.h"

Cube cube=Cube();

float x=0;
float xInc=0.1;
void setup() {

  cube.begin();
}

void loop() {
    cube.background(black);

	cube.shell(cube.center, 5.3, Color(3,3,1));
  	cube.shell(Point(cube.center.x+x, cube.center.y, cube.center.z+1),2,Color(0,30,30));
  	x+=xInc;
  	if((x>2)||(x<-2))
      xInc*=-1;
    cube.show();
}



