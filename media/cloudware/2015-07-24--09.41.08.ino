
#include "beta-cube-library.h"

Cube cube = Cube();

#define VOX_POINTS 64

int voxelXw1[VOX_POINTS];
int voxelZw1[VOX_POINTS];
int voxelXw2[VOX_POINTS];
int voxelZw2[VOX_POINTS];
int voxelXw3[VOX_POINTS];
int voxelZw3[VOX_POINTS];
int voxelXw4[VOX_POINTS];
int voxelZw4[VOX_POINTS];
int voxDelay(330);
int wave01(7);

Color brightLine01 = Color(199,118,221);
Color brightLine02 = Color(143,85,204);
Color brightLine03 = Color(101,62,178);
Color brightLine04 = Color(59,41,135);
Color brightLine05 = Color(18,10,99);
Color brightLine06 = Color(8, 15, 3);
Color brightLine07 = Color(8, 15, 3);


void setup() {
  cube.begin();
  for(int i=8;i>0;i--)
    {
      voxelXw1[i]=rand()%8;
      voxelZw1[i]=rand()%8;
      voxelXw2[i]=rand()%8;
      voxelZw2[i]=rand()%8;
      voxelXw3[i]=rand()%8;
      voxelZw3[i]=rand()%8;
      voxelXw4[i]=rand()%8;
      voxelZw4[i]=rand()%8;
    }
}

void loop() {


    for(int i=8;i>0;i--)
      {
        cube.setVoxel(voxelXw1[i], wave01+9, voxelZw1[i], black);
        cube.setVoxel(voxelXw1[i], wave01+8, voxelZw1[i], brightLine06);
        cube.setVoxel(voxelXw1[i], wave01+7, voxelZw1[i], brightLine05);
        cube.setVoxel(voxelXw1[i], wave01+6, voxelZw1[i], brightLine04);
        cube.setVoxel(voxelXw1[i], wave01+5, voxelZw1[i], brightLine04);
        cube.setVoxel(voxelXw1[i], wave01+4, voxelZw1[i], brightLine03);
        cube.setVoxel(voxelXw1[i], wave01+3, voxelZw1[i], brightLine03);
        cube.setVoxel(voxelXw1[i], wave01+2, voxelZw1[i], brightLine02);
        cube.setVoxel(voxelXw1[i], wave01+1, voxelZw1[i], brightLine02);
        cube.setVoxel(voxelXw1[i], wave01, voxelZw1[i], brightLine01);
      }


  cube.show();
  delay(voxDelay);

}