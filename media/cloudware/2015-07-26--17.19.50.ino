
#include "beta-cube-library.h"

Cube cube = Cube();
int frame=0;

void setup()
{
  
  cube=new Cube(this);
  cube.background(black);
  voxels = new PVector[8][8][8];
    for (float z=0; z<8; z++){
     for (float y=0; y<8; y++){
      for (float x=0; x<8; x++){
        voxels[x][y][z]=new PVector(x, y, z);
      }}}
}

void loop()
{        
  cube.show();
  cube.background(black);
  for (float z=0; z<8; z++){
    for (float y=0; y<8; y++){
      for (float x=0; x<8; x++){


        cube.setVoxel(voxels[x][y][z],blue);
        cube.draw;

      }}}
}