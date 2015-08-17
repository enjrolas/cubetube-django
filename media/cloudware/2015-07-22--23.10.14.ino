
#include "beta-cube-library.h"

#include <math.h>
#include "beta-cube-library.h"

Cube cube = Cube();
void setup() {
    cube.begin();
}


void loop() {
    cube.background(black);
  	cube.setVoxel(rand()%7,rand()%7,rand()%7,color(255,255,255));
    cube.show();
}