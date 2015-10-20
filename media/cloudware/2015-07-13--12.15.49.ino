
#include "beta-cube-library.h"

char* vizName="  Processing Listener";
int vizId=205;
Cube cube=Cube();

void setup()
{
  cube.begin();
  cube.show();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
  cube.listen();
}
