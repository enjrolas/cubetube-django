
#include "beta-cube-library.h"

char* vizName="   Tranquility";
int vizId=517;
Cube cube=Cube();

int min = 0;
int max = 500;

double colorscale = 75.0/255.0; // scale color brightness

Color start = Color(random(colorscale), random(colorscale), random(colorscale));
Color end = Color(random(colorscale), random(colorscale), random(colorscale));

int value = 0;

void setup()
{
  cube.begin();
  
  cube.background(start);
  cube.show();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void newColor()
{
	start = end;
    end = Color(random(colorscale), random(colorscale), random(colorscale));
}

void loop()
{
  if(value > max)
  {
    value = 0;
    newColor();
  }
  
  //cube.background(cube.lerpColor(start, end, value, min, max));
  Color mapColor = cube.colorMap(value, min, max);
  mapColor.red = mapColor.red * colorscale;
  mapColor.blue = mapColor.blue * colorscale;
  mapColor.green = mapColor.green * colorscale;
  
  cube.background(mapColor);
  
  cube.show();
  //delay(10);   //wait 0.01 seconds before advancing to the next voxel
  
  value++;
}

