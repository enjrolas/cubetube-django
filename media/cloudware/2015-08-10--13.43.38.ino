
#include "beta-cube-library.h"

Cube cube = Cube();

Color color = Color(random(256), random(2), random(2));

void setup()
{
  cube.begin();
  cube.background(black);
}

void randomColor()
{
  color =  Color(random(256), random(2), random(2));
}

void loop()
{
  cube.background(black);

  randomColor();
  for(int i = 0; i < random(64); i++)
  {
    cube.setVoxel(random(8),random(8),random(8),color);
  }

  cube.show();

}