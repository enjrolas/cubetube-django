
#include "beta-cube-library.h"

#include <math.h>

Cube cube = Cube();
int frame;
void setup() {
    cube.begin();
}


void loop() {
    cube.background(black);
  	cube.setVoxel(rand()%7,rand()%7,rand()%7,color(255,255,255));
    cube.show();
	frame++;
}