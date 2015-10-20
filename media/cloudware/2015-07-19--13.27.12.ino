
#include "beta-cube-library.h"

char* vizName="     Color Trees";
int vizId=250;
// Inspiration from trees in game Relativity

Cube cube = Cube();

void setup()
{
  cube.begin();
  cube.background(black);
  
  //Serial.begin(115200);  //initialize the serial port
  cube.show();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

int height = 5;
Color trunkColor = Color(30, 30, 30);
void drawTrunk()
{
  
  for(int i = 0; i < height; i++)
  {
  	cube.setVoxel( 3, i, 3,  trunkColor);
  	cube.setVoxel( 4, i, 3,  trunkColor);
  	cube.setVoxel( 3, i, 4,  trunkColor);
  	cube.setVoxel( 4, i, 4,  trunkColor);
    
  	cube.show();
    delay(100);
  }
	
}

Point treeStart = Point(4, 4, 4);
void connectTuft(Point tuftLoc)
{
  
  /*Serial.println("x: " + String(tuftLoc.x) + 
                 "y: " + String(tuftLoc.y) +
                 "z: " + String(tuftLoc.z));*/
  
  int xDist = (tuftLoc.x - treeStart.x) - 1;
  
  // come up 1 short under to avoid overwriting the first tuft voxel
  int yDist = (tuftLoc.y - treeStart.y) - 1;
  int zDist = (tuftLoc.z - treeStart.z) - 1;
  
  Point branch = treeStart;
  for(int i = 0; i < abs(xDist); i++)
  {
    xDist > 0 ? branch.x++ : branch.x--;
    cube.setVoxel(branch, trunkColor);
    cube.show();
    delay(100);
    //Serial.println("x move: " + String(branch.x));
  }
  
  for(int i = 0; i < abs(zDist); i++)
  {
    zDist > 0 ? branch.z++ : branch.z--;
    cube.setVoxel(branch, trunkColor);
    cube.show();
    delay(100);
    //Serial.println("z move: " + String(branch.z));
  }
  
  for(int i = 0; i < abs(yDist); i++)
  {
    yDist > 0 ? branch.y++ : branch.y--;
    cube.setVoxel(branch, trunkColor);
    cube.show();
    delay(100);
    //Serial.println("y move: " + String(branch.y));
  }
  
}

void drawTuft(Color tuftColor)
{
  int height = random(5) + 3;
  
  int x = random(8);
  int z = random(8);
  
  connectTuft(Point(x, height, z));
  
  for(int i = 0; i < 2; i++)
  {
    cube.setVoxel( x, height + i, z,  tuftColor);
    cube.setVoxel( x + 1, height + i, z,  tuftColor);
    cube.setVoxel( x, height + i, z + 1,  tuftColor);
    cube.setVoxel( x + 1, height + i, z + 1,  tuftColor);
  }
  
  cube.show();
  delay(100);
}

void loop()
{
  cube.background(black);
  
  drawTrunk();
  
  int tuftCount = random(7) + 1;
  
  Color tuftColor = Color(random(255), random(255), random(255));
  for(int i = 0; i < tuftCount; i++)
  {
    drawTuft(tuftColor);
  }
  
  delay(30000);
}