
#include "beta-cube-library.h"

char* vizName="misty";
int vizId=253;
Cube cube;
color col;
PVector[][][] voxels;
PVector sphereCenter = new PVector(3.5, 3.5, 3.5);
float sphereRadius = 3;

void setup()
{
  cube=new Cube(this);
  voxels = new PVector[8][8][8];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 8; k++) {
        voxels[i][j][k] = new PVector(i, j, k);
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
      }
    }
  }
}

void draw()
{
  sphereCenter.x = sin((float) millis() / 1000 / 2) * 3 + 3.5;
  sphereCenter.y = cos((float) millis() / 1000 / 3) * 3 + 3.5;
  sphereCenter.z = cos((float) millis() / 1000 / 4) * 3 + 3.5;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 8; k++) {
        float myDist = voxels[i][j][k].dist(sphereCenter);
        float redness = map(myDist, sphereRadius+.5, sphereRadius, 0, 255);
        if (redness > 255) {
          redness = 255;
        }
        if (redness < 0) {
          redness = 0;
        }
        cube.setVoxel(voxels[i][j][k], color(redness, 0, 0));
      }
    }
  }
  cube.draw();
}