
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
    // For each row
    for (row = 0; row < cube.size; row++) {
	float row_f = float(row); // Optimization: Keep a floating point value of the row number, instead of recasting it repeatedly.

	// For each column
	for (col = 0; col < cube.size; col++) {
	    float col_f = float(col); // Optimization.

	    // For each depth
	    for (dep = 0; dep < cube.size; dep++) {
		float dep_f = float(dep); // Optimization.

		// Calculate the distance between this LED, and p1.
		Point dist1 = {col_f - p1.x, row_f - p1.y, dep_f - p1.z}; // The vector from p1 to this LED.
		float distance1 = sqrt(dist1.x * dist1.x + dist1.y * dist1.y + dist1.z * dist1.z);

		// Calculate the distance between this LED, and p2.
		Point dist2 = {col_f - p2.x, row_f - p2.y, dep_f - p2.z}; // The vector from p2 to this LED.
		float distance2 = sqrt(dist2.x * dist2.x + dist2.y * dist2.y + dist2.z * dist2.z);

		// Calculate the distance between this LED, and p3.
		Point dist3 = {col_f - p3.x, row_f - p3.y, dep_f - p3.z}; // The vector from p3 to this LED.
		float distance3 = sqrt(dist3.x * dist3.x + dist3.y * dist3.y + dist3.z * dist3.z);

		// You can use a cos() for slightly different shading, or experiment with other functions.
		float color_1 = distance1; // range: 0.0...1.0
		float color_2 = distance2;
		float color_3 = distance3;
		float color_4 = (sin(distance1 * distance2 * colorStretch)) + 2.0 * 0.5;

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
Color brightLine01 = Color(244, 241, 250);
Color brightLine02 = Color(98, 193, 97);
Color brightLine03 = Color(30, 131, 30);
Color brightLine04 = Color(5, 45, 6);
Color brightLine05 = Color(6, 25, 3);
Color brightLine06 = Color(8, 15, 3);
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