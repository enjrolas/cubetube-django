
#include "beta-cube-library.h"

char* vizName="Blink";
int vizId=581;
Cube cube = Cube();
int frame=0;

void setup()
{
  cube.begin();
  cube.background(red);
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
  cube.background(red);
  if(frame%20>10)
	  cube.setVoxel(3,3,3,blue);
  cube.show();
  frame++;
}