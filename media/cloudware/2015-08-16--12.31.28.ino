
#include "beta-cube-library.h"

Cube cube = Cube();

void setup()
{
  cube.begin();
}

void loop()
{
  Color c = Color(50,44, 0);
  cube.background(c);
  cube.show();
  delay(1000);
}