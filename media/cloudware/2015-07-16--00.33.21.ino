
#include "beta-cube-library.h"

char* vizName=" THIS DOESNT WORK :)";
int vizId=322;
Cube cube = Cube();

void setup()
{
  cube.begin();
  cube.background(black);
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
  for ( int i = 0; i < 8; i ++ ) {
    cube.setVoxel(i,3,3,red);
    cube.show();
    delay(500);
    cube.background(black);
    cube.show();
    delay(500);
  }
}