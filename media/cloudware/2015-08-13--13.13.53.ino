
#include "beta-cube-library.h"

Cube cube = Cube();

void setup()
{
  cube.begin();

  
}

void loop()
{
  Cube cube = Cube(8, 50); // 8^3 cube with max brightness of 50
  cube.begin(); // initialize the cube


  cube.sphere(4, 4, 4, 3, Color(255, 50, 200)); // draw a red sphere in the center
  cube.show();
}