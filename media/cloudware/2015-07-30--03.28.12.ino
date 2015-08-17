
#include "beta-cube-library.h"

#define NAME "ALEXII"

Cube cube = Cube();
int cube_max;

Color clearColor = black;



int lA[8][8]={
		{0,60,60,0,0,60,60,0},
		{0,60,60,0,0,60,60,0},
		{0,60,60,0,0,60,60,0},
  		{0,60,60,60,60,60,60,0},
		{0,60,60,60,60,60,60,0},
        {0,60,60,0,0,60,60,0},
		{0,60,60,0,0,60,60,0},
		{0,60,60,0,0,60,60,0}
		};


int lB[8][8]={
		{0,0,0,180,180,0,0,0},
		{0,180,180,180,180,180,180,0},
		{0,180,180,0,0,180,180,0},
  		{0,180,180,0,0,180,180,0},
		{0,180,180,180,180,180,180,0},
		{0,180,180,180,180,180,180,0},	
		{0,180,180,0,0,180,180,0},
		{0,180,180,0,0,180,180,0}
		};

int lE[8][8]={
		{0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0},
  		{0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0},
        {0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0}
		};

int lF[8][8]={
		{0,64,64,64,0,0,0,0},
		{0,64,64,64,64,0,0,0},
		{0,64,64,0,0,64,0,0},
  		{0,64,64,0,0,0,64,0},
		{0,64,64,0,0,0,64,0},
		{0,64,64,0,0,0,64,0},	
		{0,64,64,64,64,0,0,0},
		{0,64,64,64,0,0,0,0}
		};

int lO[8][8]={
		{0,64,64,0,0,0,0,0},
		{0,64,64,0,0,0,0,0},
		{0,64,64,0,0,0,0,0},
  		{0,64,64,0,0,0,0,0},
  		{0,64,64,0,0,0,0,0},
        {0,64,64,0,0,0,0,0},
  		{0,64,64,64,64,64,0,0},
		{0,64,64,64,64,64,0,0}
		};

int lP[8][8]={
		{0,64,64,0,0,0,64,64},
		{0,64,64,64,64,64,64,64},
		{0,64,64,64,64,64,64,64},
  		{0,64,64,0,64,64,64,64},
		{0,64,64,0,0,64,64,64},
        {0,64,64,0,0,0,64,64},
		{0,64,64,0,0,0,64,64},
		{0,64,64,0,0,0,64,64}
		};

int lR[8][8]={
		{0,0,0,180,180,0,0,0},
		{0,180,180,180,180,180,180,0},
		{0,180,180,0,0,180,180,0},
  		{0,180,180,0,0,180,180,0},
		{0,180,180,180,180,180,180,0},
		{0,180,180,180,180,180,180,0},	
		{0,180,180,0,0,180,180,0},
		{0,180,180,0,0,180,180,0}
		};

int lS[8][8]={
		{0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0},
  		{0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0},
        {0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0},
		{0,0,0,64,64,0,0,0}
		};



// Spell the Word. MAKE SURE array Pointer length is as long as your word ie.letters[NUM] 

int (*letters[9])[8][8] = { &lH &lA, &lI, &lD, &lL, &lM, &lA, &lI, &lR, };

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
          
          // Color softener set variables in runLetters
          //int c = letterVal[y][x]/colorSeed;
          //letterColor.red = c*c;
          //letterColor.green = c*c;
          //letterColor.blue = c*c;                    
         
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
  // Add Weird colors
  //for(int c=1;c<127; c++) {     
  // Add Soft colors
  for(int c=16;c<64; c++) {   
    // TODO make threshold dynamic
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
