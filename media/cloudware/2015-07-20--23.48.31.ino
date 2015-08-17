
#include "beta-cube-library.h"

char* vizName="             Mood";
int vizId=665;
#include <math.h>

#define BRIGHTNESS_MAX 0.5
#define COLOR_CHANGE_TIME 10.0
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
  
  cubeColor[0] = random(0, 256) / 256.0;
  cubeColor[1] = random(0, 256) / 256.0;
  cubeColor[2] = random(0, 256) / 256.0;
  
  changeColorDir();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void changeColorDir() {
  cubeColorDir[0] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED+1) / 1000.0;
  cubeColorDir[1] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED+1) / 1000.0;
  cubeColorDir[2] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED+1) / 1000.0;
  colorTimer = COLOR_CHANGE_TIME;
}

void loop() {
  unsigned long curTime = millis();
  float deltaSecs = (curTime - lastTime) / 1000.0;

  cubeColor[0] += cubeColorDir[0] * deltaSecs;
  cubeColor[1] += cubeColorDir[1] * deltaSecs;
  cubeColor[2] += cubeColorDir[2] * deltaSecs;
  
  //cubeColor[0] = min(1.0, max(0.0, cubeColor[0]));
  //cubeColor[1] = min(1.0, max(0.0, cubeColor[1]));
  //cubeColor[2] = min(1.0, max(0.0, cubeColor[2]));
  
  Color col(cubeColor[0] * 256, cubeColor[1] * 256, cubeColor[2] * 256);
  cube.setVoxel(7,7,7,col);
  cube.show();
  
  colorTimer -= deltaSecs;
  if (colorTimer <= 0.0)
  {
    changeColorDir();
  }
  
  lastTime = curTime;
}
