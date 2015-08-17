
#include "beta-cube-library.h"

Cube cube;
PVector[][][] voxels;

/********************************
 * zplasma variables *
 * *****************************/
float phase = 0.5;
float phaseIncrement = 0.1; // Controls the speed of the moving points. Higher == faster
float colorStretch = 0.8; // Higher numbers will produce tighter color bands 
//float phaseIncrement = 0.035; // Controls the speed of the moving points. Higher == faster
//float colorStretch = 0.23; // Higher numbers will produce tighter color bands 
float plasmaBrightness = 0.9;
Color plasmaColor;


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
      float h = (sin(dist / 1 + (float) millis() / -300) + 1) * 3;
      for (int j = 0; j < 8; j++) {
        float closeness = abs(j - h);
        float brightness = map(closeness, 0, 6, 255, 0);
        cube.setVoxel(voxels[i][j][k], color(brightness, 12, 512));
      }
    }
  }
  cube.draw();
}