
#include "beta-cube-library.h"

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
int delayValue  = 1000; // Delay time for new number cycling, in ms

int number;

Color colorTenThousands;
Color colorOneThousands;
Color colorHundreds;
Color colorTens;
Color colorOnes;
	
Point pointTenThousands;
Point pointOneThousands;
Point pointHundreds;
Point pointTens;
Point pointOnes;

//==================================================
// Method Declarations
//
void initVoxelData();
void checkTenThousands();
void checkOneThousands();
void checkHundreds();
void checkTens();
void checkOnes();

//==================================================
// Setup code
//
void setup()
{
  	cube.begin();
}


//==================================================
// Loop code
//
void loop()
{

	cube.background(black);
	//
	// Initialize the default voxel data
	//
	initVoxelData();
	
  	//
  	// Pick a new random number to display.
  	//
  	number = random(1, 100000);
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
	delay(delayValue);

	//
	// Since this is a loop, this behavior will continue. Since this doesn't do
	// anything different, this displayed value will appear to be static/unchanging.
	//
	
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