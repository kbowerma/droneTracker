/* From
* https://github.com/pkourany/Adafruit_Unified_Sensor_Library
*/

#include "application.h"
#include "spark_wiring_i2c.h"
#include "math.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"
#include"lib/streaming/firmware/spark-streaming.h"
#include "math.h"
#include "lsmhelper.h"



void setup(void) {
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  lsminit();
  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop(void) {

  lsmGetValues();
    // Display the lsm heading values
    Serial << endl << "heading: " << heading << endl;
    Serial << "ax " << ax << " ay " << ay << " az " << az <<" mx " << mx << " my " << my << " mz " << mz << endl;


  delay(500);
}
