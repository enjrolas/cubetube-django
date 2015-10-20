
#include "beta-cube-library.h"

#include "beta-cube-library.h"
#include <math.h>

Cube cube = Cube();
Color A = Color(0,0,255);   // blue
Color T = Color(255,255,0); // yellow
Color G = Color(0,255,0);   // green
Color C = Color(255,0,0);   // red
Color mC = Color(255,0,255);
Color base;

int maxColumns = 0;

String strands = {"TTTTTTTTTTACTAGcATCGACTAGCATCGACTACG", "TTTTTTTTTGGACTGCACGAGCACAGCACTGCCCGAGCTGACGCGATGCGATCGCTAGCTACG", "TTTTTTTTTGACGCGGATAGACGCGATCCGACTAGCGACTGCTAGTGAGTGAG", "TTTTTTTTTTACTACAGACTATCGACTACGGAGCTAGCGGCATCTGAGTACGCGCGCATGAATACGT"};
int offsets = {0,0,0,0};
int x;
int y;
int z;

void setup()
{
  maxColumns = cube.size*cube.size;
  for (int i = 0; i < maxColumns; i++) {
    offsets[i] = 1;
    strands[i] = strands[i] ? strands[i] : "";
  }
  cube.begin();
}

void loop()
{
  cube.background(black);

  for (int column = 0; column < maxColumns; column++) {
    x = column / cube.size;
    z = column % cube.size;

    for (y = 0; y < cube.size; y++) {
      char c = strands[column].charAt(y + offsets[column]);
      base = Color(0,0,0);
      if(c == "A") {
        base = A;
      } else if (c == "T") {
        base = T;
      } else if (c == "G") {
        base = G;
      } else if (c == "C") {
        base = C;
      } else if (c == "c") {
        base = mC;
      }
      cube.setVoxel(x,y,z, base);
      offsets[column] ++;
      if(offsets[column] > strands[column].length()) {
        offsets[column] = -10;
      }
    }
      for (int i = 0; i < 20000; i++) { } // portable delay
  }

  cube.show();  //send the data to the cube
//  delay(5);  // Stops the program for 250 milliseconds
}
