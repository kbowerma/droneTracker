/* Kyle Bowerman 12.29.2016
*
*/
#define MYVERSION "0.08.0102"
#define FILENAME "trackerthree"

bool serialDebug = true;


float lat, lon, movingRatio;
int sats;
int page = 5, pubCount = 0, holdDownTimer = 300;
double clat, clon, mph, mps, alt,  hdop;
double speedThreshold = 7.5;
unsigned long age, lastPub, nextPub;
unsigned long isMoving = 1, isStill = 1;
String myName = " not set";
String mongoid;


//myprototypes declartions
void gpsDispatch();
void oledDispatch(int);
int setPage(String command);
int setSpThr(String command);
int setHDT(String command);
void oled0();
void oled1();
void oled2();
void oled3();
void oled4();
void oled5();
void oledInit();
void getMyName();
void nextPage();
void testPub();
void goPub ();
