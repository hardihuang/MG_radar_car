
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

int angle=0;
const float pi = 3.14159267;
void setup()   {                
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
}


void loop() {
  
  
  if(angle<180){
    for(angle=0;angle<=180;angle++){
      display.clearDisplay();
      drawOutline();
      int pX=63+63*cos(toRadians(angle));
      int pY=63-63*sin(toRadians(angle));
      display.drawLine(pX, pY, 63 , 63, WHITE);
      display.display();
    }
    
  }else if(angle>0){
    for(angle=180;angle>=0;angle--){
      display.clearDisplay();
      drawOutline();
      int pX=63+63*cos(toRadians(angle));
      int pY=63-63*sin(toRadians(angle));
      display.drawLine(pX, pY, 63 , 63, WHITE);
      display.display();
    }
  }
  delay(200);
  Serial.println(" ");
}

int plotPoint(int angle, float distance){
  int pX=63+distance*cos(toRadians(angle));
  int pY=63-distance*sin(toRadians(angle));
  display.drawPixel(pX,pY,WHITE);
}

void drawOutline(){
  //display.drawLine(0, 32, 64 , 64, WHITE);
  //display.drawLine(128, 32, 64 , 64, WHITE);
  display.drawLine(0, 63, 126 , 63, WHITE);
  display.drawCircle(63,63,63,WHITE);  
  plotPoint(45,30);
}


float toDegrees(float radians) {
  return radians / 2 / pi * 360;
}

float toRadians(float degrees) {
  return degrees / 360 * 2 * pi;
}
