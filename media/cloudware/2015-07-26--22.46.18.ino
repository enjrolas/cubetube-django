
#include "beta-cube-library.h"

Cube cube = Cube();

int[] voxelXs = {rand()%8, rand()%8};
int[] voxelZs = {rand()%8, rand()%8};
int[] voxelYs = {7, 5};

void setup() {
  cube.begin();
}

void loop() {
  cube.background(black);
  
  for (int x = 0; x < voxelXs
  
  //downward motion
  if (voxelYs[0]>=0) {
   	cube.setVoxel(voxelXs,voxelYs[0],voxelZ,red);
    voxelYs[0]--;
  }
  
   if (voxelYs[1]>=0) {
   	cube.setVoxel(voxelX,voxelYs[1],voxelZ,red);
    voxelYs[0]--;
  } 
  
  
  //resets coordinates of voxel when it reaches the bottom
  if (voxelY==0) {
    voxelX=rand()%8, voxelZ=rand()%8;
    voxelY=7;
  } 
  
  cube.show();
}