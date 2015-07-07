
#include "beta-cube-library.h"

char* vizName="photon v core";
int vizId=133;
void setup()
{
	Serial.begin(115200)
    /*
	if (PLATFORM_ID ==6)
  		Serial.println("photon");
  	else
      	Serial.println("core");
        */
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
#if defined(PLATFORM)
 	Serial.println(PLATFORM);
#else
 	Serial.println("no");

}
      