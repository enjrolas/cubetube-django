
#include "beta-cube-library.h"

Cube cube = Cube();
int pos=0;
int inc=1;
int frame=0;

void setup() {
  cube.begin();
}

void loop() {
  cube.background(black);
  
  Point start={pos, cube.size-1, 0};
  Point end={cube.size-1-pos, 0, cube.size-1};
  cube.line(start,end,cube.colorMap(frame%1000,0,1000));
  
  Point start2={cube.size-1-pos, 0, 0};
  Point end2={pos, cube.size-1, cube.size-1};
  cube.line(start2,end2,cube.colorMap(frame%1000,0,1000));
  
  pos+=inc;
  if ((pos==0)||(pos==cube.size-1)) {
    inc*=-1;
  }
  
  frame++;
  cube.show();
}