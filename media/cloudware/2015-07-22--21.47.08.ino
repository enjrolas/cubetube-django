
#include "beta-cube-library.h"

Cube cube;
color col;
int frame=0;
Point puckdude2[8][8];
void setup()
{
  cube=new Cube(this);
  puckdude = new PVector[8][8];
  ghost = new PVector[8][8];
  ghosteye = new PVector[8][8];
  ghostface = new PVector[8][8];
    aliasAmount = .75;
  
  //voxels[i][j] = new PVector(i, j, 0);


  
  
  
  
}

void draw()
{
  
  
  cube.background(color(0,0,0));
    
for (int i = 0; i < 8; i++) {
  for (int j = 0; j < 8; j++) {
      puckdude[i][j] = new PVector(-1,-1,-1);
      ghost[i][j] = new PVector(-1,-1,-1);
      ghosteye[i][j] = new PVector(-1,-1,-1);
      ghostface[i][j] = new PVector(-1,-1,-1);
  }
}

  
  
puckdude[4][6] = new PVector(4,6,7);
puckdude[3][6] = new PVector(3,6,7);
puckdude[2][6] = new PVector(2,6,7);
puckdude[4][5] = new PVector(4,5,7);
puckdude[3][5] = new PVector(3,5,7);
puckdude[2][5] = new PVector(2,5,7);
puckdude[1][5] = new PVector(1,5,7);
puckdude[3][4] = new PVector(3,4,7);
puckdude[2][4] = new PVector(2,4,7);
puckdude[1][4] = new PVector(1,4,7);
puckdude[0][4] = new PVector(0,4,7);
puckdude[1][3] = new PVector(1,3,7);
puckdude[0][3] = new PVector(0,3,7);
puckdude[3][2] = new PVector(3,2,7);
puckdude[2][2] = new PVector(2,2,7);
puckdude[1][2] = new PVector(1,2,7);
puckdude[0][2] = new PVector(0,2,7);
puckdude[4][1] = new PVector(4,1,7);
puckdude[3][1] = new PVector(3,1,7);
puckdude[2][1] = new PVector(2,1,7);
puckdude[1][1] = new PVector(1,1,7);
puckdude[4][0] = new PVector(4,0,7);
puckdude[3][0] = new PVector(3,0,7);
puckdude[2][0] = new PVector(2,0,7);
puckdude[2][3] = new PVector(2,3,7);
  

	ghost[3][6] = new PVector(3,6,0);
	ghost[4][6] = new PVector(4,6,0);
	ghost[5][6] = new PVector(5,6,0);
	ghost[2][5] = new PVector(2,5,0);
	ghost[3][5] = new PVector(3,5,0);
	ghost[4][5] = new PVector(4,5,0);
	ghost[5][5] = new PVector(5,5,0);
	ghost[6][5] = new PVector(6,5,0);
	ghost[1][4] = new PVector(1,4,0);
	ghost[2][4] = new PVector(2,4,0);
	ghost[3][4] = new PVector(3,4,0);
	ghost[4][4] = new PVector(4,4,0);
	ghost[5][4] = new PVector(5,4,0);
	ghost[6][4] = new PVector(6,4,0);
	ghost[7][4] = new PVector(7,4,0);
	ghost[1][3] = new PVector(1,3,0);
	ghost[2][3] = new PVector(2,3,0);
	ghost[3][3] = new PVector(3,3,0);
	ghost[4][3] = new PVector(4,3,0);
	ghost[5][3] = new PVector(5,3,0);
	ghost[6][3] = new PVector(6,3,0);
	ghost[7][3] = new PVector(7,3,0);
	ghost[1][2] = new PVector(1,2,0);
	ghost[2][2] = new PVector(2,2,0);
	ghost[3][2] = new PVector(3,2,0);
	ghost[4][2] = new PVector(4,2,0);
	ghost[5][2] = new PVector(5,2,0);
	ghost[6][2] = new PVector(6,2,0);
	ghost[7][2] = new PVector(7,2,0);

	ghosteye[3][5] = new PVector(3,5,0);
	ghosteye[5][5] = new PVector(5,5,0);
	ghosteye[3][4] = new PVector(3,4,0);
	ghosteye[5][4] = new PVector(5,4,0);
        
 // ghostface[2][2]=(2,2,0);
 // ghostface[3][3]=(3,3,0);
 // ghostface[4][2]=(4,2,0);
 // ghostface[5][3]=(5,3,0);
 // ghostface[6][2]=(6,2,0);
 // ghostface[5][5]=(5,5,0);
 // ghostface[3][5]=(3,5,0);
    
    
    puckdude[4][6]=new PVector(4,6,7);
    puckdude[3][6]=new PVector(3,6,7);
    puckdude[2][6]=new PVector(2,6,7);
    puckdude[4][5]=new PVector(4,5,7);
    puckdude[3][5]=new PVector(3,5,7);
    puckdude[2][5]=new PVector(2,5,7);
    puckdude[1][5]=new PVector(1,5,7);
    puckdude[3][4]=new PVector(3,4,7);
    puckdude[2][4]=new PVector(2,4,7);
    puckdude[1][4]=new PVector(1,4,7);
    puckdude[0][4]=new PVector(0,4,7);
    puckdude[1][3]=new PVector(1,3,7);
    puckdude[0][3]=new PVector(0,3,7);
    puckdude[3][2]=new PVector(3,2,7);
    puckdude[2][2]=new PVector(2,2,7);
    puckdude[1][2]=new PVector(1,2,7);
    puckdude[0][2]=new PVector(0,2,7);
    puckdude[4][1]=new PVector(4,1,7);
    puckdude[3][1]=new PVector(3,1,7);
    puckdude[2][1]=new PVector(2,1,7);
    puckdude[1][1]=new PVector(1,1,7);
    puckdude[4][0]=new PVector(4,0,7);
    puckdude[3][0]=new PVector(3,0,7);
    puckdude[2][0]=new PVector(2,0,7);
    puckdude[2][3]=new PVector(2,3,7);
  
  
  
    switch(frame%32){
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :
        case 7 :
        case 8 :
        case 9 :
        case 10 :
        case 11 :
        case 12 :
        case 13 :
        case 14 :
        case 15 :
			ghost[2][1]=new PVector(2,1,0);
			ghost[4][1]=new PVector(4,1,0);
			ghost[6][1]=new PVector(6,1,0);
			ghost[2][0]=new PVector(2,0,0);
			ghost[4][0]=new PVector(4,0,0);
			ghost[6][0]=new PVector(6,0,0);
    	break;
        default:
        	ghost[1][1]=new PVector(1,1,0);
			ghost[3][1]=new PVector(3,1,0);
			ghost[5][1]=new PVector(5,1,0);
			ghost[7][1]=new PVector(7,1,0);
        	ghost[1][0]=new PVector(1,0,0);
			ghost[3][0]=new PVector(3,0,0);
			ghost[5][0]=new PVector(5,0,0);
			ghost[7][0]=new PVector(7,0,0);
        break;
    }
  
    switch(frame%32){
        case 0:	case 31:
        puckdude[6][3]=new PVector(6,3,7);
        case 1:	case 30:
        case 2:	case 29:
        puckdude[5][3]=new PVector(5,3,7);
        case 3:	case 28:
        puckdude[6][2]=new PVector(6,2,7);
        puckdude[6][4]=new PVector(6,4,7);
        case 4:	case 27:
        puckdude[4][3]=new PVector(4,3,7);
        case 5:	case 26:
        case 6:	case 25:
        puckdude[5][4]=new PVector(5,4,7);
        puckdude[5][2]=new PVector(5,2,7);
        case 7:	case 24:
        puckdude[5][5]=new PVector(5,5,7);
        puckdude[4][4]=new PVector(4,4,7);
        puckdude[3][3]=new PVector(3,3,7);
        puckdude[4][2]=new PVector(4,2,7);
        puckdude[5][1]=new PVector(5,1,7);
        default:
        break;
    }
  
  
  
  
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
        cube.setVoxel(puckdude[i][j], color(255, 230, 0));
        cube.setVoxel(ghost[i][j], color(255,45, 230));
      //  cube.setVoxel(rotate_x(ghost[i][j],1), color(255,45, 230));
        cube.setVoxel(ghosteye[i][j], color(255, 255, 255));
    }
  }
  cube.draw();
  frame++;
}
