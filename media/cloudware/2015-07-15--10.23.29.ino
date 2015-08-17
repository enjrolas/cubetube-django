
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
  Color clr = Color(0, 0, 1);
  int s = cube.size;
  cube.setVoxel(rand()%s,rand()%s,rand()%s,clr);
  
  for (int i = 0; i <= 5; i++) { // clear some pixels
	  cube.setVoxel(rand()%s,rand()%s,rand()%s,black);
  }
  
  cube.show(); 
  delay(100+rand()%100);
}


