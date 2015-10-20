
#include "beta-cube-library.h"

char* vizName="        Mood";
int vizId=665;
#include <math.h>

#define BREATH_SPEED 4000.0
#define BRIGHTNESS_MIN 0.1
#define BRIGHTNESS_MAX 1.0
#define COLOR_CHANGE_TIME 10.0
#define MAX_COLOR_SPEED 10

Cube cube;
float cubeColor[3];
float cubeColorDir[3];
float colorTimer;
unsigned long lastTime = 0;

void setup() {
  cube.begin();
  
  int seed = analogRead(1);
  randomSeed(seed);
  
  cubeColor[0] = random(0, 256);
  cubeColor[1] = random(0, 256);
  cubeColor[2] = random(0, 256);
  
  cubeColorDir[0] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED+1);
  cubeColorDir[1] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED+1);
  cubeColorDir[2] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED+1);
  colorTimer = COLOR_CHANGE_TIME;
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

float getBreathAmount(float time)
{
  return (exp(sin(time / BREATH_SPEED * 3.14)) - 0.367) * 0.4254;
}

void loop() {
  unsigned long curTime = millis();
  float deltaSecs = (curTime - lastTime) / 1000.0;
  float colorFact = getBreathAmount(curTime);
  
  /*
  float sect0Min = 0.100;
  float sect0Max = 0.500;
  float sect0Value = max(0, min(1, (colorFact - sect0Min) / (sect0Max - sect0Min)));
  float colorValue = BRIGHTNESS_MIN + sect0Value * (BRIGHTNESS_MAX - BRIGHTNESS_MIN);
  rgb_t finalColor = RGB(cubeColor[0] * colorValue, cubeColor[1] * colorValue, cubeColor[2] * colorValue);
  cube.setplane(Z, 0, finalColor);
  
  float sect1Min = 0.200;
  float sect1Max = 0.600;
  float sect1Value = max(0, min(1, (colorFact - sect1Min) / (sect1Max - sect1Min)));
  colorValue = BRIGHTNESS_MIN + sect1Value * (BRIGHTNESS_MAX - BRIGHTNESS_MIN);
  finalColor = RGB(cubeColor[0] * colorValue, cubeColor[1] * colorValue, cubeColor[2] * colorValue);
  cube.setplane(Z, 1, finalColor);
  
  float sect2Min = 0.4;
  float sect2Max = 0.8;
  float sect2Value = max(0, min(1, (colorFact - sect2Min) / (sect2Max - sect2Min)));
  colorValue = BRIGHTNESS_MIN + sect2Value * (BRIGHTNESS_MAX - BRIGHTNESS_MIN);
  finalColor = RGB(cubeColor[0] * colorValue, cubeColor[1] * colorValue, cubeColor[2] * colorValue);
  cube.setplane(Z, 2, finalColor);
  
  float sect3Min = 0.600;
  float sect3Max = 1.000;
  float sect3Value = max(0, min(1, (colorFact - sect3Min) / (sect3Max - sect3Min)));
  colorValue = BRIGHTNESS_MIN + sect3Value * (BRIGHTNESS_MAX - BRIGHTNESS_MIN);
  finalColor = RGB(cubeColor[0] * colorValue, cubeColor[1] * colorValue, cubeColor[2] * colorValue);
  cube.setplane(Z, 3, finalColor);
  */

  Color col(cubeColor[0], cubeColor[1], cubeColor[2]);
  cube.background(col);
  cube.show();
  
  //cubeColor[0] += cubeColorDir[0] * deltaSecs;
  //cubeColor[1] += cubeColorDir[1] * deltaSecs;
  //cubeColor[2] += cubeColorDir[2] * deltaSecs;
  
  //cubeColor[0] = min(255, max(0, cubeColor[0]));
  //cubeColor[1] = min(255, max(0, cubeColor[1]));
  //cubeColor[2] = min(255, max(0, cubeColor[2]));
  
  //colorTimer -= deltaSecs;
  //if (colorTimer <= 0.0)
  //{
  //  cubeColorDir[0] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED+1);
  //  cubeColorDir[1] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED+1);
  //  cubeColorDir[2] = random(-MAX_COLOR_SPEED, MAX_COLOR_SPEED+1);
  //  colorTimer = COLOR_CHANGE_TIME;
  //}
  
  lastTime = curTime;
  delay(10);
}
