
#include "beta-cube-library.h"

// FFT_Meteors_Rainbow
// Werner Moecke - 2015-07-21

// FFT code lifted from original L3D demo
// Find it interesting? There's plenty of
// room for tweaking it even further.
// * Hint - Check the variables section right below...  ;-|

#include <math.h>


#define MICROPHONE 12
#define GAIN_CONTROL 11
#define MAX_POINTS 20
#define SPEED 0.22
#define SIDE 8
#define TRAIL_LENGTH 50


//maxBrightness is the brightness limit for each pixel.  All color data will be scaled down 
//so that the largest value is maxBrightness
int maxBrightness=80;

/*********************************
 * function definitions
 *********************************/
//void  _background(Color col);
Color _colorMap(float _val, float _min, float _max);
Color _lerpColor(Color a, Color b, int _val, int _min, int _max);

/*********************************
 * FFTJoy variables *
 * *******************************/
#define M 4             //if the M value changes, then the 'ARRAY_SIZE' constant also needs
#define ARRAY_SIZE 16   //to be changed to reflect the result of the formula: pow(2,M)
float real[ARRAY_SIZE];       //[(int)pow(2,M)]
float imaginary[ARRAY_SIZE];  //[(int)pow(2,M)]
float maxVal=8;
Color _black;
int   inputLevel=63;	//this sets the sensitivity for the onboard AGC circuit (0-255); the higher, the more sensitive
bool  smoothLEDs=true;	//switch for 'smoothening-out' the LEDs fade-to-black on the spectrum
bool  dotMode=true;		//switch to control the peak indication over each strip (bar/dot)
//float sample;

//int frameCount=0;

Cube cube=Cube();

void FFTJoy();
short FFT(short int dir,int m,float *x,float *y);

void setup() {
    _black.red=0;
    _black.green=0;
    _black.blue=0;
  
	cube.begin();
	initMicrophone();
}

void loop() {
	//do our thingy
  	_background(_black);
  	FFTJoy();
	//frameCount++;
    cube.show();
}


/********************************************
 *   FFT JOY functions
 * *****************************************/
 void FFTJoy(){
    for(int i=0;i<pow(2,M);i++)
    {
        real[i]=analogRead(MICROPHONE)-2048;
        delayMicroseconds(120);  //*210* this sets our 'sample rate'.  I went through a bunch of trial and error to 
                                 //find a good sample rate to put a soprano's vocal range in the spectrum of the cube
								 //
      							 //*120* gets the entire range from bass to soprano within the cube's spectrum
      							 //From bass to soprano: https://youtu.be/6jqCuE7C3rg
        imaginary[i]=0;
    }
    FFT(1, M, real, imaginary);
    for(int i=0;i<pow(2,M);i++)
    {
        imaginary[i]=sqrt(pow(imaginary[i],2)+pow(real[i],2));
        if(imaginary[i]>maxVal)
            maxVal=imaginary[i];
    }
    //if(maxVal>100)
    //    maxVal--;
    for(int i=0;i<pow(2,M)/2;i++)
    {
        imaginary[i]=cube.size*imaginary[i]/maxVal;
        Color pixelColor;
        int y;

		for(y=0;y<=imaginary[i];y++){
          	pixelColor=_colorMap(y,0,cube.size);
            cube.setVoxel(i,y,cube.size-1,pixelColor);
            
			if(dotMode){
                pixelColor=_black;
                if(smoothLEDs){
                    pixelColor=cube.getVoxel(i, (y>0 ? y-1 : 1), cube.size-1);
                    //gotta fade the pixels *much* faster in 'dot mode', so that the human eye can spot the 'trailing' effect
                    int dimCoeff= TRAIL_LENGTH+(maxBrightness/(y+1));
                    pixelColor.red= (pixelColor.red>dimCoeff) ? pixelColor.red-dimCoeff : 0;
                    pixelColor.green= (pixelColor.green>dimCoeff) ? pixelColor.green-dimCoeff : 0;
                    pixelColor.blue= (pixelColor.blue>dimCoeff) ? pixelColor.blue-dimCoeff : 0;
                }
				cube.setVoxel(i, (y>0 ? y-1 : 1), cube.size-1, pixelColor);
          		//delayMicroseconds(30);  //introducing a little bit of delay to 'smoothen-out' transitions
            }
		}
		for(;y<cube.size;y++){
            pixelColor=_black;
            if(smoothLEDs){
                pixelColor=cube.getVoxel(i, y, cube.size-1);
                int dimCoeff= maxBrightness/(y+i);
                pixelColor.red= (pixelColor.red>dimCoeff) ? pixelColor.red-dimCoeff : 0;
                pixelColor.green= (pixelColor.green>dimCoeff) ? pixelColor.green-dimCoeff : 0;
                pixelColor.blue= (pixelColor.blue>dimCoeff) ? pixelColor.blue-dimCoeff : 0;
                
              	//pixelColor.red=pixelColor.red*(cube.size-y)/(cube.size-1);
                //pixelColor.green=pixelColor.green*(cube.size-y)/(cube.size-1);
                //pixelColor.blue=pixelColor.blue*(cube.size-y)/(cube.size-1);
            }
            cube.setVoxel(i, y, cube.size-1, pixelColor);
          	//delayMicroseconds(30);  //introducing a little bit of delay to 'smoothen-out' transitions
		}
    }
    for(int z=0;z<cube.size-1;z++)
        for(int x=0;x<cube.size;x++)
            for(int y=0;y<cube.size;y++)
            {
                Color trailColor=cube.getVoxel(x, y, z+1);
                if(smoothLEDs){
                    //fade the trail to black over the length of the cube's z-axis
                    trailColor.red=trailColor.red*(z+2)/(SIDE-1);
                    trailColor.green=trailColor.green*(z+2)/(SIDE-1);
                    trailColor.blue=trailColor.blue*(z+2)/(SIDE-1);

                  	//trailColor.red=trailColor.red*(z+(cube.size-z))/(cube.size-1);
                    //trailColor.green=trailColor.green*(z+(cube.size-z))/(cube.size-1);
                    //trailColor.blue=trailColor.blue*(z+(cube.size-z))/(cube.size-1);
                }
                cube.setVoxel(x, y, z, trailColor);
                delayMicroseconds(20);  //introducing a little bit of delay to 'smoothen-out' transitions
                
				//Color col=cube.getVoxel(x,y,z+1);
                //cube.setVoxel(x,y,z,col);
            }
	
	maxVal= (maxVal>=120) ? maxVal-2 : (maxVal<8) ? 8 : maxVal-0.8;
    //sample++;
    //if(sample>=pow(2,M))
    //    sample-=pow(2,M);
}

