
#include "beta-cube-library.h"

// L3D Clock
// Dennis Williamson - 2015/06/14
//
// Modified by:
// Werner Moecke - 2015/07/22
//
// accelerometer code lifted from original L3D demo
// used here to set timezone and toggle 24hr/am-pm mode

Cube cube = Cube();

bool use24hr = false;
int tz_offset = -3; // -12.0 to 13.0 + UTC

int seconds = 0;
Point s = Point(0, 0, 0);
Color scolor = black;

int minutes = 0;
Point m = Point(0, 0, 0);
Color mcolor = black;

int hours = 0;
Point h = Point(0, 0, 0);
Color hcolor = black;

// digit positions
int hrow = 3;
int hplane = 6;
int mrow = 2;
int mplane = 3;
int srow = 1;
int splane = 0;

// digit colors
Color hdcolor = black;
Color mdcolor = black;
Color sdcolor = black;

static const bool ampm[2][3][2] = {
  {
    {1, 1},
    {1, 1},
    {1, 1}
  },
  {
    {1, 1},
    {1, 1},
    {1, 0}
  }
};

//Point amdot = Point(0, 0, 0);
//Point pmdot = Point(0, 0, 0);
Color amcolor = orange; // dim orange
Color pmcolor = teal;   // dim purple

Color bg = black;

int size = 0;

//maxBrightness is the brightness limit for each pixel.  All color data will be scaled down 
//so that the largest value is maxBrightness
int maxBrightness = 64;

/**********************************
 * flip variables                 *
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

/**********************************
 * function definitions           *
 * ********************************/
void display_time(int hours, int minutes, int seconds);
void display_digits(int number, int drow, int dplane, Color numcolor);
void checkFlipState();
void updateAccelerometer();
Color adjust_brightness(Color col, int amount_perc);

void setup()
{
	cube.begin();
	cube.background(bg);
  	//not working atm
  	//cube.maxBrightness = 30;
  	
  	//adjust color intensity (dim by percent)
  	amcolor = adjust_brightness(amcolor, 20);
  	pmcolor = adjust_brightness(pmcolor, 20);

  	size = cube.size;

	//amdot = Point(0, size - 1, size - 1);
	//pmdot = Point(size - 1, size - 1, size - 1);

	// set plane to display time elements
	s.z = size - 7;
	m.z = size - 4;
	h.z = size - 1;

	Time.zone(tz_offset);
}

void loop()
{
  	// calling this function prevents the cube
    // from being disconnected by cubetube's API
  	//Spark.process();

  	cube.background(bg);

	seconds = Time.second();
	minutes = Time.minute();
	if (use24hr)
    {
    	hours = Time.hour();
    }
	else
    {
    	hours = Time.hourFormat12();
		if (Time.isAM())
        {
          for (int row = 0; row < 3; row++)
			for (int col = 0; col < 2; col++)
              if (ampm[0][row][col])
                cube.setVoxel(col, size - (row + 1), size - 1, amcolor);
          //cube.setVoxel(amdot, amcolor);
        }
    	else
        {
          for (int row = 0; row < 3; row++)
			for (int col = 0; col < 2; col++)
              if (ampm[1][row][col])
                cube.setVoxel(col, size - (row + 1), size - 1, pmcolor);
          //cube.setVoxel(pmdot, pmcolor);
        }
    }

	s.x = seconds % size;
	s.y = seconds / size;

	m.x = minutes % size;
	m.y = minutes / size;

	h.x = hours % size;
	h.y = hours / size;

	cube.setVoxel(s, scolor);
	cube.setVoxel(m, mcolor);
	cube.setVoxel(h, hcolor);
	
  	//this sets the color tones and the ranges for
  	//varying each color; if you don't like going too
  	//wild, you can try narrowing the ranges to create
  	//more subtle tones to match your taste...  ;-P
	hdcolor=cube.colorMap(hours, maxBrightness*2, maxBrightness*4);
	mdcolor=cube.colorMap(minutes, maxBrightness, maxBrightness*2);
    sdcolor=cube.colorMap(seconds, cube.size, maxBrightness);
  	hcolor = hdcolor;
  	mcolor = mdcolor;
  	scolor = sdcolor;
  	//adjust color intensity (dim by percent)
  	hdcolor = adjust_brightness(hdcolor, 60);
  	mdcolor = adjust_brightness(mdcolor, 60);
  	sdcolor = adjust_brightness(sdcolor, 60);
  	hcolor = adjust_brightness(hdcolor, 20);
  	mcolor = adjust_brightness(mdcolor, 20);
  	scolor = adjust_brightness(sdcolor, 20);

	display_time(hours, minutes, seconds);

	checkFlipState();

	cube.show();

  	/*** DO NOT COMMENT OUT/REMOVE THIS LINE ***/
	delay(50);
}

