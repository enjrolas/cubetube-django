
#include "beta-cube-library.h"

Cube cube = Cube();

int voxelXs[2] = {rand()%8, rand()%8};
int voxelZs[2] = {rand()%8, rand()%8};
int voxelYs[2] = {7, 5};

void setup() {
  cube.begin();
}

void loop() {
  cube.background(black);
  
  for (int x = 0; x < sizeof(voxelXs)/sizeof(int); x++) {
    if (voxelYs[x] >= 0) {
      cube.setVoxel(voxelXs[x], voxelYs[x], voxelZs[x], red); 
      voxelYs[x]--;
    }
    
    if (voxelYs[x] == 0) {
      voxelYs[x] = 8;
      voxelXs[x] = rand()%8;
      voxelZs[x] = rand()%8;
    }
  }
  
  cube.show();
}