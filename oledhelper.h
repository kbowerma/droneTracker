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
