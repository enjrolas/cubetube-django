
#include "beta-cube-library.h"

Cube cube;
color col;
PVector[][][] voxels;
PVector sphereCenter = new PVector(3.5, 3.5, 3.5);
float sphereRadius = 1;
float aliasAmount;

String acessToken="54ff70066672524820351167";
String coreName="CEDCUBE_2";



void setup()
{
  cube=new Cube(this,acessToken);
  cube.enableDrawing();
  cube.streamToCore(coreName);
  cube.enablePosecube();
  voxels = new PVector[8][8][8];
    aliasAmount = .75;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 8; k++) {
        voxels[i][j][k] = new PVector(i, j, k);
      }
    }
  }
}

void draw()
{
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 8; k++) {
        float myDist = sdSphere(voxels[i][j][k], sphereRadius);
        float redVal = map(myDist, 1, 2, 255, 0);
        float greenVal = map(myDist, 0, 1, 255, 0);
        redVal = smoothstep(0, 255, redVal);
        greenVal = smoothstep(0, 255, greenVal);
        cube.setVoxel(voxels[i][j][k], color(redVal, greenVal, 0));
      }
    }
  }
  cube.draw();
}

/* Lots of notes came frm this guy:
http://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

And some shadertoy stuff
*/
float sdSphere(PVector p, float radius) {
  float t = - (float) millis() / 1000;
  PVector fbmSeed = PVector.mult(p, .2);
  fbmSeed.add(t, t, t);
  float noisyVal = noise(fbmSeed.x, fbmSeed.y, fbmSeed.z) * 20;
  PVector noisyVec = new PVector(noisyVal, noisyVal, noisyVal);
  PVector p2 = p.get();
  p2.add(noisyVec);
  p2.sub(9,9,9);
  return p2.dist(sphereCenter) - radius;
}

float smoothstep(float minV, float maxV, float variance) {
  float lin = max(0,min(1,(variance-minV)/(maxV-minV)));
  return minV + lin*lin*(3-2*lin) * (maxV - minV);
}