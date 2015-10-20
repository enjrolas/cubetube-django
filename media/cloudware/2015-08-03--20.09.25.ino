
#include "beta-cube-library.h"

Cube cube = Cube();

#define VOX_POINTS 16

int voxelX[VOX_POINTS];
int voxelZ[VOX_POINTS];
int voxDelay(150);
int wave01=7;
int wave02=7;
int wave03=7;
int wave04=7;
int wave05=7;
Color brightLine01 = Color(244, 241, 250);
Color brightLine02 = Color(98, 193, 97);
Color brightLine03 = Color(30, 131, 30);
Color brightLine04 = Color(5, 45, 6);
Color brightLine05 = Color(6, 25, 3);
Color brightLine06 = Color(8, 15, 3);

void setup() {
  cube.begin();
}

void loop() {

  if (wave01=-10) {
    wave01=7;
    for(int i=16;i>8;i--)
    {
      voxelX[i]=rand()%8;
      voxelZ[i]=rand()%8;
    }
  } 

  if (wave02=-10) {
    wave02=9;
    for(int i=8;i>0;i--)
    {
      voxelX[i]=rand()%8;
      voxelZ[i]=rand()%8;
    }
  } 
  
  if (wave01>-10) {
      for(int i=16;i>8;i--)
        {
          cube.setVoxel(voxelX[i], wave01+9, voxelZ[i], black);
          cube.setVoxel(voxelX[i], wave01+8, voxelZ[i], brightLine06);
          cube.setVoxel(voxelX[i], wave01+7, voxelZ[i], brightLine05);
          cube.setVoxel(voxelX[i], wave01+6, voxelZ[i], brightLine04);
          cube.setVoxel(voxelX[i], wave01+5, voxelZ[i], brightLine04);
          cube.setVoxel(voxelX[i], wave01+4, voxelZ[i], brightLine03);
          cube.setVoxel(voxelX[i], wave01+3, voxelZ[i], brightLine03);
          cube.setVoxel(voxelX[i], wave01+2, voxelZ[i], brightLine02);
          cube.setVoxel(voxelX[i], wave01+1, voxelZ[i], brightLine02);
          cube.setVoxel(voxelX[i], wave01, voxelZ[i], brightLine01);
        }
      cube.show();
      wave01--;
  } 

  if (wave02>-10) {
      for(int i=8;i>0;i--)
        {
          cube.setVoxel(voxelX[i], wave02+9, voxelZ[i], black);
          cube.setVoxel(voxelX[i], wave02+8, voxelZ[i], brightLine06);
          cube.setVoxel(voxelX[i], wave02+7, voxelZ[i], brightLine05);
          cube.setVoxel(voxelX[i], wave02+6, voxelZ[i], brightLine04);
          cube.setVoxel(voxelX[i], wave02+5, voxelZ[i], brightLine04);
          cube.setVoxel(voxelX[i], wave02+4, voxelZ[i], brightLine03);
          cube.setVoxel(voxelX[i], wave02+3, voxelZ[i], brightLine03);
          cube.setVoxel(voxelX[i], wave02+2, voxelZ[i], brightLine02);
          cube.setVoxel(voxelX[i], wave02+1, voxelZ[i], brightLine02);
          cube.setVoxel(voxelX[i], wave02, voxelZ[i], brightLine01);
        }
      cube.show();
      wave02--;
  } 

  delay(voxDelay);
      
}