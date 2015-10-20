
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
  Cube cube = Cube(8, 50); // 8^3 cube with max brightness of 50
  cube.begin(); // initialize the cube

  cube.background(Color(0, 0, 0)); // clear the cube
  cube.sphere(4, 4, 4, 3, Color(255, 0, 0)); // draw a red sphere in the center
  cube.show();
}