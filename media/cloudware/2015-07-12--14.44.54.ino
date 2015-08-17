
#include "beta-cube-library.h"

char* vizName=" Solid Background Color";
int vizId=575;
// Solid Background color
// @damiannelson - 2015-07-12

// accelerometer code lifted from original L3D demo

Cube cube = Cube();

// digit colors
Color hdcolor = Color(30, 0, 0);
Color bg = blue;

int size = 0;

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

long lastFaceplant = -1 * FLIP_TIMEOUT;
bool upsideDown = false;
bool sideways = false;
int upsideDownTime = -1 * FLIP_TIMEOUT;
long lastAutoCycle = 0;
int lastLeft = -1 * FLIP_TIMEOUT;
int lastRight = -1 * FLIP_TIMEOUT;
int accelerometer[3];
long lastChange = 0;

int maxBrightness = 50;

void checkFlipState();
void updateAccelerometer();

void setup()
{
	cube.begin();
	cube.background(bg);
	size = cube.size;
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
	checkFlipState();
	cube.show();
}

void checkFlipState()
{
    updateAccelerometer();

	if(accelerometer[0]>FACEPLANT)  //if the cube is upside-down, set the upside-down flag and mark the time when it was flipped
    {
        lastFaceplant = millis();
		Color color = Color(30, 0, 30)
        cube.background(color);
    }
    if(accelerometer[1]<LEFT_SIDE)  //if the cube is flipped to either side
    {
        lastLeft = millis();
        Color color = Color(30, 0, 0);
        cube.background(color);
    }
    if(accelerometer[1]>RIGHT_SIDE)
    {
        lastRight = millis();
        Color color = Color(0, 30, 0)
        cube.background(color);
    }

    if(accelerometer[2]>RIGHTSIDE_UP)
    {
        Color color = Color(30, 30, 30);
        cube.background(color);
    }       
}
 
void updateAccelerometer()
{
    for(int i=0; i<3; i++)
        accelerometer[i] = analogRead(X+i);
}