void initMicrophone()
{
  pinMode(GAIN_CONTROL, OUTPUT);
  analogWrite(GAIN_CONTROL, inputLevel);  //digitalWrite(GAIN_CONTROL, LOW);
}

short FFT(short int dir,int m,float *x,float *y)
{
   int n,i,i1,j,k,i2,l,l1,l2;
   float c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   n = 1;
   for (i=0;i<m;i++) 
      n *= 2;

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;
   for (i=0;i<n-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1; 
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1) 
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<n;i++) {
         x[i] /= n;
         y[i] /= n;
      }
   }

   return(0);
}

void _background(Color col)
{
    for(int x=0;x<cube.size;x++)
        for(int y=0;y<cube.size;y++)
            for(int z=0;z<cube.size;z++)
                cube.setVoxel(x,y,z, col);    
}

//returns a color from a set of colors fading from blue to green to red and back again
//the color is returned based on where the parameter *val* falls between the parameters
//*min* and *max*.  If *val* is min, the function returns a blue color.  If *val* is halfway
//between *min* and *max*, the function returns a yellow color.  
Color _colorMap(float _val, float _min, float _max)
{
  float _range=1024;
  _val=_range*(_val-_min)/(_max-_min);
  Color colors[6];
  
  colors[0].red=0;
  colors[0].green=0;
  colors[0].blue=maxBrightness;
  
  colors[1].red=0;
  colors[1].green=maxBrightness;
  colors[1].blue=maxBrightness;
  
  colors[2].red=0;
  colors[2].green=maxBrightness;
  colors[2].blue=0;
  
  colors[3].red=maxBrightness;
  colors[3].green=maxBrightness;
  colors[3].blue=0;
  
  colors[4].red=maxBrightness;
  colors[4].green=0;
  colors[4].blue=0;

  colors[5].red=maxBrightness;
  colors[5].green=0;
  colors[5].blue=maxBrightness;
  
  if (_val<=_range/6)
    return(_lerpColor(colors[0], colors[1], _val, 0, _range/6));
  else if (_val<=2*_range/6)
    return(_lerpColor(colors[1], colors[2], _val, _range/6, 2*_range/6));
  else if (_val<=3*_range/6)
    return(_lerpColor(colors[2], colors[3], _val, 2*_range/6, 3*_range/6));
  else if (_val<=4*_range/6)
    return(_lerpColor(colors[3], colors[4], _val, 3*_range/6, 4*_range/6));
  else if (_val<=5*_range/6)
    return(_lerpColor(colors[4], colors[5], _val, 4*_range/6, 5*_range/6));
  else
    return(_lerpColor(colors[5], colors[0], _val, 5*_range/6, _range));
}

//returns a color that's an interpolation between colors a and b.  The color
//is controlled by the position of val relative to min and max -- if val is equal to min,
//the resulting color is identical to color a.  If it's equal to max, the resulting color 
//is identical to color b.  If val is (max-min)/2, the resulting color is the average of
//color a and color b
Color _lerpColor(Color a, Color b, int _val, int _min, int _max)
{
    Color lerped;
    lerped.red=a.red+(b.red-a.red)*(_val-_min)/(_max-_min);
    lerped.green=a.green+(b.green-a.green)*(_val-_min)/(_max-_min);
    lerped.blue=a.blue+(b.blue-a.blue)*(_val-_min)/(_max-_min);
    return lerped;
}