
#include "beta-cube-library.h"

Cube cube = Cube();

int r,g,b;
int mov, counter;
bool stationary=false;

void setup() {
  cube.begin();
}

void loop() {
  cube.background(black);
  
  Point start={1,2,mov};
  Point end={1,6,mov};
  
  r=16;
  g=78;
  b=193;
 
  cube.line(start,end,Color(r,g,b));
  
  r=10;
  g=10;
  b=10;
  
  Point start2={2,6,mov};
  Point end2={6,6,mov};
  cube.line(start2,end2,Color(r,g,b));
  
  Point start3={3,6,mov};
  Point end3={6,2,mov};
  cube.line(start3,end3,Color(r,g,b));
  
  Point start4={6,2,mov};
  Point end4={6,6,mov};
  cube.line(start4,end4,Color(r,g,b));
  
  Point v={4,5,mov};
  cube.setVoxel(v,Color(r,g,b));
  
  Point w={5,5,mov};
  cube.setVoxel(w,Color(r,g,b));
  
  Point y={5,4,mov};
  cube.setVoxel(y,Color(r,g,b));
  
  if (stationary==false)
  {
    counter++;
    if (counter==4) {
      counter=0;
      mov++;
    }
  }
  if (mov==7 && stationary==false) {
    stationary=true;
  }
  else if (mov==7 && stationary==true) {
    counter++;
    if (counter==65) {
      counter=0;
      mov=0;
      stationary=false;
    }
  }
  
  cube.show();
}