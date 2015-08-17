
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
    /*
    if(accelerometer[2]<UPSIDE_DOWN)  //if the cube is upside-down, set the upside-down flag and mark the time when it was flipped
    {
        upsideDownTime=millis();
       // Serial.println("I'm upside-down!");
    }
    */
    if(accelerometer[0]>FACEPLANT)  //if the cube is upside-down, set the upside-down flag and mark the time when it was flipped
    {
        lastFaceplant=millis();
       Serial.println("I'm upside-down!");
    }
    if(accelerometer[1]<LEFT_SIDE)  //if the cube is flipped to either side
    {
        lastLeft=millis();
       Serial.println("I'm on my left side");
    }
    if(accelerometer[1]>RIGHT_SIDE)
    {
        lastRight=millis();
       Serial.println("I'm on my right side");
    }

    if(accelerometer[2]>RIGHTSIDE_UP)
    {
        if(((millis()-lastFaceplant)<FLIP_TIMEOUT)&&(millis()-lastFaceplant>FLIP_DEBOUNCE))
        {
            autoCycle=false;
            lastFaceplant=millis()-FLIP_TIMEOUT;
            Color flash;
            flash.red=maxBrightness;
            flash.green=maxBrightness;
            flash.blue=maxBrightness;
            cube.background(flash);
        }
        if(((millis()-lastLeft)<FLIP_TIMEOUT)&&(millis()-lastChange>FLIP_DEBOUNCE))
        {
    //        Serial.println("turned to the left and back");
            autoCycle=false;
            lastChange=millis();
            decrementDemo();
            lastLeft=millis()-FLIP_TIMEOUT;
        }
        if(((millis()-lastRight)<FLIP_TIMEOUT)&&(millis()-lastChange>FLIP_DEBOUNCE))
        {
       //     Serial.println("turned to the right and back");
            autoCycle=false;
            lastChange=millis();
            incrementDemo();
            lastRight=millis()-FLIP_TIMEOUT;
        }
    }
    
    if(autoCycle)
        if(millis()-lastAutoCycle>AUTOCYCLE_TIME)   //in autocycle, change demos every 15 seconds
        {
            incrementDemo();
//            Serial.print("autocycling...Demo is ");
//            Serial.println(demo);
            lastAutoCycle=millis();
        }
        
 }
 
void updateAccelerometer()
{
    for(int i=0;i<3;i++)
        accelerometer[i]=analogRead(X+i);
}