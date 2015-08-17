
#include "beta-cube-library.h"

Cube cube = Cube();

//int LIGHTNINGINTERVAL 950; // How many raindrops between random lightning strikes, on average
//int RAINDROPDELAY 80;     // Microseconds between raindrop movements
//int MAXLIGHTNING 3;       // Maximum number of flashes per lightning strike
int SHOWCLOUDS 1; 

byte drop1XPos;
byte drop1YPos;
byte drop1ZPos = 2;

byte drop2XPos;
byte drop2YPos;
byte drop2ZPos = 0;

void setup() {
    
    
  cube.begin();
  cube.background(black);
  
  //cube.show();
}

void loop() {
  if(drop1ZPos == 8)
  {
    drop1XPos = random(8);
    drop1YPos = random(8);
  }

  if(drop2ZPos == 8)
  {
    drop2XPos = random(8);
    drop2YPos = random(8);
  }

  //cube.background(black);
  if(SHOWCLOUDS == 1)
  {
    for(int x=0; x<8; x++) {
   	  for(int y=0; y<8; y++) {
    	cube.setVoxel(x,y,7,white);
      }
    }  
  }
  cube.show();
}
/*
  if(drop1ZPos > 0)
  {
    drop1ZPos--;
    //strip.setPixelColor(drop1XPos * 8 + drop1ZPos, 0,0,255);
    //strip.show();
    cube.setVoxel(drop1XPos, drop1YPos, drop1ZPos, blue);
  } else {
    drop1ZPos = 8;
  }

  if(drop2ZPos > 0)
  {
    drop2ZPos--;
    //strip.setPixelColor(drop2XPos * 8 + drop2ZPos, 0,0,255);
    //strip.show();
    cube.setVoxel(drop2XPos, drop2YPos, drop2ZPos, blue);
  } else {
    drop2ZPos = 8;
  }
    strip.show();

  //delayMicroseconds(1);

  //lightning();
}

void lightning()
{
  if(random(LIGHTNINGINTERVAL) == 1)
  {
    int lightningFlashes = random(0, MAXLIGHTNING + 1);
    int i = 0;
    while (i < lightningFlashes)
    {
      cube.background(white);
      cube.show();
      delay(50);
      cube.background(black);
      
      cube.show();
      delay(150);
      i++;
    }
  }
}
*/