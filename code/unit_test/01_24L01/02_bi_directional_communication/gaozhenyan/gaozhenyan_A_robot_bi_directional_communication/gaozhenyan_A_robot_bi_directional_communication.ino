#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

Servo myservo;
bool dir=1;
const int pingPin = A1;
const int echoPin = A0; 
#define COUNT 10 // nomber for statistics
#define DATARATE RF24_2MBPS
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};
RF24 radio(A3, A4);

int _success = 0;
int _fail = 0;
unsigned long _startTime = 0;

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

int angle=0;
const float pi = 3.14159267;
int data[180];
int distance;

void setup(){
  Serial.begin(9600);
  myservo.attach(10);
  myservo.write(0);
  delay(1000);
  
  radio.begin();

  //different from remote code
  radio.openWritingPipe(pipes [1]);
  radio.openReadingPipe(1, pipes[0]);
  
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
  myservo.write(angle);
  distance = getDistance();
//  Serial.print(angle);
//  Serial.print(" ");
//  Serial.print(distance);
//  Serial.print(" ");
//  Serial.println(" ");
  if ( radio.available()){
    radio.read(&remotePak, sizeof(remotePak));
    Serial.print(remotePak.btnA);
    Serial.print(" ");
    Serial.print(remotePak.btnB);
    Serial.print(" ");
    Serial.println(" ");
    robotPak.angle = angle;
    robotPak.distance = distance;
    radio.stopListening();
    radio.write( &robotPak, sizeof(robotPak) );
    radio.startListening();
  }
  
  if(dir==1){
    if(angle<180){
      angle++;  
    }else{
      dir = 0;  
    }
  }else if(dir ==0){
    if(angle>0){
      angle--;  
    }else{
      dir = 1;  
    }
  }

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
