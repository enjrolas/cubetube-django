
#include "beta-cube-library.h"

#include "math.h"

Cube cube = Cube(); 

#define CYCLE_INTERVAL 60000 // milliseconds between restart
unsigned long lastSwap;

#define MAX_DOTS 19
#define PI 3.1415926535
#define MIN_RADI 1
#define MAX_RADI 5

Color clr[MAX_DOTS];
float angle[MAX_DOTS];
float radi[MAX_DOTS];
float y[MAX_DOTS];
int arcs = 180; 
Point center = { 4.5, 4.5, 4.5 };

void setup() { 
  cube.begin();
  srand(millis());
  
  initDots();
}

void initDots() {
  lastSwap = millis();
  
  for (int i=0; i<MAX_DOTS; i++) {
    y[i] = random(cube.size);
    radi[i] = random(MIN_RADI,MAX_RADI) + randomDecimal();
    angle[i] = randomDecimal() * 2 * PI;
    //clr[i] = Color(rand()%16, rand()%16, rand()%16);
    randomColor(&clr[i]);
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

float randomDecimal() {
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float radius(float x, float y, float z) {
  return sqrt(pow(center.x - x, 2) + pow(center.z - z, 2));
}

void moveDots() {
  for (int i=0; i<MAX_DOTS; i++) {
    angle[i] += 2 * PI / arcs; 
    if (angle[i] > 2 * PI) angle[i] -= 2*PI;
    
    radi[i] += randomDecimal() / 200;
    y[i]    += randomDecimal() / 100;
    
    if (y[i] > cube.size || radi[i] > MAX_RADI) {
      y[i] = 0;
      radi[i] = MIN_RADI;
    }
  }
}

void drawDots() {
  for (int i=0; i<MAX_DOTS; i++) {
    cube.setVoxel(center.x + radi[i] * cos(angle[i]), y[i], center.z + radi[i] * sin(angle[i]), clr[i]);
  }
}

void loop() {
    
  if (millis() - lastSwap > CYCLE_INTERVAL) initDots();
      
  cube.background(black);
  
  for (int i=0; i<MAX_DOTS; i++) {
    drawDots();
    
    moveDots();
  }

  cube.show(); 

}