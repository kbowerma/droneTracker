/* Kyle Bowerman 12.29.2016
*
*/
#define MYVERSION "2.00.0111"
#define FILENAME "droneTracker"
// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_COUNT 8
#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);


bool serialDebug, lsmEnabled = true;
bool dspPublish = true;
bool flasher = false;


float lat, lon, movingRatio;
float movingRatioThreshold = 0.2;
int sats;
int NFZalarm = 0;
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
int setNFZAlarm(String command);
String generateRequestBody();
void colorWipe(uint32_t c, uint8_t wait);
int setHorn(String command);
int setFlasher(String command);
