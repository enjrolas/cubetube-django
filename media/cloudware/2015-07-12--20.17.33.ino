
#include "beta-cube-library.h"

char* vizName="     Rain";
int vizId=325;
Cube cube = Cube();

int voxelX=rand()%8, voxelZ=rand()%8;
int voxelY=7;

void setup() {
  cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {
  cube.background(black);
  
  //downward motion
  if (voxelY>=0) {
   	cube.setVoxel(voxelX,voxelY,voxelZ,blue);
    voxelY--;
  } 
  
  //resets coordinates of voxel when it reaches the bottom
  if (voxelY==0) {
    voxelX=rand()%8, voxelZ=rand()%8;
    voxelY=7;
  } 
  
  cube.show();
}