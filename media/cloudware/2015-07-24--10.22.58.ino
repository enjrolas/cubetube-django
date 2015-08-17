
#include "beta-cube-library.h"

#include <math.h>
Cube cube = Cube();

#define VOX_POINTS 64

void add(Point& a, Point& b);

//maxBrightness is the brightness limit for each pixel.  All color data will be scaled down 
//so that the largest value is maxBrightness
int maxBrightness = 50;

/********************************
 * zplasma variables *
 * *****************************/
float phase = 0.0;
float phaseIncrement = 0.0052; // Controls the speed of the moving points. Higher == faster
float colorStretch = 0.1; // Higher numbers will produce tighter color bands 
float plasmaBrightness = 0.15;
//float phaseIncrement = 0.035; // Controls the speed of the moving points. Higher == faster
//float colorStretch = 0.23; // Higher numbers will produce tighter color bands 
//float plasmaBrightness = 0.2;
Color plasmaColor;

void zPlasma() {

    phase += phaseIncrement;
    // The two points move along Lissajious curves, see: http://en.wikipedia.org/wiki/Lissajous_curve
    // We want values that fit the LED grid: x values between 0..8, y values between 0..8, z values between 0...8
    // The sin() function returns values in the range of -1.0..1.0, so scale these to our desired ranges.
    // The phase value is multiplied by various constants; I chose these semi-randomly, to produce a nice motion.
    Point p1 = {(sin(phase * 1.000) + 1.0) * 4, (sin(phase * 1.310) + 1.0) * 4.0, (sin(phase * 1.380) + 1.0) * 4.0};
    Point p2 = {(sin(phase * 1.770) + 1.0) * 4, (sin(phase * 2.865) + 1.0) * 4.0, (sin(phase * 1.410) + 1.0) * 4.0};
    Point p3 = {(sin(phase * 0.250) + 1.0) * 4, (sin(phase * 0.750) + 1.0) * 4.0, (sin(phase * 0.380) + 1.0) * 4.0};
}

int voxelXw1[VOX_POINTS];
int voxelZw1[VOX_POINTS];
int voxelXw2[VOX_POINTS];
int voxelZw2[VOX_POINTS];
int voxelXw3[VOX_POINTS];
int voxelZw3[VOX_POINTS];
int voxelXw4[VOX_POINTS];
int voxelZw4[VOX_POINTS];
int voxDelay(150);
int wave01(7);
int wave02(10);
int wave03(15);
int wave04(19);
Color brightLine01 = color_1;
Color brightLine02 = color_1;
Color brightLine03 = color_1;
Color brightLine04 = color_1;
Color brightLine05 = color_1;
Color brightLine06 = color_1;
Color medLine01 = Color(20, 158, 18);
Color medLine02 = Color(41, 114, 41);
Color medLine03 = Color(5, 45, 6);
Color medLine04 = Color(6, 25, 3);
Color medLine05 = Color(8, 15, 3);
Color darkLine01 = Color(10, 70, 10);
Color darkLine02 = Color(5, 55, 4);
Color darkLine03 = Color(3, 30, 4);
Color darkLine04 = Color(2, 15, 1);
Color darkLine05 = Color(1, 8, 1);

void setup() {
  cube.begin();
  for(int i=8;i>0;i--)
    {
      voxelXw1[i]=rand()%8;
      voxelZw1[i]=rand()%8;
      voxelXw2[i]=rand()%8;
      voxelZw2[i]=rand()%8;
      voxelXw3[i]=rand()%8;
      voxelZw3[i]=rand()%8;
      voxelXw4[i]=rand()%8;
      voxelZw4[i]=rand()%8;
    }
}

