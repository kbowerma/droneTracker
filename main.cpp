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





TinyGPS tgps;
char szInfo[64];
// Every 15 minutes
int sleep = 15 * 60 * 1000;

void setup(){
    Serial1.begin(9600);
    Serial.begin(9600);
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
        float lat, lon;
        unsigned long age;

        tgps.f_get_position(&lat, &lon, &age);

        sprintf(szInfo, "%.6f,%.6f", (lat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lat), (lon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : lon));


         Serial << "lat: " << String(lat) << " lon " << String(lon) << " alt " << tgps.f_altitude() << " sats " << tgps.satellites() << " hdop " << tgps.hdop() << " speed "<< tgps.f_speed_mph() <<" mph"  << endl;
    }
    else{
        // Not a vlid GPS location, jsut pass 0.0,0.0
        // This is not correct because 0.0,0.0 is a valid GPS location, we have to pass a invalid GPS location
        // and check it at the client side
        sprintf(szInfo, "0.0,0.0");
    }

    // Spark.publish("gpsloc", szInfo);

    // Sleep for some time
    //delay(sleep);
    delay(500);
}
