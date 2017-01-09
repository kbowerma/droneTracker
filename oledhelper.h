#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

#define OLED_RESET D4

Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void oledInit() {
  String myID = System.deviceID();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();   // clears the screen and buffer
            // from 1-9 sensible actually can be bigger but useless
  display.setTextColor(WHITE, BLACK); // 'normal' text
       // 128,64 pixels
  //display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(myName);
  display.println(FILENAME);
  display.setCursor(0,20);
  display.setTextSize(2);
  display.println("setup");
  display.setTextSize(1);
  display.println(MYVERSION);

  display.display();
  delay(4000);
}
void oled0() {
  display.clearDisplay();
  display.display();
}
void oled1() {  // INFO
  display.setTextSize(2);
  display.clearDisplay();
  display.setTextColor(BLACK,WHITE);
  display.setCursor(0,0);
  display << "  INFO  1";
  // blue
  display.setCursor(0,16);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display << String(WiFi.SSID()) << endl;

  display << FILENAME << endl << MYVERSION << endl;
  display << "sats: " << sats << endl;
  display  << myName << endl;
  display <<  mongoid << endl;


  display.display();
}
void oled2() { // SPEED and Sats
  display.setTextSize(4);
  display.clearDisplay();
  display.setCursor(5,16);
  display << mph << " " << endl;
  display.setTextSize(1);
  display.setCursor(0,0);
  display << "sats: " << sats << "  HDOP " << hdop << endl;
  display << "     " << heading << " deg" << endl;
  display.display();
}
void oled3() { // COMPASs
  display.setTextSize(2);
  display.clearDisplay();
  display.setTextColor(BLACK,WHITE);
  display.setCursor(4,0);
  display << "COMPASS 3";
  // blue
  display.setCursor(0,16);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
  display << heading << endl;
  display.setTextSize(1);
  display << "x: " << ax << endl;
  display << "y: " << ay << endl;
  display << "z: " << az << endl;
  display.display();
}
void oled4() { // MAGNOMETER
  display.setTextSize(2);
  display.clearDisplay();
    display.setTextColor(BLACK,WHITE);
  display.setCursor(0,0);
  display << "MAGNOMET";
  // blue
  display.setCursor(0,16);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
  display << heading << endl;
  display.setTextSize(1);
  display << "MX: " << mx << endl;
  display << "MY: " << my << endl;
  display << "MZ: " << mz << endl;
  display.display();
}
void oled5() { // Publish
  display.setTextSize(2);
  display.clearDisplay();
    display.setTextColor(BLACK,WHITE);
  display.setCursor(0,0);
  display << "Publish 5";
  // blue
  display.setCursor(0,16);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display << "pub: " << pubCount << "  sats: " << sats << endl;
  display << "mv: " << isMoving << " still: " << isStill << endl;
  display << "mph: " << String(mph) << endl;
  display << "mRatio: " << movingRatio << endl;
  display << "Pub in " << ( nextPub - millis())/1000 << endl;
  display << "HDT: " << holdDownTimer;
  display << " | MT " << speedThreshold;


  display.display();
}
