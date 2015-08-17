
#include "beta-cube-library.h"

Cube cube = Cube();
int frame = 0;
int c = 0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  cube.background(black);

  for(int i=0; i<8; i++){
  	for(int j=0; j<8; j++){
  		for(int k=0; k<8; k++){
  			cube.setVoxel(i, j, c, blue);
  			cube.setVoxel(i, c, k, blue);
  			cube.setVoxel(c, j, k, blue);
  		}
    }
  }
  
  cube.show();
  
  frame++;
  if(frame % 10 == 0)
    c++;
}