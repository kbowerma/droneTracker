/* From
* https://github.com/krvarma/tinygps_sparkcore
* [x] Oled
* [x] LSM303DLHC
* [x] display modes
* [x] Posting with holdown logic
* [x] Siren
* [ ] Slow belt
* [ ] Logic to stop pub when loose fix
* [ ] check lsm hardware exists
 */

#include "application.h"
#include "spark_wiring_i2c.h"
#include "math.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"
#include "lib/Adafruit_SSD1306/Adafruit_SSD1306.h"
#include "lib/streaming/firmware/spark-streaming.h"
#include "lib/HttpClient/firmware/HttpClient.h"
#include "lib/SparkJson/firmware/SparkJson.h"
#include "neopixel.h"
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
    Particle.variable("pubCount",pubCount);
    Particle.variable("version", MYVERSION);
    Particle.variable("project", FILENAME);
    Particle.variable("heading", heading);
    Particle.variable("alarm", NFZalarm);
    Particle.function("setPage", setPage);
    Particle.function("setSpThr", setSpThr);
    Particle.function("setHDT", setHDT);
    Particle.function("gpsPublish", gpsPublish);

    pinMode(D7, OUTPUT); // built in oled
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    pinMode(D3, OUTPUT);
    Particle.function("horn",setHorn);
    Particle.function("flasher",setFlasher);

    getMyName();
    oledInit();
    lsminit();
    nextPub = millis() + holdDownTimer * 1000;  // need to init first HDT

    request.port = 80;
    request.hostname = "kb-dsp-server-dev.herokuapp.com";
    // request.path = String("/api/v1/drones/" + mongoid +"?returnNFZ=true&nfzFields=id");  // by id
    request.path = String("/api/v1/drones/position/" + System.deviceID() +"?returnNFZ=true&nfzFields=id"); 

    nextGPSCheck = millis() + SLOW_BELT_TIMER;

}

