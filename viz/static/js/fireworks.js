L3D cube;
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
    size(500,500, P3D); 
    cube=new L3D();
    newFirework();
    smooth();
}

void draw()
{
    background(0);
    translate(width/2, height/2);
    rotateY(mouseX/50);
    rotateX(mouseY/50);
    updateFirework();
    cube.draw();
}

void updateFirework()
{
    cube.background(0);
    if (popped)
	{
	    radius+=radInc;
	    pos+=posInc;
	    if ((pos<=0)||(pos>=cube.side))
		posInc*=-1;
	    if (radius>maxRadius)
		newFirework();
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
    center=new PVector(random(cube.side), random(cube.side), random(cube.side));
    maxRadius=3+random(cube.side);
    radInc=random(0.05, 0.15);
    radius=0;
    popped=false;
    launchSpeed=random(10, 100);
    launchPoint=new PVector(random(cube.side), 0, random(cube.side));
    rocket=launchPoint;
    increment=PVector.sub(center, launchPoint);    
    increment.div(launchSpeed);
}
