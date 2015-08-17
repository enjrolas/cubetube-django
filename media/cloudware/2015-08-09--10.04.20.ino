
#include "beta-cube-library.h"

Cube cube=Cube();

int minColorVal = 0;
int maxColorVal = 500;

double colorscale = 75.0/255.0; // scale color brightness

Color start = Color(random(colorscale), random(colorscale), random(colorscale));
Color end = Color(random(colorscale), random(colorscale), random(colorscale));

int value = 0;

void setup()
{
  cube.begin();
  
  cube.background(start);
  cube.show();
}

void newColor()
{
	start = end;
    end = Color(random(colorscale), random(colorscale), random(colorscale));
}

void loop()
{
  if(value > maxColorVal)
  {
    value = minColorVal;
    newColor();
  }
  
  //cube.background(cube.lerpColor(start, end, value, min, max));
  Color mapColor = cube.colorMap(value, minColorVal, maxColorVal);
  mapColor.red = mapColor.red * colorscale;
  mapColor.blue = mapColor.blue * colorscale;
  mapColor.green = mapColor.green * colorscale;
  
  cube.background(mapColor);
  
  cube.show();
  //delay(10);   //wait 0.01 seconds before advancing to the next voxel
  
  value++;
}