void loop() {

  if (wave01>-10) {
    for(int i=8;i>0;i--)
      {
        cube.setVoxel(voxelXw1[i], wave01+9, voxelZw1[i], black);
        cube.setVoxel(voxelXw1[i], wave01+8, voxelZw1[i], brightLine06);
        cube.setVoxel(voxelXw1[i], wave01+7, voxelZw1[i], brightLine05);
        cube.setVoxel(voxelXw1[i], wave01+6, voxelZw1[i], brightLine04);
        cube.setVoxel(voxelXw1[i], wave01+5, voxelZw1[i], brightLine04);
        cube.setVoxel(voxelXw1[i], wave01+4, voxelZw1[i], brightLine03);
        cube.setVoxel(voxelXw1[i], wave01+3, voxelZw1[i], brightLine03);
        cube.setVoxel(voxelXw1[i], wave01+2, voxelZw1[i], brightLine02);
        cube.setVoxel(voxelXw1[i], wave01+1, voxelZw1[i], brightLine02);
        cube.setVoxel(voxelXw1[i], wave01, voxelZw1[i], brightLine01);
      }
    wave01--;
  }

  else {
    wave01=7;
    for(int i=8;i>0;i--)
      {
        voxelXw1[i]=rand()%8;
        voxelZw1[i]=rand()%8;
      }
  }

  if (wave02>-10) {
    for(int i=8;i>0;i--)
      {
        cube.setVoxel(voxelXw2[i], wave02+9, voxelZw2[i], black);
        cube.setVoxel(voxelXw2[i], wave02+8, voxelZw2[i], medLine05);
        cube.setVoxel(voxelXw2[i], wave02+7, voxelZw2[i], medLine04);
        cube.setVoxel(voxelXw2[i], wave02+6, voxelZw2[i], medLine04);
        cube.setVoxel(voxelXw2[i], wave02+5, voxelZw2[i], medLine03);
        cube.setVoxel(voxelXw2[i], wave02+4, voxelZw2[i], medLine03);
        cube.setVoxel(voxelXw2[i], wave02+3, voxelZw2[i], medLine02);
        cube.setVoxel(voxelXw2[i], wave02+2, voxelZw2[i], medLine02);
        cube.setVoxel(voxelXw2[i], wave02+1, voxelZw2[i], medLine02);
        cube.setVoxel(voxelXw2[i], wave02, voxelZw2[i], medLine01);
      }
    wave02--;
  }

  else {
    wave02=7;
    for(int i=8;i>0;i--)
      {
        voxelXw2[i]=rand()%8;
        voxelZw2[i]=rand()%8;
      }
  }

  if (wave03>-10) {
    for(int i=8;i>0;i--)
      {
        cube.setVoxel(voxelXw3[i], wave03+9, voxelZw3[i], black);
        cube.setVoxel(voxelXw3[i], wave03+8, voxelZw3[i], darkLine05);
        cube.setVoxel(voxelXw3[i], wave03+7, voxelZw3[i], darkLine04);
        cube.setVoxel(voxelXw3[i], wave03+6, voxelZw3[i], darkLine04);
        cube.setVoxel(voxelXw3[i], wave03+5, voxelZw3[i], darkLine03);
        cube.setVoxel(voxelXw3[i], wave03+4, voxelZw3[i], darkLine03);
        cube.setVoxel(voxelXw3[i], wave03+3, voxelZw3[i], darkLine02);
        cube.setVoxel(voxelXw3[i], wave03+2, voxelZw3[i], darkLine02);
        cube.setVoxel(voxelXw3[i], wave03+1, voxelZw3[i], darkLine02);
        cube.setVoxel(voxelXw3[i], wave03, voxelZw3[i], darkLine01);
      }
    wave03--;
  }

  else {
    wave03=7;
    for(int i=8;i>0;i--)
      {
        voxelXw3[i]=rand()%8;
        voxelZw3[i]=rand()%8;
      }

  }
  if (wave04>-10) {
    for(int i=8;i>0;i--)
      {
        cube.setVoxel(voxelXw4[i], wave04+9, voxelZw4[i], black);
        cube.setVoxel(voxelXw4[i], wave04+8, voxelZw4[i], medLine05);
        cube.setVoxel(voxelXw4[i], wave04+7, voxelZw4[i], medLine04);
        cube.setVoxel(voxelXw4[i], wave04+6, voxelZw4[i], medLine04);
        cube.setVoxel(voxelXw4[i], wave04+5, voxelZw4[i], medLine03);
        cube.setVoxel(voxelXw4[i], wave04+4, voxelZw4[i], medLine03);
        cube.setVoxel(voxelXw4[i], wave04+3, voxelZw4[i], medLine02);
        cube.setVoxel(voxelXw4[i], wave04+2, voxelZw4[i], medLine02);
        cube.setVoxel(voxelXw4[i], wave04+1, voxelZw4[i], medLine02);
        cube.setVoxel(voxelXw4[i], wave04, voxelZw4[i], medLine01);
      }
    wave04--;
  }

  else {
    wave04=7;
    for(int i=8;i>0;i--)
      {
        voxelXw4[i]=rand()%8;
        voxelZw4[i]=rand()%8;
      }
  }

  cube.show();
  delay(voxDelay);

}