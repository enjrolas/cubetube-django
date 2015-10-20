
#include "beta-cube-library.h"

char* vizName="FFTJoy";
int vizId=635;
#include <math.h>


#define MICROPHONE 12
#define GAIN_CONTROL 11
#define MAX_POINTS 20
#define SPEED 0.22


//maxBrightness is the brightness limit for each pixel.  All color data will be scaled down 
//so that the largest value is maxBrightness
int maxBrightness=50;


/*********************************
 * FFTJoy variables *
 * *******************************/
#define M 4
float real[(int)pow(2,M)];
float imaginary[(int)pow(2,M)];
float max=0;
float sample;

int frameCount=0;

Cube cube=Cube();

void FFTJoy();
short FFT(short int dir,int m,float *x,float *y);

void setup() {
 cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop() {
  	FFTJoy();
	frameCount++;
    cube.show();
}


/********************************************
 *   FFT JOY functions
 * *****************************************/
 void FFTJoy(){
    for(int i=0;i<pow(2,M);i++)
    {
        real[i]=analogRead(MICROPHONE)-2048;
        delayMicroseconds(212);  //this sets our 'sample rate'.  I went through a bunch of trial and error to 
                                //find a good sample rate to put a soprano's vocal range in the spectrum of the cube
        imaginary[i]=0;
    }
    FFT(1, M, real, imaginary);
    for(int i=0;i<pow(2,M);i++)
    {
        imaginary[i]=sqrt(pow(imaginary[i],2)+pow(real[i],2));
        if(imaginary[i]>max)
            max=imaginary[i];
    }
    if(max>100)
        max--;
    for(int i=0;i<pow(2,M)/2;i++)
    {
        imaginary[i]=cube.size*imaginary[i]/max;
        int y;
        for(y=0;y<=imaginary[i];y++)
            cube.setVoxel(i,y,cube.size-1,cube.colorMap(y,0,cube.size));
        for(;y<cube.size;y++)
            cube.setVoxel(i,y,cube.size-1,black);
    }
    for(int z=0;z<cube.size-1;z++)
        for(int x=0;x<cube.size;x++)
            for(int y=0;y<cube.size;y++)
            {
                Color col=cube.getVoxel(x,y,z+1);
                cube.setVoxel(x,y,z,col);
            }

    sample++;
    if(sample>=pow(2,M))
        sample-=pow(2,M);
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
