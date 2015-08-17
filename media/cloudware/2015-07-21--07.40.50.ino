
#include "beta-cube-library.h"

char* vizName="         Snake 3D";
int vizId=488;
#pragma SPARK_NO_PREPROCESSOR
#undef max
#undef min

#include <math.h>
#include <stdint.h>
#include <vector>


Cube cube = Cube();

int deathFrame;
int frameCount;
int initialSnakeLength;
int speed;

struct voxel {
  int x;
  int y;
  int z;

  voxel(int x=0, int y=0, int z=0) 
    : x(x), y(y), z(z)
  {
  };

  bool operator==(const voxel& v) const
  {
      return (v.x == x && v.y == y && v.z == z);
  };

  bool operator!=(const voxel& v) const
  {
      return (v.x != x || v.y != y || v.z != z);
  };

  double distance(const voxel& v) {
    return sqrt(
      pow((v.x - x), 2) +
      pow((v.y - y), 2) +
      pow((v.z - z), 2)
    );
  };
};
    
voxel operator+(const voxel& v1, const voxel& v2) {
  return voxel(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);    
}

std::vector<voxel> snake;
std::vector<voxel> treats;
voxel* snakeDirection;

std::vector<voxel> possibleDirections = {
  { 1,  0,  0},
  {-1,  0,  0},
  { 0,  1,  0},
  { 0, -1,  0},
  { 0,  0,  1},
  { 0,  0, -1}
};

bool containsVoxel(std::vector<voxel> &vectorList, voxel &voxel) {
  for (auto it = vectorList.begin(); it != vectorList.end(); ++it) {
    if (*it == voxel) {
      return true;
    }
  }
  return false;
}

bool canMove(voxel* direction) {
  voxel next = snake[0] + *direction;
  return (next.x >= 0 && next.y >= 0 && next.z >= 0 && next.x <= 7 && next.y <= 7 && next.z <= 7 && !containsVoxel(snake, next));
}

void addTreat() {
  voxel treat;
  while (true) {
    treat = { random(0, 7), random(0, 7), random(0, 7) };
    if (containsVoxel(snake, treat) || containsVoxel(treats, treat)) {
      continue;
    } else {
      treats.push_back(treat);
      break;
    }
  }
}


void updateDirection() {
  // Mostly reuse the same direction but sometimes turn at random
  if (canMove(snakeDirection) && random(0, 100) < 40) {
    return;
  }

  std::vector<voxel*> allowedDirections;
  for(auto it = possibleDirections.begin(); it != possibleDirections.end(); ++it) {
    if (canMove(&(*it))) {
      allowedDirections.push_back(&(*it));
    }
  }

  if (allowedDirections.size() == 0) {
    snakeDirection = NULL;
    return;
  }

  double leastDistance = 65536.0;
  double distance;
  voxel next;
  for(auto it = allowedDirections.begin(); it != allowedDirections.end(); ++it) {
    next = snake[0] + **it;
    distance = next.distance(treats[0]);
    if (distance < leastDistance) {
      leastDistance = distance;
      snakeDirection = *it;
    }
  }
}

void resetCube() {
  cube.background(black);
  snakeDirection = &possibleDirections[0];
  initialSnakeLength = 5;
  snake.clear();
  snake.emplace_back(0, 0, 0);
  speed = 10;
  deathFrame = 0;
  frameCount = 0;

  treats.clear();
  addTreat();
  cube.show();
}

void setup() {
  cube.begin();
  randomSeed(analogRead(0));
  resetCube();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void moveSnake() {
  updateDirection();
  if (snakeDirection == NULL) {
    deathFrame = frameCount;
    return;
  }

  bool grow = (snake.size() < initialSnakeLength);
  voxel front = snake[0] + *snakeDirection;
  for(auto it = treats.begin(); it != treats.end(); ++it) {
    if (*it == front) {
      treats.erase(it);
      grow = true;
      break;
    }
  }

  snake.insert(snake.begin(), front);
  if(!grow) {
    snake.pop_back();
  } 

  if (!treats.size()) {
    addTreat();
  }
}


void loop() {
  Color segmentColor;
  frameCount++;

  if (!deathFrame) {
    moveSnake();
  } else {
    if (frameCount - deathFrame > 100) {
      resetCube();
    }
  }

  cube.background(black);
  for(auto it = snake.begin(); it != snake.end(); ++it) {
    if (deathFrame && frameCount % 16 < 8) {
      segmentColor = Color(255, 255, 255);
    } else if (deathFrame > 0) {
      segmentColor = Color(255, 0, 0);
    } else {
      segmentColor = Color((it->x+1)*255/8, (it->y+1)*255/8, (it->z+1)*255/8);
    }
    cube.setVoxel(it->x, it->y, it->z, segmentColor);
  }

  if (!deathFrame) {
    for(auto it = treats.begin(); it != treats.end(); ++it) {
      cube.setVoxel(it->x, it->y, it->z, Color(150, 255, 0));
    }
  }
  cube.show();
  delay(50);
}
