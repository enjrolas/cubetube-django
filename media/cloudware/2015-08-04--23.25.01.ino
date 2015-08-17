
#include "beta-cube-library.h"

Cube cube=Cube();

void setup() {

  cube.begin();
}

void loop() {
    cube.background(black);

	cube.shell(cube.center, 5.3, Color(3,3,1));
  	cube.shell(Point(cube.center.x+1, center.y, cube.center.z+2),2,Color(0,30,30));
  
    cube.show();
}



