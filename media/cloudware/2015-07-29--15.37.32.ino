
#include "beta-cube-library.h"

Cube _cube = Cube();
int _state = 0;
int _brightness = 50;

float _colorVal = 0.0;
int _t = 20;

void setup()
{
  _cube.begin();
}

void doline(int state, int z, Color c)
{
	int side = state / 7 % 4;

	int x, y;
	int x1, y1;
	switch (state % 7)
	{
	case 0:
		x = 1;
		y = 1;
		break;
	case 1:
		x = 0;
		y = 1;
		break;
	case 2:
		x = 0;
		y = 2;
		break;
	case 3:
		x = 0;
		y = 3;
		break;
	case 4:
		x = 0;
		y = 4;
		break;
	case 5:
		x = 0;
		y = 5;
		break;
	case 6:
		x = 0;
		y = 6;
		break;
	}

	switch (side)
	{
	case 0:
		x1 = x;
		y1 = y;
		break;
	case 1:
		x1 = y;
		y1 = 7 - x;
		break;
	case 2:
		x1 = 7 - x;
		y1 = 7 - y;
		break;
	case 3:
		x1 = 7 - y;
		y1 = x;
		break;
	}

	_cube.line(x1, y1, z, 7 - x1, 7 - y1, z, c);
}

// x: 0 = links, 7 = rechts
// y: 0 = unten, 7 = oben
// z: 0 = hinten, 7 = vorne
// (links/rechts, unten/oben, hinten/vorne)
void loop()
{
	_cube.background(black);  //clear the cube for each frame

	_colorVal += 0.01;

	if (_colorVal > 1.0)
	{
		_colorVal = 0.0;
	}

	for (int z = 0; z < 8; z++)
	{
		float val = _colorVal + 0.03 * z;
		if (val > 1.0)
		{
			val -= 1.0;
		}

		doline(_state + z * 2, z, _cube.colorMap(val, 0.0, 1.0));
	}
	_state++;

	_cube.show();  //send the data to the cube
	delay(_t);
}


