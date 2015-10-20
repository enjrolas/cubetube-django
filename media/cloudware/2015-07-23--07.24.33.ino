
#include "beta-cube-library.h"

#include <math.h>

#define SPEED 10
/******************************
 * function definitions
 * ***************************/
Cube cube = Cube();

void add(Point& a, Point& b);
void rotate_x(Point& a, int b);
int frame=0;
Color voxelColor;
int maxBrightness=50;




void setup() {
    cube.begin();
}


void loop() {
    cube.background(black);
	puckdude();
    cube.show();
}
void add(Point& a, Point& b)
{
    a.x+=b.x;
    a.y+=b.y;
    a.z+=b.z;
}

void puckdude() {
	int spritesize = 65;
	Point puckdude[spritesize],
		  ghost[spritesize],
		  ghostface[spritesize],
		  ghosteye[spritesize];

	for(int i=spritesize-1;i>0;i--){
	    puckdude[i]={-1,-1,-1};
	    ghost[i]={-1,-1,-1};
	    ghostface[i]={-1,-1,-1};
	    ghosteye[i]={-1,-1,-1};
    }

	ghost[ 1]={3,6,0};
	ghost[ 2]={4,6,0};
	ghost[ 3]={5,6,0};
	ghost[ 4]={2,5,0};
	ghost[ 5]={3,5,0};
	ghost[ 6]={4,5,0};
	ghost[ 7]={5,5,0};
	ghost[ 8]={6,5,0};
	ghost[ 9]={1,4,0};
	ghost[10]={2,4,0};
	ghost[11]={3,4,0};
	ghost[12]={4,4,0};
	ghost[13]={5,4,0};
	ghost[14]={6,4,0};
	ghost[15]={7,4,0};
	ghost[16]={1,3,0};
	ghost[17]={2,3,0};
	ghost[18]={3,3,0};
	ghost[19]={4,3,0};
	ghost[20]={5,3,0};
	ghost[21]={6,3,0};
	ghost[22]={7,3,0};
	ghost[23]={1,2,0};
	ghost[24]={2,2,0};
	ghost[25]={3,2,0};
	ghost[26]={4,2,0};
	ghost[27]={5,2,0};
	ghost[28]={6,2,0};
	ghost[29]={7,2,0};

	ghosteye[1]={3,5,0};
	ghosteye[2]={5,5,0};
	ghosteye[3]={3,4,0};
	ghosteye[4]={5,4,0};
        
    ghostface[1]={2,2,0};
    ghostface[2]={3,3,0};
    ghostface[3]={4,2,0};
    ghostface[4]={5,3,0};
    ghostface[5]={6,2,0};
    ghostface[6]={5,5,0};
    ghostface[7]={3,5,0};
    
    
    puckdude[ 1]={4,6,7};
    puckdude[ 2]={3,6,7};
    puckdude[ 3]={2,6,7};
    puckdude[ 4]={4,5,7};
    puckdude[ 5]={3,5,7};
    puckdude[ 6]={2,5,7};
    puckdude[ 7]={1,5,7};
    puckdude[ 8]={3,4,7};
    puckdude[ 9]={2,4,7};
    puckdude[10]={1,4,7};
    puckdude[11]={0,4,7};
    puckdude[12]={1,3,7};
    puckdude[13]={0,3,7};
    puckdude[14]={3,2,7};
    puckdude[15]={2,2,7};
    puckdude[16]={1,2,7};
    puckdude[17]={0,2,7};
    puckdude[18]={4,1,7};
    puckdude[19]={3,1,7};
    puckdude[20]={2,1,7};
    puckdude[21]={1,1,7};
    puckdude[22]={4,0,7};
    puckdude[23]={3,0,7};
    puckdude[24]={2,0,7};
    puckdude[25]={2,3,7};
    switch(frame%32){
        case 1 ... 15:
			ghost[30]={2,1,0};
			ghost[31]={4,1,0};
			ghost[32]={6,1,0};
			ghost[33]={2,0,0};
			ghost[34]={4,0,0};
			ghost[35]={6,0,0};
    	break;
        default:
        	ghost[30]={1,1,0};
			ghost[31]={3,1,0};
			ghost[32]={5,1,0};
			ghost[33]={7,1,0};
        	ghost[34]={1,0,0};
			ghost[35]={3,0,0};
			ghost[36]={5,0,0};
			ghost[37]={7,0,0};
        break;
    }
    switch(frame%32){
        case 0:	case 31:
        puckdude[37]={6,3,7};
        case 1:	case 30:
        case 2:	case 29:
        puckdude[26]={5,3,7};
        case 3:	case 28:
        puckdude[27]={6,2,7};
        puckdude[28]={6,4,7};
        case 4:	case 27:
        puckdude[29]={4,3,7};
        case 5:	case 26:
        case 6:	case 25:
        puckdude[30]={5,4,7};
        puckdude[31]={5,2,7};
        case 7:	case 24:
        puckdude[32]={5,5,7};
        puckdude[33]={4,4,7};
        puckdude[34]={3,3,7};
        puckdude[35]={4,2,7};
        puckdude[36]={5,1,7};
        default:
        break;
    }

    Color blinky = {50,0,0};
    Color pinky = {50,9,46};
    Color inky = {0,0,50};
    Color clyde = {50,30,0};
    Color ghost_white = {50,50,50};
    Color ghost_blue = {19, 18, 42};
    Color ghostface_white = {50,0,0};
    Color ghostface_normal = {50,50,50};
    Color ghostface_blue = {50, 41, 28};
    Color ghost1 = ghost_white;
    Color ghostface1;
    Color ghost2 = ghost_white;
    Color ghostface2;
    int direction;
    int phase = frame%(100*SPEED);
    if(phase<50*SPEED){
        direction=1;
        ghost1=blinky;
        ghostface1=ghostface_normal;
        ghost2=clyde;
        ghostface2=ghostface_normal;
    }else{
        direction=-1;
        
    	for(int i=spritesize-1;i>0;i--){  // We need to flip the sprites around 
    	    puckdude[i].x=(7-puckdude[i].x); 
    	    ghost[i].x=(7-ghost[i].x);
    	    ghostface[i].x=(7-ghostface[i].x);
    	    ghosteye[i].x=(7-ghosteye[i].x);
        }
        
        
        if((phase/10)%2){
            ghost1=ghost_white;
            ghostface1=ghostface_white;
            ghost2=ghost_white;
            ghostface2=ghostface_white;
        }else{
            ghost1=ghost_blue;
            ghostface1=ghostface_blue;
            ghost2=ghost_blue;
            ghostface2=ghostface_blue;
        }
    }
    int start_delay = 1;
	for(int i=spritesize-1;i>0;i--){
	    
	    if(frame>(start_delay*SPEED)){
	        rotate_x(puckdude[i],frame%(28*SPEED)*direction);
	        rotate_x(puckdude[i],(1*SPEED*direction));// move puckdude ahead of ghosts
	    }
	    if(direction<0){
           rotate_x(puckdude[i],(-5*SPEED));
	    }
        
        rotate_x(puckdude[i],(1*SPEED*direction));// move puckdude ahead of ghosts
		cube.setVoxel(puckdude[i].x,puckdude[i].y,puckdude[i].z,{50,46,0});

	    if(frame>(start_delay*SPEED)){rotate_x(ghosteye[i],frame%(28*SPEED)*direction);}
		cube.setVoxel(ghosteye[i].x,ghosteye[i].y,ghosteye[i].z,ghostface1);
	    rotate_x(ghosteye[i],(8*SPEED*direction));
	    rotate_x(ghosteye[i],(1*SPEED*direction));//makes red look forward
		cube.setVoxel(ghosteye[i].x,ghosteye[i].y,ghosteye[i].z,ghostface2);
		
	 //   if(frame>(50*SPEED)){rotate_x(ghostface[i],frame%(28*SPEED));}
	////	cube.setVoxel(ghostface[i].x,ghostface[i].y,ghostface[i].z,{50,30,50});
	 //   rotate_x(ghostface[i],(8*SPEED));
	//	cube.setVoxel(ghostface[i].x,ghostface[i].y,ghostface[i].z,{50,50,50 });

	    if(frame>(start_delay*SPEED)){rotate_x(ghost[i],frame%(28*SPEED)*direction);}
		cube.setVoxel(ghost[i].x,ghost[i].y,ghost[i].z,ghost1);
	    rotate_x(ghost[i],(8*SPEED*direction));
		cube.setVoxel(ghost[i].x,ghost[i].y,ghost[i].z,ghost2);
	}
	frame++;
	
}

void rotate_x(Point& a, int b)
{
    if(b>0){
        for(int i=abs(b)/SPEED;i>0;i--){
            if(a.z==7){
               if(a.x<7)
                    a.x+=1;
                else 
                    a.z-=1;
            }else if(a.x==7){
                if(a.z>0)
                    a.z-=1;
                else 
                    a.x-=1;
            }else if(a.z==0){
               if(a.x>0)
                    a.x-=1;
                else 
                    a.z+=1;
            }else if(a.x==0){
                if(a.z<7)
                    a.z+=1;
                else 
                    a.x-=1;
            }
        }
    }else{
        for(int i=abs(b)/SPEED;i>0;i--){
            if(a.z==7){
               if(a.x>0)
                    a.x-=1;
                else
                    a.z-=1;
            }else if(a.x==7){
                if(a.z<7)
                    a.z+=1;
                else 
                    a.x+=1;
            }else if(a.z==0){
               if(a.x<7)
                    a.x+=1;
                else
                   a.z-=1;
            }else if(a.x==0){
                if(a.z>0)
                    a.z-=1;
                else 
                    a.x+=1;
            }
        }
    }
}