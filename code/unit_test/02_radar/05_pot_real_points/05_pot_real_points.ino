
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <Servo.h>

Servo myservo;
const int pingPin = 7;
const int echoPin = 6; 

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

int angle=0;
const float pi = 3.14159267;
int data[180];
int distance;

void setup()   {                
  Serial.begin(9600);
  myservo.attach(9);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay();
  myservo.write(0);
  delay(1000);
}


void loop() {
    for(int angle=40;angle<140;angle+=5){
      //Serial.println(angle);
      myservo.write(angle);
      distance = map(getDistance(),0,100,0,63);
      data[angle]=distance;
      display.clearDisplay();
      drawOutline();
      int lX=63+63*cos(toRadians(angle));
      int lY=63-63*sin(toRadians(angle));
      display.drawLine(lX, lY, 63 , 63, WHITE);
      //plotPoints();
      display.display();
      
    }
    for(int angle=140;angle>40;angle-=5){
      //Serial.println(angle);
      myservo.write(angle);
      distance = map(getDistance(),0,100,0,63);
      data[angle]=distance;
      display.clearDisplay();
      drawOutline();
      int lX=63+63*cos(toRadians(angle));
      int lY=63-63*sin(toRadians(angle));
      display.drawLine(lX, lY, 63 , 63, WHITE);
      //plotPoints();
      display.display();
     
    }
  //Serial.println(" ");
}

int plotPoints(){
  for(int i=0;i<180; i++)  {
    plotPoint(i,data[i]); 
  }
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
}


float toDegrees(float radians) {
  return radians / 2 / pi * 360;
}

float toRadians(float degrees) {
  return degrees / 360 * 2 * pi;
}

int getDistance(){
  long duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  return cm;  
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
