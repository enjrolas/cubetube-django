
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

  int n = c % 8;
  for(int i=0; i<=n; i++){
  	for(int j=0; j<=n; j++){
  		for(int k=0; k<=n; k++){
  			cube.setVoxel(i, j, n, blue);
  			cube.setVoxel(i, n, k, blue);
  			cube.setVoxel(n, j, k, blue);
  		}
    }
  }
  
  cube.show();
  
  frame++;
  if(frame % 10 == 0)
    c++;
}