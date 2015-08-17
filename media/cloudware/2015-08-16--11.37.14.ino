
#include "beta-cube-library.h"

Cube cube = Cube(); 

#define CYCLE_INTERVAL 2000 // milliseconds between restart
unsigned long lastSwap;

#define BOUNCE_DAMP .9

#define NUM_DOTS 3
Point pos[NUM_DOTS];
Point velocity[NUM_DOTS];
Color clr[NUM_DOTS];

#define MAX_ACCEL 3
int accelerometer[MAX_ACCEL];
#define FACEPLANT 2300
#define UPSIDE_DOWN 1850
#define RIGHTSIDE_UP 2400
#define LEFT_SIDE 1800
#define RIGHT_SIDE 2400

float gravfactor = 0.01;
Point gravity = { 0, gravfactor * -1, 0 }; 
float damp = .0001; 

void setup() {
  cube.begin();
  srand(millis());
  Serial.begin(9600);
  Serial.print("setup");
  initDots();
}

#include <stdarg.h>

#define PRINTF_BUFFER_SIZE 128
void Serial_printf(const char* fmt, ...) {
   char buff[PRINTF_BUFFER_SIZE];
   va_list args;
   va_start(args, fmt);
   vsnprintf(buff, PRINTF_BUFFER_SIZE, fmt, args);
   va_end(args);
   Serial.println(buff);
}

float randInitialVelocity() {
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX) / 7;
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

void initDots() {
  lastSwap = millis();
  int locus = random(cube.size);
  for (int i=0; i<NUM_DOTS; i++) {
    pos[i] = { locus, random(cube.size/2, cube.size), locus };
    velocity[i] = { randInitialVelocity(), randInitialVelocity(), randInitialVelocity() };
    randomColor(&clr[i]);
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
    *v = *v * BOUNCE_DAMP * -1;
  } else if (*dim < 0) {
    *dim = *dim * -1;
    *v = *v *BOUNCE_DAMP * -1;
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

int lastFlip = 23;
void flipGravity() {
  lastSwap = millis();
  int r;
  do {
    r = random(6);
  } while (r == lastFlip);
  
  switch (r) {
    case 0:
      gravity = { 0, gravfactor, 0 };
      break;
    case 1:
      gravity = { gravfactor * -1, 0, 0 };
      break;
    case 2:
      gravity = { gravfactor , 0, 0 };
      break;
    case 3:
      gravity = { 0, gravfactor * -1, 0 };
      break;
    case 4:
      gravity = { 0, 0, gravfactor * -1 };
      break;
    case 5:
      gravity = { 0, 0, gravfactor };
      break;
  }
  lastFlip = r;
}
 
void updateAccelerometer() {
  
}

void loop() {
    
  //updateAccelerometer();
  
  //if (millis() - lastSwap > CYCLE_INTERVAL) flipGravity();
    
  cube.background(black);

  applyForces();
  moveDots();
  drawDots();
  
  cube.show(); 
  delay(1); 
}