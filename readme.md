
# DroneTracker
DroneTracker is an adaptation of Particle's Asset Tracker but primary using  the  Photon.   It uses the tinyGPS library over the Adafruit one, since it appears to be more feature rich.   Unlike the Asset tracker it also supports an oLed with several dsiplay modes to show speed (natively calcualted in the tinyGPS lib ), heading, x y z acceleration, x y z magnometer, altitude and update publish information.   Since GPS receivers can be a little un reliable in terms of consistent reading, we use the tinyGPS speed calculation to determine if we are moving.   The variables ```speedThreshold, movingRatioThreshold, holdDownTimer``` are all setting to adjust the publishing of locations.  

![img_2963](https://cloud.githubusercontent.com/assets/1180747/21743262/16fe756c-d4c4-11e6-9b81-dcc6e507ade2.jpeg)

---

>Version 2 with LM7805 voltage regulator and 500mAh 3S lipo

![img_2972](https://cloud.githubusercontent.com/assets/1180747/21792889/781a6096-d6b2-11e6-8d3e-d191f10ce97c.JPG)

---

>Version 3 with tail lights on bread board

![img_2978](https://cloud.githubusercontent.com/assets/1180747/21911359/1164223e-d8e6-11e6-8847-68ccdfb94b1e.JPG)

---




The particle mobile app allows for the device inspector to show all the particle variables and update the configuration variables via Particle.functions like: ```oled page, speed threshold for update, Update frequency, move ratio , Force Publish, Toggle Horn, Toggle Lights```  



 ![img_2980](https://cloud.githubusercontent.com/assets/1180747/21912899/954d4258-d8ee-11e6-8eed-725266389036.PNG)

Type |Name | Purpose
 ----| ------- | ----|
variable | lat | current GPS Latitude
variable | lon | current GPS Longitude
variable | alt | current GPS Altitude
variable | sats | number of Satellites
variable | mph | derived speed in miles per hour
variable | mps | derived speed in meters per seconds
variable | HDOP | Horizontal Degree of Precision
variable | pubCount| number or time location published to dsp since last reboot
variable | version | version defined in header file
variable | project | project (FILE) name defined in header file
variable | heading | LSM303 heading
variable | alarm | is in alarm state NFZ violation
function | setPage | sets the oled page [0-5]
function | setSpThr| sets speed threshold, the speed value (in mph) which counts a a "move" if exceeded or "still" if under.  Related to move ratio. (the number of moves/still during a cycle to determine if a gps update is necessary)
function | setHDT | Hold Down timer.  Frequency of update candidacy in seconds
function | gpsPublish | Forces a GPS update to the GPS



## BOM

1. [Adafruit Ultimate GPS Breakout - 66 channel w/10 Hz updates - Version 3](https://www.adafruit.com/products/746)
2. [Adafruit Triple-axis Accelerometer+Magnetometer (Compass) Board - LSM303](https://www.adafruit.com/products/1120)
3. [128x64 Oled LCD](https://www.amazon.com/gp/product/B01HEBIJKK/ref=oh_aui_detailpage_o02_s00?ie=UTF8&psc=1)
4. [Particle Photon](https://store.particle.io/collections/photon)
5. [SparkFun Solder-able Breadboard](https://www.sparkfun.com/products/12070)
6. [Female Headers](https://www.amazon.com/gp/product/B01DLX6RSQ)
6.  [Usmile® 2 in 1 WS2812B LED And 5V Active Buzzer For FPV Quadcopter NAZE32 Skyline32 SPRaciangF3(CleanFlight) Flight Controller](https://www.amazon.com/gp/product/B019UVHWMS) **Use this one**
7. [5V/12V Mini BEC](https://www.amazon.com/gp/product/B018I3S5SY/ref=oh_aui_detailpage_o01_s00?ie=UTF8&psc=1)  **Use this to drop down a 2S or 3S lipo to 5V to power Vin of the photon and the tail light/buzzer.   Use the 3.3v Pin on the photon to power the GPS, oled and Accelerometer
8.  [N-channel power MOSFET - 30V / 60A](https://www.adafruit.com/products/355)  This mosfet allows you to draw power from the BEC but switch from the Photon.


## Additional Hardware Considerations
The following buzzer/led are also under consideration for when the tracker flies into a no fly zone:

1.  [Usmile Dual Modes (FC mode& TX Mode) 2 in 1 WS2812B LED Tail light And 5V loud Buzzer For FPV Quadcopter racing](https://www.amazon.com/gp/product/B01MG3ZWCZ)

## Assembly notes

- The buzzer will horn during bootup since pin is floating but a 10k resistor across D3 and ground will fix that.
- the GPS should get a fix after a few minutes if it is outside or in the dashboard or the car.  However it will struggle indoors.   To aid in development you can get the [antenna](https://www.adafruit.com/products/960) and [SMA to uFL adaptor](https://www.adafruit.com/products/851) from Adafruit

***Hookup guide***

Photon pin |Device | Device  Pin
----| ------- | ----| -------
VIN| IBEC / volt reg | +
GND| IBEC / volt reg  | -
Tx| GPS | Rx
Rx| GPS | Tx
WKP| - |
DAC| - |
A5| - |
A4| - |
A3| - |
A2| - |
A1| - |
A0| - |
3v3 | OLED/GPS | VIN
RST | -
VBAT | -
GND | OLED/GPS |gnd
D7 | -
D6 | -
D5 | -
D4 | -
D3 | MOSFET Gate | 2 (buzzer- MOSFET Drain)
D2 | tail lights | DIN
D1 | OLED/Alt | SCL
D0 | OLED/Alt | SDA


## Test flight 1
On 1/6/2017 rig flew approximately 5 minutes and provide two updates to dsp.  ```speedThreshold``` was set to 4.5mph, update frequency (```holdDownTimer```) was set to 30 seconds and  ```movingRatioThreshold ``` set to 0.2.  Reading the particle variables it seemed like the speed was under the threshold (2.5mps) for the first few minutes of the flight not resulting in publishing location. I need to look at the speed calculation which I think is weighted over last few reading so it takes time to rise and I also notice that it takes a few minutes to settle.   

![screen shot 2017-01-07 at 10 25 45 am](https://cloud.githubusercontent.com/assets/1180747/21743256/03d9552e-d4c4-11e6-9926-93ec6632baea.png)

## Test Drive 14 and 15

Test drive 14 performed on 1/9/2016 included 30 second updates (HDT) and 4.5mph Moving threshold - 17 updates
![screen shot 2017-01-09 at 3 38 03 pm](https://cloud.githubusercontent.com/assets/1180747/21784547/ae423be8-d681-11e6-823c-5d85712beecf.png)

Test drive 15 upd' the update (HDT threshold to 20 seoncds) and had a moving threshold of 4.5mph, 15 points are not included due to zoom factor.   This trip included a ~10 minute stop which did not include any updates.
<img width="1646" alt="screen shot 2017-01-09 at 7 20 58 pm" src="https://cloud.githubusercontent.com/assets/1180747/21790304/e827f734-d6a0-11e6-8398-f382817bb8d4.png">
