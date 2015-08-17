
#include "beta-cube-library.h"

Cube cube = Cube(); 

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() { 
  cube.begin();  
  cube.background(black);  
}

void loop() {
  Color clr = Color(rand()%2, rand()%3, 1+rand()%8);
  int s = cube.size;
  cube.setVoxel(rand()%s,rand()%s,rand()%s,clr);
  for (int i = 0; i <= 5; i++) { // clear some pixels
	  cube.setVoxel(rand()%s,rand()%s,rand()%s,black);
  }
  cube.show(); 
  delay(100+rand()%100);
}


