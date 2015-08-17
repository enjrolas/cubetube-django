
#include "beta-cube-library.h"

char* vizName="RGB";
int vizId=626;
Cube cube = Cube();

void setup() {
  cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {

  Color col = Color(1,1,1);
  cube.setVoxel(0, 0, 0, col);

  cube.show();
  delay(100);

}
