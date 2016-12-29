/* Kyle Bowerman 12.29.2016
*
*/
#define MYVERSION "0.08.1229"
#define FILENAME "trackerthree"

bool serialDebug = true;


float lat, lon;
int sats;
double clat, clon, mph, mps, alt,  hdop;
unsigned long age;

void gpsDispatch();
