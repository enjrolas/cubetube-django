
#include "beta-cube-library.h"

#include "beta-cube-library.h"

Cube cube = Cube();

/**********************************
 * flip variables *
 * ********************************/
 //accelerometer pinout
#define X 13
#define Y 14
#define Z 15
#define AUTOCYCLE_TIME 22222
#define FACEPLANT 2300
#define UPSIDE_DOWN 1850
#define RIGHTSIDE_UP 2400
#define LEFT_SIDE 1800
#define RIGHT_SIDE 2400
#define FLIP_TIMEOUT 3000
#define FLIP_DEBOUNCE 250
int maxBrightness=50;
long lastFaceplant=-1*FLIP_TIMEOUT;
bool upsideDown=false;
bool sideways=false;
bool autoCycle=true;    //start on autocycle by default
int upsideDownTime=-1*FLIP_TIMEOUT;
long lastAutoCycle=0;
int lastLeft=-1*FLIP_TIMEOUT;
int lastRight=-1*FLIP_TIMEOUT;
long lastChange=0;


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
    char buf[12];
    sprintf(buf, "%d %d %d %d", accelerometer[0],accelerometer[1],accelerometer[2],accelerometer[3]); // puts string into buffer

    Serial.println(buf);
 }
 
void updateAccelerometer()
{
    for(int i=0;i<3;i++)
        accelerometer[i]=analogRead(X+i);
}