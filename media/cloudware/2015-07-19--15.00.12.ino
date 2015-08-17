
#include "beta-cube-library.h"

char* vizName="                letter-box";
int vizId=642;
#define NAME "SANDRO"

Cube cube = Cube();
int cube_max;


static const int letter[8][8]={
		{0,127,239,255,207,80,0,0},
		{96,255,112,64,191,255,32,0},
		{127,255,64,0,32,127,32,0},
		{32,239,255,223,159,64,0,0},
		{0,16,112,175,255,255,64,0},
		{143,143,0,0,32,255,127,0},
		{127,255,127,64,127,255,64,0},
		{0,127,239,255,223,112,32,0}
		};

void setup()
{
  cube.begin();
  cube.background(black);
  cube_max = cube.size - 1;
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void setFront(){
 	for (int x = 0; x <= cube_max; x++) 
      for (int y = 0; y <= cube_max; y++) 
        if ( letter[y][x] > 0 ) {
			Color letterColor;
          	int c = letter[y][x]/32;
		    letterColor.red = c*c;
		    letterColor.green = c*c;
		    letterColor.blue = c*c;
 		   	cube.setVoxel(x,cube_max-y,4,letterColor);
        }
}


void setFront2() {
 	for (int x = 0; x <= cube_max; x++) 
      for (int y = 0; y <= cube_max; y++) 
      {
        	Color letterColor;
		    letterColor.red = (y+1)*(y+1);
		    letterColor.green = (y+1)*(y+1);
		    letterColor.blue = (y+1)*(y+1);
            cube.setVoxel(x,y,cube_max,letterColor);
      }
}

void setLetter() {
	for (int x = 0; x <= cube_max; x++) 
      
}

void loop()
{
  cube.background(black);

  setFront();
  cube.show();
}




