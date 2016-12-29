/* From
* https://github.com/pkourany/Adafruit_Unified_Sensor_Library
*/

#include "application.h"
#include "spark_wiring_i2c.h"
#include "math.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_LSM303_U.h"

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accelo = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

float mx, yx, zx, ax, ay, az;

void displaySensorDetails(void)
{
  sensor_t sensor;
  accelo.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
Serial.println("");
  Serial.println("");
  delay(500);
}

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");

  /* Initialise the sensor */
  if(!accelo.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  /* Enable auto-gain */
mag.enableAutoRange(true);

/* Initialise the sensor */
if(!mag.begin())
{
  /* There was a problem detecting the LSM303 ... check your connections */
  Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
  while(1);
}

  /* Display some basic information on this sensor */
  displaySensorDetails();
}

void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  accelo.getEvent(&event);
      ax = event.acceleration.x;
  //mag.getEvent(&event);



  /* Display the results (acceleration is measured in m/s^2) */

  //Serial.print("Xaccel: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Xaccel: "); Serial.print(ax); Serial.print("  ");
  Serial.print("Yaccel: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Zaccel: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  delay(500);


  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
 mag.getEvent(&event);
  Serial.print("Xmag: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Ymag: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Zmag: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  delay(500);
}
