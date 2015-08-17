
#include "beta-cube-library.h"

char* vizName=" Cleaning the clock";
int vizId=663;
// L3D Clock
// Dennis Williamson - 2015-06-14

// accelerometer code lifted from original L3D demo
// used here to set timezone and toggle 24hr/am-pm mode

SYSTEM_MODE(SEMI_AUTOMATIC);  //don't connect to the internet on boot
#define BUTTON D2 //press this button to connect to the internet
#define MODE D3
bool onlinePressed=false;
bool lastOnline=true;
Cube cube = Cube();

bool use24hr = false;
int tz_offset = -3; // -12.0 to 13.0 + UTC

int seconds = 0;
Point s = Point(0, 0, 0);
Color scolor = blue;

int minutes = 0;
Point m = Point(0, 0, 0);
Color mcolor = purple;

int hours = 0;
Point h = Point(0, 0, 0);
Color hcolor = red;

// digit positions
int hrow = 3;
int hplane = 7;
int mrow = 2;
int mplane = 4;
int srow = 1;
int splane = 1;

// digit colors
Color hdcolor = Color(64, 0, 0);
Color mdcolor = Color(0, 64, 0);
Color sdcolor = Color(0, 0, 64);

Point amdot = Point(0, 0, 0);
Point pmdot = Point(0, 0, 0);
Color amcolor = Color(20, 20, 0); // dim yellow
Color pmcolor = Color(20, 0, 20); // dim purple

Color bg = black;

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

int maxBrightness = 64;

void display_time(int hours, int minutes, int seconds);
void display_digits(int number, int drow, int dplane, Color numcolor);
void checkFlipState();
void updateAccelerometer();

void setup()
{
  	initCloudButton();
	cube.begin();
	cube.background(bg);

	size = cube.size;

	amdot = Point(0, size - 1, size - 1);
	pmdot = Point(size - 1, size - 1, size - 1);

	// set plane to display time elements
	s.z = size - 8;
	m.z = size - 5;
	h.z = size - 2;

	Time.zone(tz_offset);
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
    //if the 'connect to cloud' button is pressed, try to connect to wifi.  
    //otherwise, run the program
    checkCloudButton();
  
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
        	cube.setVoxel(amdot, amcolor);
        }
    	else
        {
        	cube.setVoxel(pmdot, pmcolor);
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

	display_time(hours, minutes, seconds);

	checkFlipState();

	cube.show();

//	delay(50);
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
            {1, 1, 0},
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

//sets up the online/offline switch
void initCloudButton()
{
  //set the input mode for the 'connect to cloud' button
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(MODE, INPUT_PULLUP);
    if(!digitalRead(MODE))  //if the wifi button is held down on boot, do a hard reset.  At any other time, keep the firmware, but try to add new wifi creds
    {
        WiFi.on();
        WiFi.clearCredentials();
        System.factoryReset();
    }
    //a.k.a. onlinePressed is HIGH when the switch is set to 'online' and LOW when the switch is set to 'offline'
    onlinePressed=!digitalRead(BUTTON);
    if(onlinePressed)
        Spark.connect();
}

//checks to see if the 'online/offline' switch is switched
void checkCloudButton()
{
    //if the 'connect to cloud' button is pressed, try to connect to wifi.  
    //otherwise, run the program
    //note -- how does this behave when there are no wifi credentials loaded on the spark?

    //onlinePressed is HIGH when the switch is _not_ connected and LOW when the switch is connected
    //a.k.a. onlinePressed is HIGH when the switch is set to 'online' and LOW when the switch is set to 'offline'
    onlinePressed=!digitalRead(BUTTON);

    if((onlinePressed)&&(!lastOnline))  //marked as 'online'
    {
        lastOnline=onlinePressed;
        Spark.connect();
    }    

    else if((!onlinePressed)&&(lastOnline))  //marked as 'offline'
    {
        lastOnline=onlinePressed;
        Spark.disconnect();
    }

    lastOnline=onlinePressed;
    
    if(!digitalRead(MODE))
        WiFi.listen();
}