void loop(){



if (millis() > nextGPSCheck ) {  // CHECK THE GPS LOOP EVERY SLOW_BELT_TIMER

  gpsDispatch();
  if (lsmEnabled) lsmGetValues();
  oledDispatch(page);
  testPub();
  oledAlarm();
  nextGPSCheck = millis() + SLOW_BELT_TIMER;
}

if ( flasher == true ) {
  // 120ms
 colorWipe(strip.Color(255, 0, 0), 50); // Red
 colorWipe(strip.Color(255, 255, 255), 20); // white?
 colorWipe(strip.Color(0, 0, 255), 50); // Blue
 colorWipe(strip.Color(0, 0, 0), 1); // off

  // was 50 20 50
}



    //delay(500); replaced by SLOW_BELT_TIMER logic
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
int setMRT(String command) {
  /* this sets the moving Ratio which is the number of isMoving
  * vs isStill that happens withing the holdDownTimer
  * for example is this is set to 0.5 then half of the polls
  * have to be moving in order it to publish
  */
  movingRatioThreshold = command.toInt();
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
       Serial << " mRatio: " << String(movingRatio) << endl;

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
    mongoid = "Fried";
  }
  if( System.deviceID().compareTo("1a0038000247353137323334") == 0 ) {
    myName = "Yetna";
    mongoid = "5873b3d04cc7cf0004d0c3c9";
  }
  if( System.deviceID().compareTo("290031000f47353136383631") == 0 ) {
    myName = "Susitna";
    mongoid = "587e3870d194300004d5c51d";
  }
  if( System.deviceID().compareTo("32001f001551353531343431") == 0 ) {
    myName = "Whisky";
    mongoid = "587e4f836772ed0004d88584";
  }
}
void testPub() {
  if (mph > speedThreshold ) isMoving++;
  if (mph < speedThreshold ) isStill++;
  movingRatio = float(isMoving) / float(isStill);

  // call goPub if timer expired && we know we are moving good.
  if (millis() > nextPub ) {
    if (mph > speedThreshold && movingRatio > movingRatioThreshold ) {
    // yes pub
      goPub();
      nextPub = millis() + holdDownTimer * 1000;
    } else {
    // no pub
    Serial << "no pub, speed: " << String(mph) << " MR " << String(movingRatio) << endl;
    nextPub = millis() + holdDownTimer * 1000;
  }
    // Do this every cycle no mater what
    isMoving = isStill = movingRatio = 1;
  }

}
void goPub() {
  //prepub

  //pub
  Particle.publish("t3", String( String(lat) + "," +String(lon)+","+String(mph)));
  if ( dspPublish == true ) gpsPublish("1");  // this does not ever seem to publish
  // gpsPublish("1");
  //postpub
  //nextPub = millis() + holdDownTimer * 1000;  moved up to test
  //pubCount++;  // move to gpsPublish so I stroke the counter when I manual run it
  //isMoving = isStill = movingRatio = 1;  //moved up to the test


}
int gpsPublish(String command){

        if ( command == "1"  ) {
        request.body = generateRequestBody();
        http.put(request, response, headers);
        Serial << "Fnc call: http body: " << request.body << endl;
        Serial << "Request.path: " << request.path << endl;
        Serial << "RESPONSE:  "  << endl << response.body << endl;
        //Serial << endl << endl << "NFZ "  << response.body.noFlyZones << endl << endl;

        /*

        StaticJsonBuffer<1200> jsonBuffer;
        char mybuff[1200];

         strncpy(mybuff,response.body,strlen(response.body)-1);

        JsonObject& root = jsonBuffer.parseObject(mybuff);

        const char* myNFZ = root["noFlyZones"];

        Serial << endl << "one " << sizeof(root["noFlyZones"]) << endl;
        //Serial << endl << "two " << String(root["noFlyZones"]) << endl;
        Serial << endl << "three " << String(myNFZ) << endl;

        // None of the above works
        */
        int nfzIndex = response.body.indexOf("noFlyZones");
        String NFZString = response.body.substring(nfzIndex);
        // Serial << "NFZ: " << response.body.substring(nfzIndex) << endl;
        Serial << "NFZ: " << NFZString << endl;
        //Serial << "the magic index of noFlyZones is: " << response.body.indexOf("noFlyZones");

        if ( NFZString.length() > 15 ) setNFZAlarm("1");  // set alarm
        if ( NFZString.length() < 16 ) setNFZAlarm("0");  // Clear Alarm

        Serial << "NFZ length is: " << NFZString.length();
        pubCount++;
        }
        return 1;

}
String generateRequestBody() {
  //Function to assembly JSON body payload
     // A Dynamic Json buffer
     DynamicJsonBuffer jsonBuffer;
     JsonObject& obj = jsonBuffer.createObject();
     char buf[500];
     JsonVariant lat;
     lat.set(clat, 6);
     obj["lat"] = lat;
     JsonVariant lng;
     lng.set(clon, 6);
     obj["lng"] = lng;
     obj.printTo(buf, sizeof(buf));
       return String(buf);

 }
int setNFZAlarm(String command) {
  if ( atoi(command) == 0 ) { // clear alarm
    NFZalarm = 0;
    digitalWrite(D7, LOW);  //turn off built in led
    setHorn("0");
    setFlasher("0");
    return 0;
  }
  if ( atoi(command) == 1 ) { // clear alarm
    NFZalarm = 1;
    digitalWrite(D7, HIGH);  //turn on built in led
    setHorn("1");
    setFlasher("1");
    return 1;
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
int setHorn(String command) {
    if ( atoi(command) == 0 ) {
        digitalWrite(D3, LOW); //turn off alarm
        return 0;
    }
        if ( atoi(command) == 1 ) {
        digitalWrite(D3, HIGH); //turn off alarm
        return 1;
    }
}
int setFlasher(String command) {
        if ( atoi(command) == 0 ) {  // turn off
        flasher = false;
        //strip.Color(0, 0, 0);
        return 0;
    }
        if ( atoi(command) == 1 ) {  // turn on
        flasher = true;
        return 1;
    }
}
