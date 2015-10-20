
#include "beta-cube-library.h"

#include <math.h>


#define MICROPHONE 12
#define GAIN_CONTROL 11
#define MAX_POINTS 20
#define SPEED 0.15




#define PLASMA 0
#define FFT_JOY 1


/********************************
 * zplasma functions *
 * *****************************/
 
void zPlasma()
{
  
phase += phaseIncrement;
// The two points move along Lissajious curves, see: http://en.wikipedia.org/wiki/Lissajous_curve
// We want values that fit the LED grid: x values between 0..8, y values between 0..8, z values between 0...8
// The sin() function returns values in the range of -1.0..1.0, so scale these to our desired ranges.
// The phase value is multiplied by various constants; I chose these semi-randomly, to produce a nice motion.
Point p1 = { (sin(phase*1.000)+1.0) * 4, (sin(phase*1.310)+1.0) * 4.0,  (sin(phase*1.380)+1.0) * 4.0};
Point p2 = { (sin(phase*1.770)+1.0) * 4, (sin(phase*2.865)+1.0) * 4.0,  (sin(phase*1.410)+1.0) * 4.0};
Point p3 = { (sin(phase*0.250)+1.0) * 4, (sin(phase*0.750)+1.0) * 4.0,  (sin(phase*0.380)+1.0) * 4.0};

byte row, col, dep;

// For each row
for(row=0; row<cube.size; row++) {
float row_f = float(row); // Optimization: Keep a floating point value of the row number, instead of recasting it repeatedly.

// For each column
for(col=0; col<cube.size; col++) {
float col_f = float(col); // Optimization.

// For each depth
for(dep=0; dep<cube.size; dep++) {
float dep_f = float(dep); // Optimization.

// Calculate the distance between this LED, and p1.
Point dist1 = { col_f - p1.x, row_f - p1.y,  dep_f - p1.z }; // The vector from p1 to this LED.
float distance1 = sqrt( dist1.x*dist1.x + dist1.y*dist1.y + dist1.z*dist1.z);

// Calculate the distance between this LED, and p2.
Point dist2 = { col_f - p2.x, row_f - p2.y,  dep_f - p2.z}; // The vector from p2 to this LED.
float distance2 = sqrt( dist2.x*dist2.x + dist2.y*dist2.y + dist2.z*dist2.z);

// Calculate the distance between this LED, and p3.
Point dist3 = { col_f - p3.x, row_f - p3.y,  dep_f - p3.z}; // The vector from p3 to this LED.
float distance3 = sqrt( dist3.x*dist3.x + dist3.y*dist3.y + dist3.z*dist3.z);

// Warp the distance with a sin() function. As the distance value increases, the LEDs will get light,dark,light,dark,etc...
// You can use a cos() for slightly different shading, or experiment with other functions.
float color_1 = distance1; // range: 0.0...1.0
float color_2 = distance2;
float color_3 = distance3;
float color_4 = (sin( distance1 * distance2 * colorStretch )) + 2.0 * 0.5;
// Square the color_f value to weight it towards 0. The image will be darker and have higher contrast.
color_1 *= color_1 * color_4;
color_2 *= color_2 * color_4;
color_3 *= color_3 * color_4;
color_4 *= color_4;
// Scale the color up to 0..7 . Max brightness is 7.
//strip.setPixelColor(col + (8 * row), strip.Color(color_4, 0, 0) );
plasmaColor.red=color_1*plasmaBrightness;
plasmaColor.green=color_2*plasmaBrightness;
plasmaColor.blue=color_3*plasmaBrightness;

cube.setVoxel(row,col,dep,plasmaColor);       
}
}
}
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
      //  Serial.print(real[i]);
        imaginary[i]=0;
    }
    FFT(1, M, real, imaginary);
    for(int i=0;i<pow(2,M);i++)
    {
        imaginary[i]=sqrt(pow(imaginary[i],2)+pow(real[i],2));
//        Serial.print(imaginary[i]);
        if(imaginary[i]>maxValue)
            maxValue=imaginary[i];
    }
    if(maxValue>100)
        maxValue--;
//    Serial.println();
    for(int i=0;i<pow(2,M)/2;i++)
    {
        imaginary[i]=cube.size*imaginary[i]/maxValue;
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
//                char output[50];
//                sprintf(output, "%d %d %d:  %d %d %d", x,y,z+1, col.red, col.green, col.blue);
//                Serial.println(output);
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
