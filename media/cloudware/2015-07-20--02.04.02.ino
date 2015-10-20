
#include "beta-cube-library.h"

char* vizName="   The Matrix";
int vizId=626;
Cube cube = Cube();

void setup() {
  cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {

  cube.setVoxel(0, 0, 0, new Color(1,1,1));

  cube.show();
  delay(100);

}
