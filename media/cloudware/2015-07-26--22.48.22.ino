
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
  
  for (int x = 0; x < sizeof(voxelXs); x++) {
    if (voxelYs[x] >= x) {
      cube.setVoxel(voxelXs[x], voxelYs[x], voxelZs[x], red); 
      voxelYs[x]--;
    }
    
    if (voxelYs[x] == 0) {
      voxelXs[x] = rand()%8;
      voxelZs[x] = rand()%8; 
    }
  }
  
  cube.show();
}