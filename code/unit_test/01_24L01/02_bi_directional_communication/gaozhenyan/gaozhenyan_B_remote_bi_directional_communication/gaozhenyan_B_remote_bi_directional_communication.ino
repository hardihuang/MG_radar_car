/*
========================================
  24L01无线通信 V0.3 -双向
  高禛言
========================================

【24L01模块接线】(模块引脚参考资料文件夹图片)
---------------------------------------
  1 - GND
  2 - VCC 连接3.3V
  3 - CE 连接D5
  4 - CSN 连接D4
  5 - SCK 连接D13
  6 - MOSI 连接D11
  7 - MISO 连接D12
  8 - UNUSED

【屏幕连接】
---------------------------------------
  SCK - A5
  SDA - A6

【按键连接】
---------------------------------------
  按钮A - 连接 D2
  按钮B - 连接 D3
  joystick_A - 连接 A0
  joystick_B - 连接 A1
  joystick_C - 连接 A2
  joystick_D - 连接 A3
*/

#include <SPI.h>
#include <RF24.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define COUNT 10 // nomber for statistics
#define DATARATE RF24_2MBPS
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};
RF24 radio(5, 6);
#define OLED_RESET 10
Adafruit_SH1106 display(OLED_RESET);

int _success = 0;
int _fail = 0;
unsigned long _startTime = 0;

int btnA = 2;
int btnB = 3;
int joystickA = A0;
int joystickB = A1;
int joystickC = A2;
int joystickD = A3;

typedef struct{
  bool btnA;
  bool btnB;
  int joyStick_A_X;
  int joyStick_A_Y;
  int joyStick_B_X;
  int joyStick_B_Y;
}
remoteDef;
remoteDef remotePak;

typedef struct{
  int angle;  
  int distance;
}
robotDef;
robotDef robotPak;

void setup(){
  Serial.begin(9600);
  
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);
  display.clearDisplay();

//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(0,0);
//  display.println("Hello, world!");
//  display.display();
//  delay(2000);
//  display.clearDisplay();
//  
  
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(joystickA, INPUT);
  pinMode(joystickB, INPUT);
  pinMode(joystickC, INPUT);
  pinMode(joystickD, INPUT);
 
  
  radio.begin();
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.setDataRate( DATARATE ) ;
  radio.setPALevel( RF24_PA_MAX ) ;
  radio.setChannel(0x34);
  radio.enableDynamicPayloads() ;
  radio.enableAckPayload();               // not used here
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setAutoAck( true ) ;
  radio.printDetails();
  radio.powerUp();
  radio.startListening();
}

void loop(){
  unsigned long loop_start = millis();
  remotePak.btnA = !digitalRead(btnA);
  remotePak.btnB = !digitalRead(btnB);
  remotePak.joyStick_A_X = analogRead(joystickA);
  remotePak.joyStick_A_Y = analogRead(joystickB);
  remotePak.joyStick_B_X = analogRead(joystickC);
  remotePak.joyStick_B_Y = analogRead(joystickD);
  radio.stopListening();
  if (!radio.write( &remotePak, sizeof(remotePak) )) {
      Serial.println("failed!");
  }
  radio.startListening();
  while ( !radio.available() && (millis() - loop_start) < 200) {
    Serial.println("waiting...");
  }
  
  if (millis() - loop_start >= 200) {
    printf("Failed. Timeout: %i...", millis() - loop_start);
    _fail++;
  } else {
    // get the telemetry data
    radio.read( &robotPak, sizeof(robotPak) );
    Serial.print("Got response ");
    _success++;
  }
  if (_fail + _success >= COUNT){
    int _ratio = 100 * _fail / (_fail + _success);
    Serial.print("Time ");
    _startTime = (millis() - _startTime);
//    Serial.print(_startTime);
//    Serial.print(" success ");
//    Serial.print(_success);
//    Serial.print(" timeout ");
//    Serial.print(_fail);
//    Serial.print(" Failed ");
//    Serial.print(_ratio);
//    Serial.print("% ");
//    for (int _i = 0; _i < _ratio; _i++) Serial.print("*");
//    
//    Serial.print(" angle: ");
//    Serial.print(robotPak.angle);
//    Serial.print(" distance: ");
//    Serial.print(robotPak.distance);

    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("angle: ");
    display.println(robotPak.angle);
    display.println("distance: ");
    display.println(robotPak.distance);
    display.display();
    display.clearDisplay();   
    
    Serial.println();
    _success = 0;
    _fail = 0;
    _startTime = millis();
  }
}
