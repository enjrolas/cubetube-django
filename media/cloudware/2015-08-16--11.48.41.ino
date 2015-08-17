
#include "beta-cube-library.h"

Cube cube = Cube(); 
#define MAX_DOTS 15
Point dots[MAX_DOTS];
Point dir[MAX_DOTS];
Color clr[MAX_DOTS];

void setup() { 
  cube.begin();
  srand(millis());
  
  for (int i=0; i<MAX_DOTS; i++) {
    dots[i] = { rand()%cube.size, rand()%cube.size, rand()%cube.size };
    //clr[i] = Color(rand()%16, rand()%16, rand()%16);
    randomColor(&clr[i]);
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

int lastRand = 0;
int lastLastRand = 0;

int strongColor() {
  return random(3,13);
}

int weakColor() {
  return random(2);
}

void randomColor(struct Color *clr) {
  int r;
  do {
    r = random(7);
  } while (r == lastRand || r == lastLastRand);
  
  switch (r) {
    case 0: 
      clr->red   = strongColor();
      clr->green = strongColor();
      clr->blue  = strongColor();
      break;
    case 1: 
      clr->red   = strongColor();
      clr->green = weakColor();
      clr->blue  = weakColor();
      break;
    case 2: 
      clr->red   = weakColor();
      clr->green = strongColor();
      clr->blue  = weakColor();
      break;
    case 3: 
      clr->red   = weakColor();
      clr->green = weakColor();
      clr->blue  = strongColor();
      break;
    case 4: 
      clr->red   = weakColor();
      clr->green = strongColor();
      clr->blue  = strongColor();
      break;
    case 5: 
      clr->red   = strongColor();
      clr->green = weakColor();
      clr->blue  = strongColor();
      break;
    case 6: 
      clr->red   = strongColor();
      clr->green = strongColor();
      clr->blue  = weakColor();
      break;
  }
  lastLastRand = lastRand;
  lastRand = r;
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
    if (bang) uc = Color(128, 128, 128);
    if (bang) cube.sphere(dots[i], 1, Color(4, 4, 4));
    cube.setVoxel(dots[i], uc);
    if (bang) dots[i] = { rand()%cube.size, rand()%cube.size, rand()%cube.size };
   }

  for (int i=0; i<MAX_DOTS; i++) {
    if (rand()%16 != 0) continue;
    // NYI - share code with setup
    int d[3], a;
    do {
      d[0] = rand()%3-1;
      d[1] = rand()%3-1;
      d[2] = rand()%3-1;
      a = abs(d[0]) + abs(d[1]) + abs(d[2]);
    } while (a != 1);
    dir[i] = { d[0], d[1], d[2] };
  }

  cube.show(); 
  
  delay(100);
}