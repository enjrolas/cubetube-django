
#include "beta-cube-library.h"

Cube cube = Cube(); 
#define MAX_DOTS 20
Point dots[MAX_DOTS];
Point dir[MAX_DOTS];
Color clr[MAX_DOTS];

void setup() { 
  cube.begin();
  srand(millis());
  
  for (int i=4; i<MAX_DOTS; i++) {
    dots[i] = { rand()%cube.size, rand()%cube.size, rand()%cube.size };
    clr[i] = Color(rand()%16, rand()%16, rand()%16);
    int d[3], a;
    do {
      d[0] = rand()%3-1;
      d[1] = rand()%3-1;
      d[2] = rand()%3-1;
      a = abs(d[0]) + abs(d[1]) + abs(d[2]);
    } while (a != 1);
    dir[i] = { d[0], d[1], d[2] };
  }
}

int wrapIf (int n) {
    if (n >= cube.size) n = 0;
    if (n < 0) n = cube.size - 1;
    return n;
}

void loop() {

  cube.background(black);  

  for (int i=0; i<MAX_DOTS; i++) {
    cube.setVoxel(dots[i], Color(clr[i].red/8, clr[i].green/8, clr[i].blue/8));
    dots[i].x += dir[i].x;
    dots[i].y += dir[i].y;
    dots[i].z += dir[i].z;
    dots[i].x = wrapIf(dots[i].x);
    dots[i].y = wrapIf(dots[i].y);
    dots[i].z = wrapIf(dots[i].z);
    Color test = cube.getVoxel(dots[i]);
    Color uc = clr[i];
    bool bang = false;
    if (test.red != 0 || test.green != 0 || test.blue != 0) bang = true;
    if (bang) uc = Color(255, 0, 0);
    if (bang) cube.sphere(dots[i], 1, Color(100, 200, 0));
    cube.setVoxel(dots[i], uc);
    if (bang) dots[i] = { rand()%cube.size, rand()%cube.size, rand()%cube.size };
   }

  for (int i=0; i<MAX_DOTS; i++) {
    if (rand()%16 != 0) continue;
    // NYI - share code with setup
    int d[3], a;
    do {
      d[0] = rand()%4-1;
      d[1] = rand()%5-2
      ;
      d[2] = rand()%3-1;
      a = abs(d[0]) + abs(d[1]) + abs(d[2]);
    } while (a != 1);
    dir[i] = { d[0], d[1], d[2] };
  }

  cube.show
  (); 
  
  delay(50);
}