/* 
 * Adjust a color by setting each of its components
 * to a percentage based on the current value setting
 */
Color adjust_brightness(Color col, int amount_perc)
{
  	//set new components by a percentage of the actual values
  	col.red   = col.red*amount_perc/100;
  	col.green = col.green*amount_perc/100;
  	col.blue  = col.blue*amount_perc/100;
  	//constrain components to maxBrightness; might distort
  	//colors if the percentage was set too high
  	if (col.red > maxBrightness) col.red = maxBrightness;
  	if (col.green > maxBrightness) col.green = maxBrightness;
  	if (col.blue > maxBrightness) col.blue = maxBrightness;
  
  	return col;
}

void display_time(int hours, int minutes, int seconds)
{
  	display_digits(hours, hrow, hplane, hdcolor);
	display_digits(minutes, mrow, mplane, mdcolor);
	display_digits(seconds, srow, splane, sdcolor);
}

void display_digits(int number, int drow, int dplane, Color numcolor)
{
    static const bool digits[10][5][3] = {
        {
            {1, 1, 1},
            {1, 0, 1},
            {1, 0, 1},
            {1, 0, 1},
            {1, 1, 1}
        },
        {
            {0, 1, 0},
            {1, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {1, 1, 1}
        },
        {
            {1, 1, 1},
            {0, 0, 1},
            {0, 1, 1},
            {1, 0, 0},
            {1, 1, 1}
        },
        {
            {1, 1, 0},
            {0, 0, 1},
            {1, 1, 0},
            {0, 0, 1},
            {1, 1, 0}
        },
        {
            {1, 0, 1},
            {1, 0, 1},
            {1, 1, 1},
            {0, 0, 1},
            {0, 0, 1}
        },
        {
            {1, 1, 1},
            {1, 0, 0},
            {1, 1, 1},
            {0, 0, 1},
            {1, 1, 0}
        },
        {
            {0, 1, 1},
            {1, 0, 0},
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1}
        },
        {
            {1, 1, 1},
            {0, 0, 1},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0}
        },
        {
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1}
        },
        {
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1},
            {0, 0, 1},
            {1, 1, 0}
        }
    };

	int indiv[2];

    indiv[0] = number / 10;
    indiv[1] = number % 10;

	int dcol = 0;

	for (int d = 0; d <= 1; d++)
    {
        for (int row = 0; row < 5; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                if (digits[indiv[d]][row][col])
                {
                    cube.setVoxel(dcol + col, size - row - drow - 1, dplane, numcolor);
                }
            }
        }
    dcol = 5;
	}
}

void checkFlipState()
{
    updateAccelerometer();

	if(accelerometer[0]>FACEPLANT)  //if the cube is upside-down, set the upside-down flag and mark the time when it was flipped
    {
        lastFaceplant = millis();
    }
    if(accelerometer[1]<LEFT_SIDE)  //if the cube is flipped to either side
    {
        lastLeft = millis();
    }
    if(accelerometer[1]>RIGHT_SIDE)
    {
        lastRight = millis();
    }

    if(accelerometer[2]>RIGHTSIDE_UP)
    {
        if(((millis()-lastFaceplant)<FLIP_TIMEOUT) && (millis()-lastFaceplant>FLIP_DEBOUNCE))
        {
        	// toggle use24hr
        	use24hr = !use24hr;
            lastFaceplant = millis()-FLIP_TIMEOUT;
        }
        if(((millis()-lastLeft)<FLIP_TIMEOUT) && (millis()-lastChange>FLIP_DEBOUNCE))
        {
        	// decrement timezone
          	tz_offset--;
            tz_offset = constrain(tz_offset, -12, 13);
			Time.zone(tz_offset);
            lastChange = millis();
            lastLeft = millis() - FLIP_TIMEOUT;
        }
        if(((millis()-lastRight)<FLIP_TIMEOUT) && (millis()-lastChange>FLIP_DEBOUNCE))
        {
        	// increment timezone
          	tz_offset++;
            tz_offset = constrain(tz_offset, -12, 13);
			Time.zone(tz_offset);
            lastChange = millis();
            lastRight = millis() - FLIP_TIMEOUT;
        }
    }       
 }
 
void updateAccelerometer()
{
    for(int i=0; i<3; i++)
        accelerometer[i] = analogRead(X+i);
}