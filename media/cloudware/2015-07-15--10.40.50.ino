
#include "beta-cube-library.h"

char* vizName="Blue Mellow";
int vizId=522;
Cube cube = Cube(); 

void setup() { 
  cube.begin();  
  cube.background(black);  
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {
  //Color clr = Color(rand()%2, rand()%3, 1+rand()%8);
  Color clr = Color(0, 0, 10);
  int s = cube.size;
  cube.setVoxel(rand()%s,rand()%s,rand()%s,clr);
  
  /*for (int i = 0; i <= 5; i++) { // clear some pixels
	  cube.setVoxel(rand()%s,rand()%s,rand()%s,black);
  }*/
  
  Color redColour = Color(255,0,0);
  //cube.setVoxel(4, 4, 8, redColour);
  //cube.setVoxel(4, 4, 1, redColour);
  //cube.setVoxel(4, 4, 1, redColour);
  //cube.setVoxel(4, 4, 1, redColour);
  
  cube.sphere(4,4,4,3, redColour);
  
  cube.show(); 
  delay(10+rand() % 100);
}


