
#include "beta-cube-library.h"

const int DELAY = 200;
// For cube
const int MAX_COLOR = 8;
// For square
//const int MAX_COLOR = 8;
const int INC_STEP = 1;

bool forward = false;
int SquareZ = 7;

Cube cube = Cube();
Color CurrentColor = Color(black);

// Update square Z position
void UpdateSquarePosition()
{
	if(forward)
	{
		SquareZ++;
		if(SquareZ == 7)
			forward = false;
	}
	else
	{
		SquareZ--;
		if(SquareZ == 0)
			forward = true;
	}
}

// Draw square
void Draw()
{
	cube.background(black);
	
	UpdateSquarePosition();
	
	cube.line(0, 7, SquareZ, 7, 7, SquareZ, CurrentColor);
	cube.line(0, 6, SquareZ, 7, 6, SquareZ, CurrentColor);
	cube.line(0, 5, SquareZ, 7, 5, SquareZ, CurrentColor);
	cube.line(0, 4, SquareZ, 7, 4, SquareZ, CurrentColor);
	cube.line(0, 3, SquareZ, 7, 3, SquareZ, CurrentColor);
	cube.line(0, 2, SquareZ, 7, 2, SquareZ, CurrentColor);
	cube.line(0, 1, SquareZ, 7, 1, SquareZ, CurrentColor);
	cube.line(0, 0, SquareZ, 7, 0, SquareZ, CurrentColor);
	//cube.background(CurrentColor);
	cube.show();
	delay(DELAY);
}

void setup()
{
  cube.begin();
}

// Increment the red part of the current color
void IncRedColor()
{
	for(int c = 0; c < MAX_COLOR; c = c + INC_STEP)
	{
		CurrentColor.red = c;
		Draw();
	}
}

// Decrement the red part of the current color
void DecRedColor()
{
	for(int c = MAX_COLOR; c > 0; c = c - INC_STEP)
	{
		CurrentColor.red = c;
		Draw();
	}
}

// Increment the green part of the current color
void IncGreenColor()
{
	for(int c = 0; c < MAX_COLOR; c = c + INC_STEP)
	{
		CurrentColor.green = c;
		Draw();
	}
}

// Decrement the green part of the current color
void DecGreenColor()
{
	for(int c = MAX_COLOR; c > 0; c = c - INC_STEP)
	{
		CurrentColor.green = c;
		Draw();
	}
}

// Increment the blue part of the current color
void IncBlueColor()
{
	for(int c = 0; c < MAX_COLOR; c = c + INC_STEP)
	{
		CurrentColor.blue = c;
		Draw();
	}
}

// Decrement the blue part of the current color
void DecBlueColor()
{
	for(int c = MAX_COLOR; c > 0; c = c - INC_STEP)
	{
		CurrentColor.blue = c;
		Draw();
	}
}

// Up color
void Up()
{
	IncRedColor();
	IncGreenColor();
	DecRedColor();
	IncBlueColor();
	DecGreenColor();
	IncRedColor();
	IncGreenColor();
}

// Down color
void Down()
{
	DecGreenColor();
	DecRedColor();
	IncGreenColor();
	DecBlueColor();
	IncRedColor();
	DecGreenColor();
	DecRedColor();
}

void RandomColor()
{
	int action = random(6);
	
	switch(action) {
		case 0:
			if(CurrentColor.red == 0) IncRedColor();
			break;
		case 1:
			if(CurrentColor.red == MAX_COLOR) DecRedColor();
			break;
		case 2:
			if(CurrentColor.green == 0) IncGreenColor();
			break;
		case 3:
			if(CurrentColor.green == MAX_COLOR) DecGreenColor();
			break;
		case 4:
			if(CurrentColor.blue == 0) IncBlueColor();
			break;
		case 5:
			if(CurrentColor.blue == MAX_COLOR) DecBlueColor();
			break;
	}
}

void loop()
{
	// Set black as starting color
	CurrentColor = Color(black);
	
	Up();
	Down();
	//RandomColor();
}