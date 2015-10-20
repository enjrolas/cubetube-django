
#include "beta-cube-library.h"

char* vizName="  letter-experiments";
int vizId=647;
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
		{32,239,255,223,159,64,0,0},
		{0,16,112,175,255,255,64,0},
		{143,143,0,0,32,255,127,0},
		{127,255,127,64,127,255,64,0},
		{0,127,239,255,223,112,32,0}
		};

int lT[8][8]={
		{0,223,255,255,255,255,223,0},
		{0,223,223,255,255,223,223,0},
		{0,0,0,255,255,0,0,0},
		{0,0,0,255,255,0,0,0},
		{0,0,0,255,255,0,0,0},
		{0,0,0,255,255,0,0,0},	
		{0,0,0,255,255,0,0,0},
		{0,0,0,255,255,0,0,0},
		};

// Spell the Word. MAKE SURE array Pointer length is as long as your word ie.letters[NUM] 
int (*letters[9])[8][8] = { &lA, &lF, &lR, &lO, &lB, &lE, &lA, &lT, &lS };

// SETUP 
void setup()
{
  cube.begin();
  cube.background(black);
  cube_max = cube.size - 1;
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

// FUNCTIONS
void setLetter(int letterVal[][8]){
  for (int z = 0; z <= cube_max; z++) {
    
    //int rgbColor = changeColor(z);
    Color letterColor = Color((255-(z*32)),(255-(z+32)), (255-(z-32)));
    //Color letterColor = Color(255,100,0);
 	for (int x = 0; x <= cube_max; x++) { 
      for (int y = 0; y <= cube_max; y++) {           
        if ( letterVal[y][x] > 0 ) {
          
          cube.setVoxel(x,cube_max-y,z,letterColor);
        }
      }
    }
    cube.show();
    delay(50);
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

void runLetters() {
 	for(int i=0; i< 9; i++) {
     	setLetter(*letters[i]);
  		clearPlane();  
    }

}

int* changeColor(int seedValue) {
  int rgbColour[3];
  // Start off with seededValue.
  rgbColour[0] = 255;
  rgbColour[1] = 0;
  rgbColour[2] = 0;  
 
  // Choose the colours to increment and decrement.
  for (int decColour = 0; decColour < 3; decColour++) {
    int incColour = decColour == 2 ? 0 : decColour + 1;
 
    // cross-fade the two colours.
    for(int i = 0; i < 255; i++) {
      rgbColour[decColour] -= seedValue;
      rgbColour[incColour] += seedValue;
      
      return rgbColour;
    }
  } 
  
}


// FINALLY THE LOOP
void loop()
{
  cube.background(black);
  
  runLetters();
}
