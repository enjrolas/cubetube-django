
#include "beta-cube-library.h"

Cube cube = Cube();
int frame=0;

void setup()
{
  cube.begin();
  Serial.begin(115200);  //initialize the serial port
  cube.background(black);
}

void loop()
{
  cube.background(black);
  if(frame%20>10)
	  cube.setVoxel(3,3,3,blue);
  cube.show();
  
  //print out the state of the LED, whenever I turn it off or on
  if(frame%20==0)
    Serial.println("off");
  if(frame%20==10)
    Serial.println("on");
  
  frame++;
}