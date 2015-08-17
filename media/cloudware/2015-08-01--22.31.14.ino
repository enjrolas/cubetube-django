
#include "beta-cube-library.h"

 /*
 Centercode logo
 
 created 7 July 2015
 by Neil White
 
 */


Cube cube = Cube(); //sets the cube: this intializes a default 8x8x8 Cube
int inc=1;  //increment: this controls how many pixels the animation will jump at each step
int pos=0;  //position: tracks position and direction of the animation
int frame=0;  
int speed=5;

int logoColors[][] = {
  {Color(0,0,0), Color(0,0,0), Color(125,76,33), Color(224,137,56), Color(217,133,55), Color(110,69,29), Color(0,0,0), Color(0,0,0)},
  {Color(0,0,0), Color(255,255,255), Color(255,255,255), Color(255,206,63), Color(255,255,113), Color(255,255,255), Color(255,255,255), Color(0,0,0)},
  {Color(196,66,48), Color(255,208,96), Color(0,0,0), Color(0,0,0), Color(0,0,0), Color(155,55,15), Color(255,128,39), Color(149,92,39)},
  {Color(249,83,59), Color(255,170,71), Color(0,0,0), Color(53,106,111), Color(78,157,165), Color(0,0,0), Color(117,42,11), Color(161,74,26)},
  {Color(250,88,59), Color(255,176,73), Color(0,0,0), Color(71,143,150), Color(97,198,207), Color(0,0,0), Color(0,0,0), Color(178,61,37)},
  {Color(193,66,47), Color(255,248,102), Color(0,0,0), Color(0,0,0), Color(0,0,0), Color(0,0,0), Color(255,85,61), Color(137,47,34)},
  {Color(0,0,0), Color(255,255,255), Color(0,0,0), Color(0,0,0), Color(255,255,194), Color(255,255,255), Color(255,255,255), Color(0,0,0)},
  {Color(0,0,0), Color(0,0,0), Color(106,36,26), Color(194,65,46), Color(174,59,37), Color(72,26,12), Color(0,0,0), Color(0,0,0)}
};

void setup() { 
  cube.begin();  //tells the Cube to start

}


void loop() {
  cube.background(black);  //clears the Cube
 
  if(frame%speed==0) //this if statement controls the speed of the animation
    pos+=inc;  //https://www.arduino.cc/en/Reference/IncrementCompound
  if((pos<=0)||(pos>=cube.size-1))  //if the points go beyond the bounds of the cube, bounce them back the other way
      inc*=-1; //https://www.arduino.cc/en/Reference/IncrementCompound
 

  for(int x=7;x>=0;x--)
    for(int y=7;y>=0;y--)
      cube.setVoxel(y, x, pos, logoColors[y][x]);
  
  cube.show(); //the cube won't show any new information without this line -- always necessary
  frame++;  //this is how we're updating the color shift on each pass through the loop
}