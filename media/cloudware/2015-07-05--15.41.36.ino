
#include "beta-cube-library.h"

char* vizName="photon v core";
int vizId=133;
void setup()
{
	Serial.begin(115200)
	if (PRODUCT_ID==6)
  		Serial.println("photon");
  	else
      	Serial.println("core");
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
 	Serial.println(PRODUCT_ID);
}
      