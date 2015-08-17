
#include "beta-cube-library.h"

char* vizName="Tranquililty";
int vizId=203;
Cube cube=Cube();

void setup()
{
  cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
  cube.background(magenta);
}