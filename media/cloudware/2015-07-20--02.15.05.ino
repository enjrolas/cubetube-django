
#include "beta-cube-library.h"

char* vizName=" RGB";
int vizId=626;
Cube cube = Cube();

void setup() {
  cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {

  for (int x = 0; x < 8; ++x) {
    	for (int y = 0; y < 8; ++y) {
          	for (int z = 0; z < 8; ++z) {
              	Color col = Color(x / 8 * 100, 1, 1);
              	cube.setVoxel(x, y, z, col);
            }
        }
  }

  cube.show();
  delay(100);

}
