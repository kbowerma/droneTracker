/* From
* https://github.com/krvarma/tinygps_sparkcore
*/

#include "application.h"
#include "spark_wiring_i2c.h"
#include "math.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"
#include"lib/streaming/firmware/spark-streaming.h"
#include "math.h"
#include "lsmhelper.h"
#include "lib/TinyGPS_SparkCore/firmware/TinyGPS.h"
#include "myhelper.h"




TinyGPS tgps;  // Instatinate Tny GPS


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
}

void loop(){
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
         hdop =  tgps.hdop();
         mps = tgps.f_speed_mps();
         mph = tgps.f_speed_mph();
         age = age;
         sats = tgps.satellites();
      if(serialDebug) {
         Serial << "lat: " << String(lat) << " lon " << String(lon) << " alt " << tgps.f_altitude() << " sats " << tgps.satellites() << " hdop " << tgps.hdop();
         Serial  << " speed "<< String(tgps.f_speed_mph()) <<" mph";
         Serial << " " << String(tgps.f_speed_mps()) <<" mps";
         Serial << " age " << String(age)   << " clat " << clat << endl;
      }
    }
    else{
        // Not a vlid GPS location, jsut pass 0.0,0.0
        // This is not correct because 0.0,0.0 is a valid GPS location, we have to pass a invalid GPS location
        // and check it at the client side
        Serial << " . ";
        //sprintf(szInfo, "0.0,0.0");
    }


    delay(500);
}
