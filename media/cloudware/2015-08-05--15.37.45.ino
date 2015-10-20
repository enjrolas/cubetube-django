
#include "beta-cube-library.h"

Cube cube = Cube();

int topLeftVoxel[3];
int direction[3] = { 1, 1, 0};
int bounds [3] = { 2, 2, 2};
int frame;
int delayTime;
bool collided;

Color cubeColor;


void loop() {
  cube.background(black);  //clear the cube for each frame
  collided = false;

  topLeftVoxel[0] += direction[0];
  topLeftVoxel[1] += direction[1];
  topLeftVoxel[2] += direction[2];

  if (topLeftVoxel[0] < 0 || topLeftVoxel[0] > 6) {
    // Reverse direction
    topLeftVoxel[0] -= 2*direction[0];
    direction[0] = -direction[0];
    collided = true;
  }

  if (topLeftVoxel[1] < 0 || topLeftVoxel[1] > 6) {
    // Reverse direction
    topLeftVoxel[1] -= 2*direction[1];
    direction[1] = -direction[1];
    collided = true;
  }

  if (topLeftVoxel[2] < 0 || topLeftVoxel[2] > 6) {
    // Reverse direction
    topLeftVoxel[2] -= 2*direction[2];
    direction[2] = -direction[2];
    collided = true;
  }

  if (collided) {
    cubeColor = Color(random(0, 255), random(0, 255), random(0, 255));
  }

  frame++;
  if (frame % 100 == 0) {
    while (true) {
      direction[0] = random(-1, 1);
      direction[1] = random(-1, 1);
      direction[2] = random(-1, 1);
      if (!(direction[0] == 0 && direction[1] == 0 && direction[2] == 0)) {
        break;
      }
    }
    delayTime = random(1, 100);
  }
  delay(delayTime);
  
  for (int i=topLeftVoxel[0]; i<topLeftVoxel[0]+bounds[0]; i++) {
    for (int j=topLeftVoxel[1]; j<topLeftVoxel[1]+bounds[1]; j++) {
      for (int k=topLeftVoxel[2]; k<topLeftVoxel[2]+bounds[2]; k++) {
        cube.setVoxel(i, j, k, cubeColor);
      }
    }
  }
  cube.show();
}


void setup() {
  cube.begin();
  randomSeed(analogRead(0));
  cube.background(black);
  topLeftVoxel[0] = random(0, 8);
  topLeftVoxel[1] = random(0, 8);
  topLeftVoxel[2] = random(0, 8);
  cubeColor = Color(random(0, 255), random(0, 255), random(0, 255));
  frame = 0;
  delayTime = random(1, 100);
  cube.show();
}