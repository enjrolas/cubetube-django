
#include "beta-cube-library.h"

Cube cube = Cube(); 

#define CYCLE_INTERVAL 10000 // milliseconds between restart
unsigned long lastSwap;

#define NUM_DOTS 7
Point pos[NUM_DOTS];
Point velocity[NUM_DOTS];
Color clr[NUM_DOTS];
float mass[NUM_DOTS];

Point gravity = { 0, 0.01, 0 }; 
//Point gravity = { 0.01, -0.01, 0 }; 
float damp = .0001; 

void setup() {
  cube.begin();
  srand(millis());
  initDots();
}

float randInitialVelocity() {
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX) / 7;
}

void randomColor(struct Color *clr) {
  clr->red   = rand()%16;
  clr->green = rand()%16;
  clr->blue  = rand()%16;
}

void initDots() {
  lastSwap = millis();
  int locus = random(cube.size);
  for (int i=0; i<NUM_DOTS; i++) {
    pos[i] = { locus, random(cube.size/2, cube.size), locus };
    velocity[i] = { randInitialVelocity(), randInitialVelocity(), randInitialVelocity() };
    randomColor(&clr[i]);
    mass[i] = 1;
  }    
}

float dampIt(float v) {
  if (v > 0) v -= damp;
  if (v < 0) v += damp;
  return v;
}

void applyForces() {
  for (int i=0; i<NUM_DOTS; i++) {
    velocity[i].x += gravity.x;
    velocity[i].y += gravity.y;
    velocity[i].z += gravity.z;
    
    velocity[i].x = dampIt(velocity[i].x);
    velocity[i].y = dampIt(velocity[i].y);
    velocity[i].z = dampIt(velocity[i].z);
  }
}

void fixDim(float *dim, float *v) {
  int max = cube.size - 1;
  if (*dim >= max) {
    *dim = max - (*dim - max);
    *v = *v * -1;
  } else if (*dim < 0) {
    *dim = *dim * -1;
    *v = *v * -1;
  }
}

void edgeCheck(int i) {
  fixDim(&pos[i].x, &velocity[i].x);
  fixDim(&pos[i].y, &velocity[i].y);
  fixDim(&pos[i].z, &velocity[i].z);
}
    
float fabs(float f) {
  if (f > 0) return f;
  return (f * -1);
}
    
void moveDots() {
  for (int i=0; i<NUM_DOTS; i++) {
    pos[i].x += velocity[i].x;
    pos[i].y += velocity[i].y;
    pos[i].z += velocity[i].z;
    edgeCheck(i);
  }
}

void drawDots() {
  for (int i=0; i<NUM_DOTS; i++) 
    cube.setVoxel(pos[i], clr[i]);
}

void loop() {
  if (millis() - lastSwap > CYCLE_INTERVAL) initDots();
    
  cube.background(black);

  applyForces();
  moveDots();
  drawDots();
  
  cube.show(); 
}