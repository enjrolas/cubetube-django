
#include "beta-cube-library.h"

char* vizName="  letter-experiments";
int vizId=647;
#define NAME "ALEXII"

Cube cube = Cube();
int cube_max;

Color clearColor = black;

int letterA[8][8]={
		{0,0,0,255,255,0,0,0},
		{0,127,255,0,0,255,127,0},
		{0,255,64,0,0,64,255,0},
  		{0,255,127,0,0,127,255,0},
		{0,255,255,223,255,255,255,0},
		{0,255,255,255,255,255,255,0},	
		{0,255,127,0,0,127,255,0},
		{0,255,127,0,0,127,255,0}
		};

int letterS[8][8]={
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

void loop()
{
  cube.background(black);
  
  setLetter(letterA);
  delay(50);
  clearPlane();  
  delay(50);
  
  setLetter(letterS);  
  delay(50);  
  clearPlane();  
  delay(50);
}

void setLetter(int letterVal[][8]){
  for (int z = 0; z <= cube_max; z++) {
 	for (int x = 0; x <= cube_max; x++) { 
      for (int y = 0; y <= cube_max; y++) {           
        if ( letterVal[y][x] > 0 ) {
          Color letterColor;
          int c = letterVal[y][x]/32;
          letterColor.red = c*c;
          letterColor.green = c*c;
          letterColor.blue = c*c;
          cube.setVoxel(x,cube_max-y,z,letterColor);
        }
      }
    }
    cube.show();
    delay(100);
  } 
}


void clearPlane() {
  for (int z = 0; z <= cube_max; z++) {
 	for (int x = 0; x <= cube_max; x++) { 
     for (int y = 0; y <= cube_max; y++) {          
       
        cube.setVoxel(x,cube_max-y,z,clearColor);
        
      }
    }
    cube.show();
    delay(100);
  } 
}