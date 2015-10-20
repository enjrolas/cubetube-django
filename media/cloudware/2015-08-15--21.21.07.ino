
#include "beta-cube-library.h"

Cube cube = Cube();

void setup()
{
  cube.begin();

  
}

void loop()
{
  cube.begin(); // initialize the cube


  cube.sphere(4, 4, 4, 3, Color(100, 10, 200)); // draw a red sphere in the center
  cube.show();
}