
#include "beta-cube-library.h"

Cube cube=Cube();

void setup() {

  cube.begin();
}

void loop() {
    cube.background(black);

	cube.shell(cube.center, 5, Color(50,50,30));
  
    cube.show();
}



