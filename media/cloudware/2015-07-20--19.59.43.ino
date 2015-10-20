
#include "beta-cube-library.h"

char* vizName="Mood";
int vizId=665;
Cube cube;

void setup() {
  Color col(1,1,1);
  cube.background(col);
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {
}
