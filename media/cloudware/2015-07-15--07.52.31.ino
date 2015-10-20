
#include "beta-cube-library.h"

char* vizName="         Classic Planes";
int vizId=616;
Cube cube = Cube(); //sets the cube: this intializes a default 8x8x8 Cube
int inc=1;  //increment: this controls how many pixels the animation will jump at each step
int pos=0;  //position: tracks position and direction of the animation
int frame=0;  


void setup() { 
  cube.begin();  //tells the Cube to start
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}


void loop() {
  cube.background(black);  //clears the Cube
 
  if(frame%3==0) //this if statement controls the speed of the animation
	  pos+=inc;  //https://www.arduino.cc/en/Reference/IncrementCompound
  if((pos<=0)||(pos>=cube.size))  //if the points go beyond the bounds of the cube, bounce them back the other way
      inc*=-1; //https://www.arduino.cc/en/Reference/IncrementCompound
  
for(int x=0;x<cube.size;x++)  //these three for statements will sweep through all 512 voxels of the cube and change the the voxel colors on each sweep
  for(int y=0;y<cube.size;y++)
    for(int z=0;z<cube.size;z++){
  
    cube.setVoxel(pos, y, z, cube.colorMap((frame+50)%100,0,200));  //a core function from the L3D library for setting the color of a particular voxel in the Cube: http://goo.gl/u1iVEF
                    
    }
  
    cube.show(); //the cube won't show any new information without this line -- always necessary
    frame++;  //this is how we're updating the color shift on each pass through the loop
  }