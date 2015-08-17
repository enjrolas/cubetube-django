
#include "beta-cube-library.h"

char* vizName="Too much random ";
int vizId=271;
Cube cube = Cube();

Color color = Color(random(256), random(256), random(256));

void setup()
{
  cube.begin();
  cube.background(black);
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void randomColor()
{
  color =  Color(random(256), random(256), random(256));
}

void loop()
{
  for(int j = 0; j < random(64); j++)
  cube.background(black);

  randomColor();
  for(int i = 0; i < random(64); i++)
  {
    cube.setVoxel(random(8),random(8),random(8),color);
  }

  cube.show();
delay (10);
  cube.background(black);
}