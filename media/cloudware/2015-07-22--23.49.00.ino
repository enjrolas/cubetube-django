
#include "beta-cube-library.h"

#include <math.h>

#define BRIGHTNESS_MAX 0.5
#define COLOR_CHANGE_TIME 8.0
#define MAX_COLOR_SPEED 500

//SYSTEM_MODE(SEMI_AUTOMATIC);

class Entity
{
public:
  float position[3];
  float velocity[3];
  float size;
  float bounds;
  
  void init(float speed, float size, float bounds)
  {
    this->size = size;
    this->bounds = bounds;
    
    position[0] = (random(100) / 100.0) * this->bounds;
    position[1] = (random(100) / 100.0) * this->bounds;
    position[2] = (random(100) / 100.0) * this->bounds;
    
    velocity[0] = random(100) / 100.0;
    velocity[1] = random(100) / 100.0;
    velocity[2] = random(100) / 100.0;
    
    float mag = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
    velocity[0] = (velocity[0] / mag) * speed;
    velocity[1] = (velocity[1] / mag) * speed;
    velocity[2] = (velocity[2] / mag) * speed;
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
    }
    else if (position[0] >= upperBounds && velocity[0] > 0)
    {
      position[0] = upperBounds - (position[0] - upperBounds);
      velocity[0] = -velocity[0];
    }
    
    if (position[1] <= 0 && velocity[1] < 0)
    {
      position[1] = -position[1];
      velocity[1] = -velocity[1];
    }
    else if (position[1] >= upperBounds && velocity[1] > 0)
    {
      position[1] = upperBounds - (position[1] - upperBounds);
      velocity[1] = -velocity[1];
    }
  
    if (position[2] <= 0 && velocity[2] < 0)
    {
      position[2] = -position[2];
      velocity[2] = -velocity[2];
    }
    else if (position[2] >= upperBounds && velocity[2] > 0)
    {
      position[2] = upperBounds - (position[2] - upperBounds);
      velocity[2] = -velocity[2];
    }
  }
};

Cube cube;
Entity boxColor;
Entity box;
unsigned long lastTime = 0;
bool wifiOn = true;

void updateWifiButton();

void setup()
{
  int seed = analogRead(1);
  randomSeed(seed);
  
  boxColor.init(0.7, 1.0, 1.0);
  box.init(30.0, 2.0, 8.0);

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
  
  lastTime = curTime;
  delay(100);
}

//float clamp(float val, float lower, float upper) {
//  return min(upper, max(lower, val));
//}

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


