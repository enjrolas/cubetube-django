
#include "beta-cube-library.h"

char* vizName="fastSphere";
int vizId=615;
Cube cube();
void sphere(Point center, float radius, Color col);

void setup()
{
  cube.begin();
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
  sphere(Point(3.5,3.5,3.5), 3,Color(255,0,0));
  cube.show();
}

	void sphere(Point center, float radius, Color col) {
		float resolution = 30;
		for (float m = 0; m < resolution; m++)
			for (float n = 0; n < resolution; n++)
				setVoxel(
						center.x + radius * sin(PI * m / resolution)
								* cos(PI * n / resolution),
						center.y + radius * sin(PI * m / resolution)
								* sin(PI * n / resolution),
						center.z + radius * cos(PI * m / resolution),
						col);
	}