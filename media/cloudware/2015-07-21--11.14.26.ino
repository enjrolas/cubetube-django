
#include "beta-cube-library.h"

char* vizName=" TestStooge";
int vizId=97;
Cube cube = Cube();

void setup()
{
  cube.background(blue);
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void draw()
{
  cube.background(blue);
}
