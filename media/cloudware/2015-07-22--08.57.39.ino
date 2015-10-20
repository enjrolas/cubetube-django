
#include "beta-cube-library.h"

Cube cube = Cube();
void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.background(blue);
  cube.show();
}