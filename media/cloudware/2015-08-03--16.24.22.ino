
#include "beta-cube-library.h"

Cube cube;
color col;
float radius;
float radInc;
float maxRadius;
PVector center;
float pos=0;
float posInc=.1;
float launchSpeed;
boolean popped;
PVector launchPoint;
PVector increment;
PVector rocket;
void setup()
{
    cube=new Cube(this);
    newFirework();
}

void draw()
{
    updateFirework();
    cube.draw();
}

//launch a new firework whenever a key is pressed.
void keyPressed()
{
  newFirework();
}

void updateFirework()
{
    cube.background(0);
    if (popped)
	{
	    radius+=radInc;
	    pos+=posInc;
	    if ((pos<=0)||(pos>=cube.size))
		posInc*=-1;
      //commenting out the auto-fireworks code
//	    if (radius>maxRadius)
//		newFirework();
	    else
		{
		    cube.sphere(center, radius, col);
		}
	} else
	{

	    cube.sphere(rocket, 1, color(100, 100, 50));
	    rocket.add(increment);
	    if (rocket.dist(center)<1)
		popped=true;
	}
}

void newFirework()
{
    col=color(random(255), random(255), random(255));
    center=new PVector(random(cube.size), random(cube.size), random(cube.size));
    maxRadius=3+random(cube.size);
    radInc=random(0.05, 0.15);
    radius=0;
    popped=false;
    launchSpeed=random(10, 100);
    launchPoint=new PVector(random(cube.size), 0, random(cube.size));
    rocket=launchPoint;
    increment=PVector.sub(center, launchPoint);    
    increment.div(launchSpeed);
}