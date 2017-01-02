/* From
* https://github.com/krvarma/tinygps_sparkcore
* [x] Oled
* [x] LSM303DLHC
* [ ] display modes
* [ ] Posting with holdown logic
 */

#include "application.h"
#include "spark_wiring_i2c.h"
#include "math.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"
#include "lib/Adafruit_SSD1306/Adafruit_SSD1306.h"
#include "lib/streaming/firmware/spark-streaming.h"
#include "lsmhelper.h"
#include "lib/TinyGPS_SparkCore/firmware/TinyGPS.h"
#include "myhelper.h"
#include "oledhelper.h"





TinyGPS tgps;  // Instatinate Tny GPS
//Adafruit_SSD1306 display(OLED_RESET) done in myoled

void setup(){
    Serial1.begin(9600);
    Serial.begin(9600);
    Particle.variable("lat", clat);
    Particle.variable("lon", clon);
    Particle.variable("alt", alt);
    Particle.variable("sats", sats);
    Particle.variable("mph", mph);
    Particle.variable("mps", mps);
    Particle.variable("HDOP", hdop);
    Particle.variable("version", MYVERSION);
    Particle.variable("project", FILENAME);
    Particle.variable("heading", heading);
    Particle.function("setPage", setPage);
    Particle.function("setSpThr", setSpThr);
    Particle.function("setHDT", setHDT);

    getMyName();
    oledInit();
    lsminit();
    nextPub = millis() + holdDownTimer * 1000;  // need to init first HDT

}

void loop(){

gpsDispatch();
lsmGetValues();
oledDispatch(page);
testPub();


    delay(500);
}
// ************ FUNCTIONS ***************

int setPage(String command){
  page = atoi(command);
  return page;
}
int setSpThr(String command) {
  speedThreshold = command.toFloat();
  return 1;
}
int setHDT(String command) {
  holdDownTimer = command.toInt();
  //goPub();  //think this should be testPub
  testPub();
  nextPub = millis() + holdDownTimer * 1000;
  return 1;
}

void oledDispatch(int page) {
  if (page == 0 ) oled0();  // off
  if (page == 1 ) oled1();  // info
  if (page == 2 ) oled2();  // speed
  if (page == 3 ) oled3();  // compas
  if (page == 4 ) oled4();  // magnometer
  if (page == 5 ) oled5();  // pub info
}
void gpsDispatch() {
  bool isValidGPS = false;
  for (unsigned long start = millis(); millis() - start < 1000;){
      // Check GPS data is available
      while (Serial1.available()){
          char c = Serial1.read();

          // parse GPS data
          if (tgps.encode(c))
              isValidGPS = true;
      }
  }
  // If we have a valid GPS location then publish it
  if (isValidGPS){
      tgps.f_get_position(&lat, &lon, &age);

       clat = lat;
       clon = lon;
       alt = tgps.f_altitude();
       hdop =  double(tgps.hdop())/100;
       mps = tgps.f_speed_mps();
       mph = tgps.f_speed_mph();
       age = age;
       sats = tgps.satellites();
    if(serialDebug) {
       Serial << "lat: " << String(clat) << " lon " << String(clon);
       Serial << " alt " << alt << " sats " << sats << " hdop " << hdop;
       Serial  << " speed "<< String(mph) <<" mph";
       Serial << " " << String(mps) <<" mps";
       Serial << " age " << String(age)   << " heading " << heading;
       Serial << " mRatio: " << movingRatio << endl;

    }
  }
  else{
      // Not a vlid GPS location, jsut pass 0.0,0.0
      // This is not correct because 0.0,0.0 is a valid GPS location, we have to pass a invalid GPS location
      // and check it at the client side
      Serial << " . ";
      //sprintf(szInfo, "0.0,0.0");
  }
}
void getMyName() {
  if( System.deviceID().compareTo("37001a000a47353137323334") == 0 ) {
    myName = "bobcat_hunter";
    mongoid = "585c05f7fe76420004a44a0b";
  }
  if( System.deviceID().compareTo("2c0019000a47353137323334") == 0 ) {
    myName = "regal_air";
    mongoid = "58629f8a01a1100004e8d83d";
  }
  if( System.deviceID().compareTo("350039001547353236343033") == 0 ) {
    myName = "Rusts";
    mongoid = "584adbcfaebc030004a68a8d";
  }
}
void testPub() {
  if (mph > speedThreshold ) isMoving++;
  if (mph < speedThreshold ) isStill++;
  movingRatio = float(isMoving) / float(isStill);

  // call goPub if timer expired && we know we are moving good.
  if (millis() > nextPub )
  {
    if (mph > speedThreshold && movingRatio > 0.5 ) {
    // yes pub
      goPub();
      nextPub = millis() + holdDownTimer * 1000;
    }
    // no pub
    Serial << "no pub, speed: " << String(mph) << " MR " << String(movingRatio) << endl;
    nextPub = millis() + holdDownTimer * 1000;
  }
}
void goPub() {
  //prepub

  //pub
  Particle.publish("t3", String( String(lat) + "," +String(lon)+","+String(mph)));

  //postpub
  //nextPub = millis() + holdDownTimer * 1000;  moved up to test
  pubCount++;
  isMoving = isStill = movingRatio = 1;


}
