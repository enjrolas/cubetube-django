
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
              	Color col = Color((x / 8.0) * (100.0 * brightness), (y / 8.0) * (100.0 * brightness), (z / 8.0) * (100.0 * brightness));
              	cube.setVoxel(x, y, z, col);
            }
        }
  }

  cube.show();
  delay(100);

}
