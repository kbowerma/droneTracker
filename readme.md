
# DroneTracker
DroneTracker is an adaptation of Particle's Asset Tracker but primary using  the  Photon.   It uses the tinyGPS library over the Adafruit one, since it appears to be more feature rich.   Unlike the Asset tracker it also supports an oLed with several dsiplay modes to show speed (natively calcualted in the tinyGPS lib ), heading, x y z acceleration, x y z magnometer, altitude and update publish information.   Since GPS receivers can be a little un reliable in terms of consistent reading, we use the tinyGPS speed calculation to determine if we are moving.   The variables ```speedThreshold, movingRatioThreshold, holdDownTimer``` are all setting to adjust the publishing of locations.  

![img_2963](https://cloud.githubusercontent.com/assets/1180747/21743262/16fe756c-d4c4-11e6-9b81-dcc6e507ade2.jpeg)

---

>Version 2 with LM7805 voltage regulator and 500mAh 3S lipo

![img_2972](https://cloud.githubusercontent.com/assets/1180747/21792889/781a6096-d6b2-11e6-8d3e-d191f10ce97c.JPG)

---

>Version 3 with tail lights on bread board

![img_2978](https://cloud.githubusercontent.com/assets/1180747/21911359/1164223e-d8e6-11e6-8847-68ccdfb94b1e.JPG)


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


## Test flight 1
On 1/6/2017 rig flew approximately 5 minutes and provide two updates to dsp.  ```speedThreshold``` was set to 4.5mph, update frequency (```holdDownTimer```) was set to 30 seconds and  ```movingRatioThreshold ``` set to 0.2.  Reading the particle variables it seemed like the speed was under the threshold (2.5mps) for the first few minutes of the flight not resulting in publishing location. I need to look at the speed calculation which I think is weighted over last few reading so it takes time to rise and I also notice that it takes a few minutes to settle.   

![screen shot 2017-01-07 at 10 25 45 am](https://cloud.githubusercontent.com/assets/1180747/21743256/03d9552e-d4c4-11e6-9926-93ec6632baea.png)

## Test Drive 14 and 15

Test drive 14 performed on 1/9/2016 included 30 second updates (HDT) and 4.5mph Moving threshold - 17 updates
![screen shot 2017-01-09 at 3 38 03 pm](https://cloud.githubusercontent.com/assets/1180747/21784547/ae423be8-d681-11e6-823c-5d85712beecf.png)

Test drive 15 upd' the update (HDT threshold to 20 seoncds) and had a moving threshold of 4.5mph, 15 points are not included due to zoom factor.   This trip included a ~10 minute stop which did not include any updates.
<img width="1646" alt="screen shot 2017-01-09 at 7 20 58 pm" src="https://cloud.githubusercontent.com/assets/1180747/21790304/e827f734-d6a0-11e6-8398-f382817bb8d4.png">
