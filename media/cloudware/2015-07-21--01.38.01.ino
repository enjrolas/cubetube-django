
#include "beta-cube-library.h"

char* vizName="              Mood";
int vizId=665;
#include <math.h>

#define BRIGHTNESS_MAX 0.5
#define COLOR_CHANGE_TIME 8.0
#define MAX_COLOR_SPEED 500

Cube cube;
float cubeColor[3];
float cubeColorDir[3];
float colorTimer;
unsigned long lastTime = 0;

void setup() {
  cube.begin();

  int seed = analogRead(1);
  randomSeed(seed);
  
  cubeColor[0] = random(0, 256) / 255.0;
  cubeColor[1] = random(0, 256) / 255.0;
  cubeColor[2] = random(0, 256) / 255.0;
  
  //cubeColor[0] = 0;
  //cubeColor[1] = 0;
  //cubeColor[2] = 0.01;
  
  changeColorDir();
  
  lastTime = millis();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void blah(int x, int y, int z, int size, int k) {
  //for (int sx = 0; sx < size; sx++) {
  //  for (int sy = 0; sy < size; sy++) {
  //    for (int sz = 0; sz < size; sz++) {
  //      cube.setVoxel(x+sx, y+sy, z+sz, col);
  //    }
  //  }
  //}
}

void changeColorDir() {
  cubeColorDir[0] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED) / 1000.0;
  cubeColorDir[1] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED) / 1000.0;
  cubeColorDir[2] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED) / 1000.0;
  colorTimer = COLOR_CHANGE_TIME;
}

float clamp(float val, float lower, float upper) {
  return min(upper, max(lower, val));
}

void loop() {
  unsigned long curTime = millis();
  float deltaSecs = (curTime - lastTime) / 1000.0;
  deltaSecs = min(0.1, deltaSecs);

  cubeColor[0] += cubeColorDir[0] * deltaSecs;
  cubeColor[1] += cubeColorDir[1] * deltaSecs;
  cubeColor[2] += cubeColorDir[2] * deltaSecs;
  
  int r = clamp(cubeColor[0] * 255.0, 0, 255);
  int g = clamp(cubeColor[1] * 255.0, 0, 255);
  int b = clamp(cubeColor[2] * 255.0, 0, 255);
  Color col(r,g,b);
  //drawCube(cube, 4, 4, 4, 2, col);

  cube.show();
  
  colorTimer -= deltaSecs;
  if (colorTimer <= 0.0)
  {
    changeColorDir();
  }
  
  lastTime = curTime;
}
