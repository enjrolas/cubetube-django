
#include "beta-cube-library.h"

#define NAME "ALEXII"

Cube cube = Cube();
int cube_max;

Color clearColor = black;


int lA[8][8]={
		{0,0,0,223,223,0,0,0},
		{0,64,255,127,127,255,127,0},
		{0,255,64,0,0,64,255,0},
  		{0,255,127,0,0,127,255,0},
		{0,255,255,223,255,255,255,0},
		{0,255,223,223,223,255,255,0},	
		{0,255,127,0,0,127,255,0},
		{0,255,127,0,0,127,255,0}
		};

int lB[8][8]={
		{0,223,255,255,223,0,0,0},
		{0,255,127,127,127,255,0,0},
		{0,255,64,0,127,223,0,0},
  		{0,255,255,255,223,0,0,0},
		{0,255,223,223,223,0,0,0},
        {0,255,64,0,127,223,0,0},
		{0,255,127,127,127,255,0,0},
		{0,255,255,255,223,0,0,0}
		};

int lE[8][8]={
		{0,255,255,255,255,223,0,0},
		{0,255,223,223,223,223,0,0},
		{0,255,64,0,0,0,0,0},
  		{0,255,255,255,255,0,0,0},
		{0,255,223,223,223,0,0,0},
        {0,255,64,0,0,0,0,0},
		{0,255,127,127,127,127,0,0},
		{0,255,255,255,255,223,0,0}
		};

int lF[8][8]={
		{0,255,255,255,255,255,127,0},
		{0,255,223,223,223,223,127,0},
		{0,255,127,0,0,0,0,0},
  		{0,255,255,255,255,127,0,0},
		{0,255,223,223,223,127,0,0},
		{0,255,127,0,0,0,0,0},	
		{0,255,127,0,0,0,0,0},
		{0,255,127,0,0,0,0,0}
		};

int lO[8][8]={
		{0,0,0,127,127,0,0,0},
		{0,0,127,225,223,127,0,0},
		{0,223,127,32,32,127,223,0},
  		{0,255,127,0,0,127,255,0},
  		{0,255,64,0,0,64,255,0},
        {0,223,64,0,0,64,223,0},
  		{0,223,127,32,32,127,223,0},
		{0,0,127,225,223,127,0,0}
		};

int lP[8][8]={
		{0,223,255,255,223,0,0,0},
		{0,255,127,127,127,255,0,0},
		{0,255,64,0,127,223,0,0},
  		{0,255,255,255,223,0,0,0},
		{0,255,223,223,223,0,0,0},
        {0,255,64,0,0,0,0,0},
		{0,255,64,0,0,0,0,0},
		{0,255,64,0,0,0,0,0}
		};

int lR[8][8]={
		{0,223,255,255,223,0,0,0},
		{0,255,127,127,127,255,0,0},
		{0,255,64,0,127,223,0,0},
  		{0,255,255,255,223,0,0,0},
		{0,255,223,223,223,0,0,0},
        {0,255,64,0,64,223,0,0},
		{0,255,64,0,127,255,0,0},
		{0,255,64,0, 127,255,0,0}
		};
int lS[8][8]={
		{0,127,239,255,207,80,0,0},
		{96,255,112,64,191,255,32,0},
		{127,255,64,0,32,127,32,0},
		{32,239,255,0,0,32,0,0},
		{0,16,112,175,255,127,64,0},
  		{0,0,0,0,32,255,127,0},
		{127,255,127,64,127,255,64,0},
		{0,127,239,255,223,112,32,0}
		};

int lT[8][8]={
		{0,0,0,254,253,0,0,0},
		{0,0,252,251,250,245,0,0},
		{0,255,255,255,255,255,255,0},
		{255,255,255,255,255,255,255,255},
		{255,255,255,255,255,255,255,255},
		{0,255,255,255,255,255,255,0},	
		{0,0,255,255,255,255,0,0},
		{0,0,0,255,255,0,0,0},
		};

// Spell the Word. MAKE SURE array Pointer length is as long as your word ie.letters[NUM] 
int (*letters[1])[8][8] = { &lT };

// SETUP 
void setup()
{
  cube.begin();
  cube.background(black);
  cube_max = cube.size - 1;
}

// FUNCTIONS
void setLetter(int letterVal[][8], int colorSeed){
  
  for (int z = 0; z <= cube_max; z++) {    
    
 	for (int x = 0; x <= cube_max; x++) { 
      for (int y = 0; y <= cube_max; y++) {           
        if ( letterVal[y][x] > 0 ) {

          Color letterColor;
          // Weird Colors
          int c = letterVal[y][x]-colorSeed;
          letterColor = cube.colorMap(c, 0, 255);
          cube.setVoxel(x,cube_max-y,z,letterColor);
        }
      }
    }
    cube.show();
    delay(200);
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
    delay(30);
  } 
}

void runLetters() {
  for(int c=16;c<64; c++) {   
	for(int i=0; i < 9; i++) {
     	setLetter(*letters[i], c);
  		clearPlane();  
    }
  }
}


// FINALLY THE LOOP
void loop()
{
  cube.background(black);
  
  runLetters();
}
