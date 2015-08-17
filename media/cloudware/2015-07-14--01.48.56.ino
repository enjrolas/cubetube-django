
#include "beta-cube-library.h"

char* vizName=" THIS DOESNT WORK :)";
int vizId=587;
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
    for ( int j = 0; j < 8; j ++ ) {
      for ( int k = 0; k < 8; k ++ ) {
    cube.setVoxel(i,j,k,red);
    cube.show();
    delay(100);
    cube.background(black);
    cube.show();
    delay(100);
      }
    }
  }
}