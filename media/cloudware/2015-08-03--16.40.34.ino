
#include "beta-cube-library.h"

Cube cube = Cube();
int frame = 0;

void setup(){
cube.begin();
cube.background(black);
  
}



void loop(){
cube.background(black);
  for(int i=0;i<=8;i++){
    
cube.setVoxel(0,i-1,0,red);
 
  }
cube.show();
}