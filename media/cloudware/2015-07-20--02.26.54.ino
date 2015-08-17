
#include "beta-cube-library.h"

char* vizName=" RGB";
int vizId=626;
Cube cube = Cube();

int brightness = 0.5;

void setup() {
  cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {

  for (int x = 0; x < 8; ++x) {
    	for (int y = 0; y < 8; ++y) {
          	for (int z = 0; z < 8; ++z) {
              int r = (x / 8.0) * (100.0 * brightness);
              int g = (y / 8.0) * (100.0 * brightness);
              int b = (z / 8.0) * (100.0 * brightness);
              Color col = Color(r,g,b);
              cube.setVoxel(x, y, z, col);
            }
        }
  }

  cube.show();
  delay(100);

}
