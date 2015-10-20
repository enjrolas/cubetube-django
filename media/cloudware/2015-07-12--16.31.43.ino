
#include "beta-cube-library.h"

char* vizName="     Life 3D";
int vizId=577;
int randSeed = 0;

Cube cube = Cube();
Color deadColor = Color(0, 0, 0);
bool operator==(const Color& lhs, const Color& rhs)
{
    return lhs.red == rhs.red && lhs.green == rhs.green && lhs.blue == rhs.blue;
}
bool operator!=(const Color& lhs, const Color& rhs)
{
    return lhs.red != rhs.red || lhs.green != rhs.green || lhs.blue != rhs.blue;
}

void resetCube() {
  cube.background(deadColor);
  randSeed = random(0, 100) * analogRead(0);
  randomSeed(randSeed);
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      for (int k=0; k<8; k++) {
        if ((random(1, 100) > 40)) {
          cube.setVoxel(i, j, k, Color((i+1)*255/8,(j+1)*255/8,(k+1)*255/8));
        }
      }
    }
  }
  cube.show();
}

int countNeighbors(int x, int y, int z) {
  int count = 0;

  for (int i=x-1; i<x+2; i++) {
    for (int j=y-1; j<y+2; j++) {
      for (int k=z-1; k<z+2; k++) {
        if ((i >= 0 && j >= 0 && k >= 0) && (i<=7 && j<=7 && k<=7) && !(i == x && j==y && k==z) && cube.getVoxel(i,j,k) != deadColor) {
          count++;
        }
      }
    }
  }

  return count;
}

void setup() {
  cube.begin();
  resetCube();
  cube.show();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}


void loop() {  
  int changeCount = 0;
  int neighbourCount = 0;
  bool aliveNow;
  bool aliveNext;

  bool liveMap[8][8][8];

  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      for (int k=0; k<8; k++) {
        neighbourCount = countNeighbors(i, j, k);
        aliveNow = cube.getVoxel(i, j, k) != deadColor;
        aliveNext = (aliveNow && neighbourCount > 2 && neighbourCount < 8) || (!aliveNow && neighbourCount == 5);
        liveMap[i][j][k] = aliveNext;
        if (aliveNow != aliveNext) {
          changeCount++;
        }
      }
    }
  }

  cube.background(deadColor);
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      for (int k=0; k<8; k++) {
        if (liveMap[i][j][k]) {
          cube.setVoxel(i, j, k, Color((i+1)*255/8,(j+1)*255/8,(k+1)*255/8));
        } else {
          cube.setVoxel(i, j, k, deadColor);
        }
      }
    }
  }

  if (changeCount) {
    delay(100);
  } else {
    delay(1000);
    resetCube();
  }
  cube.show();
}
