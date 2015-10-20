
#include "beta-cube-library.h"

#include "beta-cube-library.h"

Cube cube = Cube();

/**********************************
 * flip variables *
 * ********************************/
 //accelerometer pinout

int accelerometer[3];
void checkFlipState();
void updateAccelerometer();
int voxelX=rand()%8, voxelZ=rand()%8;
int voxelY=7;

void setup() {
  cube.begin();
}

void loop() {
  cube.background(black);

  //resets coordinates of voxel when it reaches the bottom                                                                                                                           
  if (voxelY<0) {
    voxelX=rand()%8;
    voxelZ=rand()%8;
    voxelY=7;
  }

  //downward motion                   
  cube.setVoxel(voxelX,voxelY,voxelZ,blue);                                                                                                                                               
  if (voxelY>=0) {voxelY--;}

  checkFlipState();
  cube.show();
}


/****************************************
 * flip functions *
 * **************************************/
 
 void checkFlipState()
 {
    updateAccelerometer();
    char output[50];
    sprintf(output, "%d %d %d %d", accelerometer[0],accelerometer[1],accelerometer[2],accelerometer[3] );
    Serial.println(output);
 }
 
void updateAccelerometer()
{
    for(int i=0;i<3;i++)
        accelerometer[i]=analogRead(X+i);
}