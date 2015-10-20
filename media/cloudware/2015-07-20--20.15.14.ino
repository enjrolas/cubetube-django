
#include "beta-cube-library.h"

char* vizName="   Mood";
int vizId=665;
Cube cube;

void setup() {
  cube.begin();
  Color col(1,1,1);
  cube.background(col);
  cube.show();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {
}
