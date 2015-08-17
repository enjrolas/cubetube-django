
#include "beta-cube-library.h"

#include <math.h>

#define BRIGHTNESS_MAX 0.5
#define COLOR_CHANGE_TIME 8.0
#define MAX_COLOR_SPEED 500

//SYSTEM_MODE(SEMI_AUTOMATIC);

float randFloat()
{
  return random(100) / 100.0;
}

class Entity
{
public:
  float position[3];
  float velocity[3];
  float size;
  float bounds;
  float speed;
  const float motionNoise = 0.05;
  
  void init(float speed, float size, float bounds)
  {
    this->speed = speed;
    this->size = size;
    this->bounds = bounds;
    
    position[0] = randFloat() * this->bounds;
    position[1] = randFloat() * this->bounds;
    position[2] = randFloat() * this->bounds;
    
    velocity[0] = randFloat();
    velocity[1] = randFloat();
    velocity[2] = randFloat();
    
    normaliseSpeed();
  }

  void update(float deltaSecs)
  {
    position[0] += velocity[0] * deltaSecs;
    position[1] += velocity[1] * deltaSecs;
    position[2] += velocity[2] * deltaSecs;
    
    float upperBounds = (bounds + 1) - size;
    
    if (position[0] <= 0 && velocity[0] < 0)
    {
      position[0] = -position[0];
      velocity[0] = -velocity[0];
      noiseifyVelocity(1);
      noiseifyVelocity(2);
      normaliseSpeed();
    }
    else if (position[0] >= upperBounds && velocity[0] > 0)
    {
      position[0] = upperBounds - (position[0] - upperBounds);
      velocity[0] = -velocity[0];
      noiseifyVelocity(1);
      noiseifyVelocity(2);
      normaliseSpeed();
    }
    
    if (position[1] <= 0 && velocity[1] < 0)
    {
      position[1] = -position[1];
      velocity[1] = -velocity[1];
      noiseifyVelocity(0);
      noiseifyVelocity(2);
      normaliseSpeed();
    }
    else if (position[1] >= upperBounds && velocity[1] > 0)
    {
      position[1] = upperBounds - (position[1] - upperBounds);
      velocity[1] = -velocity[1];
      noiseifyVelocity(0);
      noiseifyVelocity(2);
      normaliseSpeed();
    }
  
    if (position[2] <= 0 && velocity[2] < 0)
    {
      position[2] = -position[2];
      velocity[2] = -velocity[2];
      noiseifyVelocity(0);
      noiseifyVelocity(1);
      normaliseSpeed();
    }
    else if (position[2] >= upperBounds && velocity[2] > 0)
    {
      position[2] = upperBounds - (position[2] - upperBounds);
      velocity[2] = -velocity[2];
      noiseifyVelocity(0);
      noiseifyVelocity(1);
      normaliseSpeed();
    }
  }

  void normaliseSpeed()
  {
    float mag = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
    if (mag > 0)
    {
      velocity[0] = (velocity[0] / mag) * speed;
      velocity[1] = (velocity[1] / mag) * speed;
      velocity[2] = (velocity[2] / mag) * speed;
    }
    else
    {
      velocity[0] = randFloat();
      velocity[1] = randFloat();
      velocity[2] = randFloat();
      normaliseSpeed();
    }
  }

  void noiseifyVelocity(int axis)
  {
    float noise = (randFloat() * 2 - 1.0) * motionNoise;
    velocity[axis] += noise * speed;
  }
};

Cube cube;
Entity boxColor;
Entity box;
unsigned long lastTime = 0;
bool wifiOn = true;

#define PANIC_TEST_TIME 60
#define PANIC_DURATION 5
#define PANIC_MOVESPEED 0.0
#define PANIC_COLORSPEED 8.0
#define NORMAL_MOVESPEED 15.0
#define NORMAL_COLORSPEED 0.1
#define PANIC_CHANCE 30

float panicTestTime = PANIC_TEST_TIME;
float panicTime = 0;

void updateWifiButton();

void setup()
{
  int seed = analogRead(1);
  randomSeed(seed);
  
  boxColor.init(NORMAL_COLORSPEED, 1.0, 1.0);
  box.init(NORMAL_MOVESPEED, 2.0, 8.0);

  initWifi();
  cube.begin();

  lastTime = millis();
}

void loop()
{
  unsigned long curTime = millis();
  float deltaSecs = (curTime - lastTime) / 1000.0;

  updateWifiButton();
  boxColor.update(deltaSecs);
  box.update(deltaSecs);
  
  cube.background(Color(0,0,0));

  Color currentBoxColor(boxColor.position[0] * 255,
                        boxColor.position[1] * 255,
                        boxColor.position[2] * 255);
  
  for (int sx = 0; sx < box.size; sx++)
  {
    for (int sy = 0; sy < box.size; sy++)
    {
      for (int sz = 0; sz < box.size; sz++)
      {
        cube.setVoxel(box.position[0] + sx,
                      box.position[1] + sy,
                      box.position[2] + sz,
                      currentBoxColor);
      }
    }
  }
  
  cube.show();
  
  if (panicTime <= 0)
  {
    panicTestTime -= deltaSecs;
    if (panicTestTime <= 0)
    {
      int test = random(100);
      if (test < PANIC_CHANCE)
      {
        panicTime = PANIC_DURATION;
        boxColor.speed = PANIC_COLORSPEED;
        box.speed = PANIC_MOVESPEED;
        boxColor.normaliseSpeed();
        box.normaliseSpeed();
      }
      panicTestTime = PANIC_TEST_TIME;
    }
  }
  else
  {
    panicTime -= deltaSecs;
    if (panicTime <= 0)
    {
      boxColor.speed = NORMAL_COLORSPEED;
      box.speed = NORMAL_MOVESPEED;
      boxColor.normaliseSpeed();
      box.normaliseSpeed();
      panicTestTime = PANIC_TEST_TIME;
    }
  }
  
  lastTime = curTime;
  delay(100);
}

void initWifi()
{
  // deliberately set it to the opposite of the switch state to force a change
  wifiOn = !digitalRead(D2);
  updateWifiButton();
}

void updateWifiButton()
{
  bool buttonSwitched = !digitalRead(D2);
  if (!wifiOn && buttonSwitched) {
    WiFi.on();
    Spark.connect();
    wifiOn = true;
  }
  else if (wifiOn && !buttonSwitched) {
    Spark.disconnect();
    WiFi.off();
    wifiOn = false;
  }
}


