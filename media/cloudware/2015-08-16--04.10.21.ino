
#include "beta-cube-library.h"

Cube cube = Cube();

void Square(Color c)
{
  for(int x = 0; x < 7; x++)
    for(int y = 0; y < 7; y++)
        cube.setVoxel(x, y, 6, c);
}

void setup()
{
  cube.begin();
}

void loop()
{
  Color c = Color(255, 216, 0);
  cube.background(black);
  Square(c);
  cube.show();
  delay(1000);
}

