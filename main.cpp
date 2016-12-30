/* From
* https://github.com/krvarma/tinygps_sparkcore
* [x] Oled
* [x] LSM303DLHC
* [ ] display odes
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

    getMyName();
    oledInit();
    lsminit();

}

void loop(){

gpsDispatch();
lsmGetValues();
oled1();
 //getMyName();
//Serial << System.deviceID() <<  " myname: " << myName << endl;

    delay(500);
}
// ************ FUNCTIONS ***************

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
       Serial << "lat: " << clat << " lon " << clon;
       Serial << " alt " << alt << " sats " << sats << " hdop " << hdop;
       Serial  << " speed "<< String(mph) <<" mph";
       Serial << " " << String(mps) <<" mps";
       Serial << " age " << String(age)   << " heading " << heading << endl;

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
void oled1() {
  display.setTextSize(2);
  display.clearDisplay();
  display.setCursor(5,16);
  display << heading << endl;
  display.setTextSize(1);
  display.setCursor(0,0);
  display << "sats: " << sats << "  HDOP " << hdop << endl;
  display << "     " << mph << "mph " << endl;
  /*
  display << "x: " << xAccl << endl;
  display << "y: " << yAccl << endl;
  display << "z: " << zAccl << endl;
  */
  display.display();
}
void getMyName() {
  if( System.deviceID().compareTo("37001a000a47353137323334") == 0 ) myName = "bobcat_hunter";
  if( System.deviceID().compareTo("2c0019000a47353137323334") == 0 ) myName = "regal_air";
  if( System.deviceID().compareTo("350039001547353236343033") == 0 ) myName = "trackertwo";
}
