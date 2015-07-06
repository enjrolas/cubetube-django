
#include "beta-cube-library.h"

char* vizName=" photon v core";
int vizId=133;
#if defined(PLATFORM_ID)
#if (PLATFORM_ID == 0) // Core
	#define PROCESSOR "core"
#elif (PLATFORM_ID == 6) // Photon
	#define PROCESSOR "photon"
#else
	#define PROCESSOR "unknown"
#endif
#else
	#define PROCESSOR "platform undefined"
#endif

void setup()
{
	Serial.begin(115200);
Spark.variable("vizName", vizName, STRING);
Spark.variable("vizId", &vizId, INT);
}

void loop()
{
 	Serial.println(PROCESSOR);
}
      