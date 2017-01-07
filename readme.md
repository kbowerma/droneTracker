
# DroneTracker
DroneTracker is an adaptation of Particle's Asset Tracker but primary using  the  Photon.   It uses the tinyGPS library over the Adafruit one, since it appears to be more feature rich.   Unlike the Asset tracker it also supports an oLed with several dsiplay modes to show speed (natively calcualted in the tinyGPS lib ), heading, x y z acceleration, x y z magnometer, altitude and update publish information.   Since GPS receivers can be a little un reliable in terms of consistent reading, we use the tinyGPS speed calculation to determine if we are moving.   The variables ```speedThreshold, movingRatioThreshold, holdDownTimer``` are all setting to adjust the publishing of locations.  

![img_2963](https://cloud.githubusercontent.com/assets/1180747/21743262/16fe756c-d4c4-11e6-9b81-dcc6e507ade2.jpeg)


## BOM

1. [Adafruit Ultimate GPS Breakout - 66 channel w/10 Hz updates - Version 3](https://www.adafruit.com/products/746)
2. [Adafruit Triple-axis Accelerometer+Magnetometer (Compass) Board - LSM303](https://www.adafruit.com/products/1120)
3. [128x64 Oled LCD](https://www.amazon.com/gp/product/B01HEBIJKK/ref=oh_aui_detailpage_o02_s00?ie=UTF8&psc=1)
4. [Particle Photon](https://store.particle.io/collections/photon)
5. [SparkFun Solder-able Breadboard](https://www.sparkfun.com/products/12070)
6. [Female Headers](https://www.amazon.com/gp/product/B01DLX6RSQ) optional

## Additional Hardware Considerations
The following buzzer/led are also under consideration for when the tracker flies into a no fly zone:
1.  [Usmile Dual Modes (FC mode& TX Mode) 2 in 1 WS2812B LED Tail light And 5V loud Buzzer For FPV Quadcopter racing](https://www.amazon.com/gp/product/B01MG3ZWCZ)
2.  [Usmile® 2 in 1 WS2812B LED And 5V Active Buzzer For FPV Quadcopter NAZE32 Skyline32 SPRaciangF3(CleanFlight) Flight Controller](https://www.amazon.com/gp/product/B019UVHWMS)

## Assembly notes

## Test flight
On 1/6/2017 rig flew approximately 5 minutes and provide two updates to dsp.  ```speedThreshold``` was set to 4.5mph, update frequency (```holdDownTimer```) was set to 30 seconds and  ```movingRatioThreshold ``` set to 0.2.  Reading the particle variables it seemed like the speed was under the threshold (2.5mps) for the first few minutes of the flight not resulting in publishing location. I need to look at the speed calculation which I think is weighted over last few reading so it takes time to rise and I also notice that it takes a few minutes to settle.   

![screen shot 2017-01-07 at 10 25 45 am](https://cloud.githubusercontent.com/assets/1180747/21743256/03d9552e-d4c4-11e6-9926-93ec6632baea.png)
