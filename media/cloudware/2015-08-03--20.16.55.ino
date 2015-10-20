
#include "beta-cube-library.h"

Cube cube;
PMatrix3D mat;
PVector [] vertices;
PVector [] tempVertices;
int cubeSize=4;
float aliasAmount;
PVector[][][] voxels;
PVector boxDef;
int frame=0;
void setup()
{
  cube=new Cube(this);
	vertices=new PVector[8];
	tempVertices=new PVector[8];
  boxDef = new PVector(2, 2, 2);
  voxels = new PVector[8][8][8];
  aliasAmount = .75;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      for (int k = 0; k < 8; k++) {
        voxels[i][j][k] = new PVector(i, j, k);
      }
    }
  }
    vertices[0]=new PVector(.5,-.5, -.5);
    vertices[1]=new PVector(.5,-.5, .5);
    vertices[2]=new PVector(-.5,-.5, .5);
    vertices[3]=new PVector(-.5,-.5, -.5);
    vertices[4]=new PVector(.5,.5, -.5);
    vertices[5]=new PVector(.5,.5, .5);
    vertices[6]=new PVector(-.5,.5, .5);
    vertices[7]=new PVector(-.5,.5, -.5);
}

void draw()
{
  cube.background(color(0));
  mat = new PMatrix3D();
  mat.rotateX((float) millis() / 750);
  mat.rotateY((float) millis() / 500);
  mat.rotateZ(0);//(float) millis() / 1000);


  color edgeColor=cube.colorMap(frame%1000, 0, 1000);
  color vertexColor=cube.colorMap((frame+500)%1000,0,1000);
  //now calculate and draw the edges
  for(int i=0;i<8;i++)
    {
        tempVertices[i]=vertices[i].get();
        tempVertices[i].mult(cubeSize);
        tempVertices[i]=mat.mult(tempVertices[i], new PVector());
        tempVertices[i].add(new PVector(3.5, 3.5, 3.5));

    }
	for(int i=0;i<4;i++)
    {
      cube.line(tempVertices[i], tempVertices[i+4], edgeColor);
      int secondIndex=(i+1)%4;
      cube.line(tempVertices[i], tempVertices[secondIndex], edgeColor);
      cube.line(tempVertices[i+4], tempVertices[secondIndex+4],edgeColor);
    }
  for(int i=0;i<8;i++)
    	cube.setVoxel(tempVertices[i], vertexColor);
  cube.draw();
  frame++;
}

float sdBox(PVector p, PVector b) {
  PVector d = new PVector(abs(p.x) - b.x, abs(p.y) - b.y, abs(p.z) - b.z);
  PVector noNegs = new PVector(max(d.x,0.0), max(d.y,0.0), max(d.z,0.0));
  return min(max(d.x,max(d.y,d.z)),0.0) + noNegs.mag();
}

