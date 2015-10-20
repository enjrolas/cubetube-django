
#include "beta-cube-library.h"

#include "beta-cube-library.h"

Cube cube = Cube();

int accelerometer[3];
void checkFlipState();
void updateAccelerometer();
int bluedot.x=rand()%8, bluedot.z=rand()%8;
int bluedot.y=7;
point direction
point bluedot

void setup() {
  cube.begin();
}

void loop() {
  cube.background(black);
                                                                                                                       
  if (bluedot.y<0) {
    bluedot.x=rand()%8;
    bluedot.z=rand()%8;
    bluedot.y=7;
  }
       
  cube.setVoxel(bluedot.x,bluedot.y,bluedot.z,blue);     
  
  direction
  if (bluedot.y>=0) {bluedot.y--;}

  checkFlipState();
  cube.show();
  delay(20);
}

 void checkFlipState()
 {
    updateAccelerometer();
    char output[50];
    sprintf(output, "%d %d %d", accelerometer[0],accelerometer[1],accelerometer[2]);
    Serial.println(output);
 }
 
void updateAccelerometer()
{
    for(int i=0;i<3;i++)
        accelerometer[i]=analogRead(X+i);
}