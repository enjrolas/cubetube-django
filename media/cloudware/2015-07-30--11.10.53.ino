
#include "beta-cube-library.h"

#include <math.h>

Cube cube = Cube();
int maxSize = 8;
Color drawColor=Color(255,150,100);
float z;
float dist;
float dim = 1;
float diameter = 3.5;

int maxBrightness=100;
float phase=0;

float PI=3.1415;
float phaseInc=PI/12;
int lastSwap = millis();
int CYCLE_INTERVAL = 1;
int iterations = 0;

// Math constants we'll use
float  pi=3.1415926535897932384626433;	// pi
float  twopi=2.0*pi;			// pi times 2
float  two_over_pi= 2.0/pi;		// 2/pi
double  halfpi=pi/2.0;			// pi divided by 2

float fmod(float a, float b)
{
  return (a - b * floor(a / b));
}


int floor(float x)
{
if (x >= 0)
    {
        return (int)x;
    }
    else
    {
        int y = (int)x;
        return ((float)y == x) ? y : y - 1;
    }
}

float cos_32s(float x)
{
 float c1= 0.99940307;
 float c2=-0.49558072;
 float c3= 0.03679168;

float x2;							// The input argument squared

x2=pow(x, 2);
return (c1 + x2*(c2 + c3 * x2));
}

//
//  This is the main cosine approximation "driver"
// It reduces the input argument's range to [0, pi/2],
// and then calls the approximator. 
// See the notes for an explanation of the range reduction.
//
float cos_32(float x){
	int quad;						// what quadrant are we in?

	x=fmod(x, twopi);				// Get rid of values > 2* pi
	if(x<0)x=-x;					// cos(-x) = cos(x)
	quad=int(x * two_over_pi);			// Get quadrant # (0 to 3) we're in
	switch (quad){
	case 0: return  cos_32s(x);
	case 1: return -cos_32s(pi-x);
	case 2: return -cos_32s(x-pi);
	case 3: return  cos_32s(twopi-x);
	}
}
//
//   The sine is just cosine shifted a half-pi, so
// we'll adjust the argument and call the cosine approximation.
//
float sin_32(float x){
	return cos_32(halfpi-x);
}

float distance(float x, float y, float z, float x1, float y1, float z1)
{
  return(sqrt(pow(x-x1,2)+pow(y-y1,2)+pow(z-z1,2)));
}

float fmap(float input, float inMin, float inMax, float outMin, float outMax)
{
    float out;
    out = (input-inMin)/(inMax-inMin)*(outMax-outMin) + outMin;
    return out;
}

float toFloat(int x)
{
  return float(x);
}

void setup()
{
  cube.begin();
}

void loop()
{
  if (millis() - lastSwap > CYCLE_INTERVAL)
  {
    lastSwap = millis();
    iterations++;
    
  	cube.background(black);  //clear the cube for each frame
   
  phase += phaseInc; 
  int p = int(phase) % 511;
  if (p > 255) 
  {
    p = 511 - p;
  }
  float dilation = fmap(toFloat(p),toFloat(0),toFloat(256),-0.5,1);
    

  for(int x = 0; x < maxSize; x++)
  {
    for(int y = 0; y < maxSize; y++)
    {
      for(int z = 0; z < maxSize; z++)
      {
   //     z = sin_32(phase + sqrt(pow(fmap(toFloat(x),toFloat(0),toFloat(maxSize-1),-PI,PI),2) + pow(fmap(toFloat(y),toFloat(0), toFloat(maxSize-1),-PI,PI),2)));		  
   //     int zm = fmap(z,toFloat(-1),toFloat(1),toFloat(0),toFloat(maxSize-1))+0.5;
      //z = (int(phase)%maxSize + x) /2;
        
        
		
        dist = distance(toFloat(x), toFloat(y), toFloat(z), 3.5, 3.5, 3.5);
        if (dist < diameter + dilation)
        {
          dim = fmap(dist,toFloat(0),toFloat(diameter + dilation),toFloat(0.1),toFloat(0.9));
  //        dim = .7;
  //        if (dist > toFloat(floor(diameter + dilation))) 
  //        {
  //          dim = .1;
 //         }
          cube.setVoxel(x, y, z, Color(int(p * dim),
                                       int(fmap(dist,toFloat(0),toFloat(diameter + dilation),toFloat(0),toFloat(255)) * dim),
                                       int(100 * dim)));
        }
      }
    }
  }
    //cube.setVoxel(iterations%8, iterations/8%8, iterations/64%8, red); 
    cube.show();  //send the data to the cube
  }
  //delay(15); // keep the cube from disconnecting, seems ok from 27-7-2015.
}
