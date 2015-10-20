
#include "beta-cube-library.h"

char* vizName="    Xmas Tree in July";
int vizId=532;
Cube cube = Cube(); 
#define MAX_LIGHTS 15

Color grn = Color(0, 8, 0);
Point light[MAX_LIGHTS];

void setup() { 
  cube.begin();
  srand(millis());
  
  drawTree();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void drawMinLayer(int y) {
  cube.line(3, y, 2, 3, y, 5, grn);
  cube.line(4, y, 2, 4, y, 5, grn);
  cube.line(2, y, 3, 5, y, 3, grn);
  cube.line(2, y, 4, 5, y, 4, grn);
}

void drawMidLayer(int y) {
  drawMinLayer(y);
  cube.line(3, y, 1, 1, y, 3, grn); // nw
  cube.line(4, y, 1, 6, y, 3, grn); // ne
  cube.line(1, y, 4, 3, y, 6, grn); // sw
  cube.line(4, y, 6, 6, y, 4, grn); // se
}

void drawMaxLayer(int y) {
  drawMidLayer(y);
  cube.line(3, y, 0, 0, y, 3, grn); // nw
  cube.line(4, y, 0, 7, y, 3, grn); // ne
  cube.line(0, y, 4, 3, y, 7, grn); // sw
  cube.line(4, y, 7, 7, y, 4, grn); // se
}

void drawTree() {
  drawMinLayer(5);
  drawMidLayer(4);
  drawMaxLayer(3);
  drawMinLayer(2);
  drawMidLayer(1);
  drawMaxLayer(0);
}

void twinkleStar() {
  for (int x=3; x<=4; x++) {
    for (int y=6; y<=7; y++) {
      for (int z=3; z<=4; z++) {
        int bright = rand()%32 + 32;
        Color clr = Color(bright, bright, bright);
        cube.setVoxel(x, y, z, clr);
      }
    }
  }
}

void blinkyLights() {
  for (int i=0; i<MAX_LIGHTS; i++) {
    Color check;
    do {
      light[i] = { rand()%8, rand()%8, rand()%8 };
      check = cube.getVoxel(light[i]);
    } while (check.red != grn.red ||check.green != grn.green || check.blue != grn.blue);
    cube.setVoxel(light[i], Color(rand()%8, rand()%8, rand()%8));
  }
}

void clearLights() {
  for (int i=0; i<MAX_LIGHTS; i++) {
    cube.setVoxel(light[i], grn);
  }
}

void loop() {

  twinkleStar();
  
  blinkyLights();
  
  cube.show(); 
  delay(1000);
  clearLights();
}
