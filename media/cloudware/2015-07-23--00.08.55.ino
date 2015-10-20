
#include "beta-cube-library.h"

Cube cube;
PVector[][][] voxels;

void setup()
{
  cube=new Cube(this);
  voxels = new PVector[8][8][8];

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 8; k++) {
        voxels[i][j][k] = new PVector(i, j, k);
      }
    }
  }
}

void draw()
{
  for (int i = 0; i < 8; i++) {
    for (int k = 0; k < 8; k++) {
      PVector birdsEyeCoord = new PVector(i, k);
      /* translate the coord to recenter the wave */
      birdsEyeCoord.add(-3.5, -8, 0);
      float dist = birdsEyeCoord.mag();
      float h = (sin(dist / 2 + (float) millis() / -300) + 1) * 2;
      for (int j = 0; j < 8; j++) {
        float closeness = abs(j - h);
        float brightness = map(closeness, 0, 1.5, 255, 0);
        cube.setVoxel(voxels[i][j][k], color(0, 0, brightness));
      }
    }
  }
  cube.draw();
}