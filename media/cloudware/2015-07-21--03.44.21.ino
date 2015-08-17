
#include "beta-cube-library.h"

char* vizName="                       Mood";
int vizId=665;
#include <math.h>

#define BRIGHTNESS_MAX 0.5
#define COLOR_CHANGE_TIME 8.0
#define MAX_COLOR_SPEED 500

Cube cube;
float cubeColor[3];
float cubeColorDir[3];
float cubePosition[3];
float cubeVelocity[3];
float colorTimer;
unsigned long lastTime = 0;

void setup() {
  cube.begin();

  int seed = analogRead(1);
  randomSeed(seed);
  
  cubeColor[0] = random(0, 256) / 255.0;
  cubeColor[1] = random(0, 256) / 255.0;
  cubeColor[2] = random(0, 256) / 255.0;
  
  cubeVelocity[0] = random(80, 120);
  cubeVelocity[1] = random(80, 120);
  cubeVelocity[2] = random(80, 120);
  
  cubePosition[0] = 0;
  cubePosition[1] = 6;
  cubePosition[2] = 6;
  
  changeColorDir();
  
  lastTime = millis();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void drawCube(int x, int y, int z, int size, Color col);
void drawCube(int x, int y, int z, int size, Color col) {
  for (int sx = 0; sx < size; sx++) {
    for (int sy = 0; sy < size; sy++) {
      for (int sz = 0; sz < size; sz++) {
        cube.setVoxel(x+sx, y+sy, z+sz, col);
      }
    }
  }
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

  cube.background(Color(0,0,0));
  updateCube(deltaSecs);
  
  cubeColor[0] += cubeColorDir[0] * deltaSecs;
  cubeColor[1] += cubeColorDir[1] * deltaSecs;
  cubeColor[2] += cubeColorDir[2] * deltaSecs;
  
  int r = clamp(cubeColor[0] * 255.0, 0, 255);
  int g = clamp(cubeColor[1] * 255.0, 0, 255);
  int b = clamp(cubeColor[2] * 255.0, 0, 255);
  Color col(r,g,b);
  drawCube(cubePosition[0], cubePosition[1], cubePosition[2], 2, col);

  cube.show();
  
  colorTimer -= deltaSecs;
  if (colorTimer <= 0.0)
  {
    changeColorDir();
  }
  
  lastTime = curTime;
}

void updateCube(float deltaSecs) {
  cubePosition[0] += cubeVelocity[0] * deltaSecs;
  cubePosition[1] += cubeVelocity[1] * deltaSecs;
  cubePosition[2] += cubeVelocity[2] * deltaSecs;
  
  if (cubePosition[0] <= 0) {
    cubeVelocity[0] = -cubeVelocity[0];
  }
  else if (cubePosition[0] >= 6) {
    cubeVelocity[0] = -cubeVelocity[0];
  }
  
  if (cubePosition[1] <= 0) {
    cubeVelocity[1] = -cubeVelocity[1];
  }
  else if (cubePosition[1] >= 6) {
    cubeVelocity[1] = -cubeVelocity[1];
  }
  
  if (cubePosition[2] <= 0) {
    cubeVelocity[2] = -cubeVelocity[2];
  }
  else if (cubePosition[2] >= 6) {
    cubeVelocity[2] = -cubeVelocity[2];
  }
}

