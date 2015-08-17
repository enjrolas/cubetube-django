
#include "beta-cube-library.h"

#include <application.h>
Cube cube = Cube(); 

void setup() { 
  cube.begin();
  srand(millis());
}

void loop() {

  cube.background(black);

  Point p1 = { rand()%cube.size, cube.size-1, rand()%cube.size };
  Point p2 = { rand()%cube.size, rand()%3, rand()%cube.size };
  Point p3 = { rand()%4-2+p2.x, 0, rand()%4-2+p2.z };
  Point p4 = { rand()%4-2+p2.x, 0, rand()%4-2+p2.z };

  Color clr[3];
  clr[0] = Color(rand()%2, rand()%2, rand()%16);
  clr[1] = Color(128, 128, 128);
  clr[2] = Color(0, 0, 2);
  
  for (int i=0; i<3; i++) {
    cube.line(p1, p2, clr[i]);
    cube.line(p2, p3, clr[i]);
    cube.line(p2, p4, clr[i]);
    cube.show();
    delay(100);
  }
  
  delay(200);
  cube.background(black);
  cube.show();
  
  delay(200+rand()%3000);
}