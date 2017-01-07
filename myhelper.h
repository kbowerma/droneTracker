/* Kyle Bowerman 12.29.2016
*
*/
#define MYVERSION "1.00.0103c"
#define FILENAME "trackerthree"

bool serialDebug = true;
bool dspPublish = true;


float lat, lon, movingRatio;
float movingRatioThreshold = 0.2;
int sats;
int page = 5, pubCount = 0, holdDownTimer = 30;
double clat, clon, mph, mps, alt,  hdop;
double speedThreshold = 4.5;
unsigned long age, lastPub, nextPub;
unsigned long isMoving = 1, isStill = 1;
String myName = " not set";
String mongoid;

http_header_t headers[] = {
      { "Content-Type", "application/json" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};
http_request_t request;
http_response_t response;


 HttpClient http;

//myprototypes declartions
void gpsDispatch();
void oledDispatch(int);
int setPage(String command);
int setSpThr(String command);
int setHDT(String command);
int setMRT(String command);
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
int gpsPublish(String command);
String generateRequestBody();
