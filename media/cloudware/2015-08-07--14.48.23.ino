
#include "beta-cube-library.h"

Cube cube = Cube();
int frame=0;

void setup()
{
  cube.begin();
  cube.background(black);
}

void loop()
{
  //cube.background(black);
  //box(0,0,7,7,7,7,white);
  //for(int x = 0; x <= 7; x++)
  //{
    //for(int y = 0; y <= 7; y++)
    //{
      //for(int z = z; zi <= z2; zi++)
     // {
        cube.sphere(3,3,3,2,green);
      //}
    // }
   //}
  //if((frame % 20) > 10)
  //{
	  //cube.setVoxel(3,3,3,blue);
  //}
  //if((millis % 10) == 0)
  //{
    if((frame % 10) == 0) 
      cube.show();
  	frame++;
  //}
}

//void box(int x1,int y1,int z1,int x2,int y2,int z2, Color colI)
//{
       
//}