
#include "beta-cube-library.h"

//==================================================
// Global Constants
//

//
// Accelerometer Defines
//
#define X 13	// Accelerometer pinout: X
#define Y 14	// Accelerometer pinout: Y
#define Z 15	// Accelerometer pinout: Z


//==================================================
// Global Variables
//
Cube cube = Cube();
Color mfColorRed   = Color(0xff, 0x00, 0x00);
Color mfColorGreen = Color(0x00, 0xff, 0x00);
Color mfColorBlue  = Color(0x00, 0x00, 0xff);
int rowTenThous = 7;
int rowOneThous = 6;
int rowHuns     = 5;
int rowTens     = 4;
int rowOnes     = 3;
int frontRow    = 7;

//
// Set the read axis below. Note:
// readAxis 0 = tilt forwards and backwards
// readAxis 1 = tilt left and right
// readAxis 2 = tilt
int readAxis    = 2;

int number;

Color colorTenThousands = Color(black);
Color colorOneThousands = Color(black);
Color colorHundreds = Color(black);
Color colorTens = Color(black);
Color colorOnes = Color(black);
	
Point pointTenThousands;
Point pointOneThousands;
Point pointHundreds;
Point pointTens;
Point pointOnes;

//
// General Variables
//
int loopVar;

//
// Accelerometer Variables
//
int accelerometer[3];


//==================================================
// Method Declarations
//
void initVoxelData();
void checkTenThousands();
void checkOneThousands();
void checkHundreds();
void checkTens();
void checkOnes();
void checkFlipState();
void updateAccelerometer();


//==================================================
// Cube Code
//

//
// SETUP method
//
// The setup method is for code that is run only once.
//
void setup () {
  	cube.begin();
}

//
// LOOP method
//
// The loop method is for code that keeps on running after
// the setup method is finished and runs until the device
// is powered off.
//
void loop () {

	cube.background(black);
	//
	// Initialize the default voxel data
	//
	initVoxelData();
	
  	//
  	// Get updates to the accelerometer.
  	//
	checkFlipState();

  	number = accelerometer[readAxis];
  	
	//
	// Check for ten thousands
	//
	if (number > 9999) {
		checkTenThousands();
	}
	
	if (number > 999) {
		checkOneThousands();
	}

	if (number > 99) {
		checkHundreds();
	}
	
	if (number > 9) {
		checkTens();
	}
	
	checkOnes();	
  	
  	cube.setVoxel(pointTenThousands,colorTenThousands);
  	cube.setVoxel(pointOneThousands,colorOneThousands);
  	cube.setVoxel(pointHundreds,colorHundreds);
  	cube.setVoxel(pointTens,colorTens);
  	cube.setVoxel(pointOnes,colorOnes);
	cube.show();

	//
	// Since this is a loop, this behavior will continue. Since this doesn't do
	// anything different, this displayed value will appear to be static/unchanging.
	//

}

//==================================================
// Accelerometer Code
//
 
void updateAccelerometer() {
  for(int loopVar = 0; loopVar < 3; loopVar++) {
    accelerometer[loopVar] = analogRead(X + loopVar);
  }
}

void checkFlipState () {
  updateAccelerometer();
  
   
/*
 * Code not used but taken from the KS demo
 *

#define AUTOCYCLE_TIME 22222
#define FACEPLANT 2300
#define UPSIDE_DOWN 1850
#define RIGHTSIDE_UP 2400
#define LEFT_SIDE 1800
#define RIGHT_SIDE 2400
#define FLIP_TIMEOUT 3000
#define FLIP_DEBOUNCE 250

    if(accelerometer[2]<UPSIDE_DOWN)  //if the cube is upside-down, set the upside-down flag and mark the time when it was flipped
    {
        upsideDownTime=millis();
  //      Serial.println("I'm upside-down!");
    }
    
    if(accelerometer[0]>FACEPLANT)  //if the cube is upside-down, set the upside-down flag and mark the time when it was flipped
    {
        lastFaceplant=millis();
  //      Serial.println("I'm upside-down!");
    }
    if(accelerometer[1]<LEFT_SIDE)  //if the cube is flipped to either side
    {
        lastLeft=millis();
//        Serial.println("I'm on my left side");
    }
    if(accelerometer[1]>RIGHT_SIDE)
    {
        lastRight=millis();
  //      Serial.println("I'm on my right side");
    }
 
    if(accelerometer[2]>RIGHTSIDE_UP)
    {
        if(((millis()-lastFaceplant)<FLIP_TIMEOUT)&&(millis()-lastFaceplant>FLIP_DEBOUNCE))
        {
            autoCycle=false;
            lastFaceplant=millis()-FLIP_TIMEOUT;
            color flash;
            flash.red=maxBrightness;
            flash.green=maxBrightness;
            flash.blue=maxBrightness;
            background(flash);
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
*/       
}

void initVoxelData () {
	colorTenThousands = Color(mfColorGreen);
	colorOneThousands = Color(mfColorGreen);
	colorHundreds = Color(mfColorGreen);
	colorTens = Color(mfColorGreen);
	colorOnes = Color(mfColorGreen);
	
	pointTenThousands = {0, rowTenThous, frontRow};
	pointOneThousands = {0, rowOneThous, frontRow};
	pointHundreds = {0, rowHuns, frontRow};
	pointTens = {0, rowTens, frontRow};
	pointOnes = {0, rowOnes, frontRow};
}

void checkTenThousands () {
	int target = (number / 10000);
	int x = target - 1;
	int y = rowTenThous;
	int z = frontRow;
	colorTenThousands = mfColorBlue;
	if (target >= 9) {
		x--;
		colorTenThousands = mfColorRed;
	}
	pointTenThousands = {x, y, z};
	number = number - (target * 10000);
}

void checkOneThousands () {
	int target = (number / 1000);
	int x = target - 1;
	int y = rowOneThous;
	int z = frontRow;
	colorOneThousands = mfColorBlue;
	if (target >= 9) {
		x--;
		colorOneThousands = mfColorRed;
	}
	pointOneThousands = {x, y, z};
	number = number - (target * 1000);
}

void checkHundreds () {
	int target = (number / 100);
	int x = target - 1;
	int y = rowHuns;
	int z = frontRow;
	colorHundreds = mfColorBlue;
	if (target >= 9) {
		x--;
		colorHundreds = mfColorRed;
	}
	pointHundreds = {x, y, z};
	number = number - (target * 100);
}

void checkTens () {
	int target = (number / 10);
	int x = target - 1;
	int y = rowTens;
	int z = frontRow;
	colorTens = mfColorBlue;
	if (target >= 9) {
		x--;
		colorTens = mfColorRed;
	}
	pointTens = {x, y, z};
	number = number - (target * 10);
}

void checkOnes () {
	int target = number;
	int x = target - 1;
	int y = rowOnes;
	int z = frontRow;
	colorOnes = mfColorBlue;
	if (target >= 9) {
		x--;
		colorOnes = mfColorRed;
	}
	pointOnes = {x, y, z};
}