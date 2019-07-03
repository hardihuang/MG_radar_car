#include <SPI.h>
#include <RF24.h>

#define btnA 2
#define btnB 3
#define btnC 4
#define btnD 5
#define light 6

#define COUNT 10 // nomber for statistics
#define DATARATE RF24_2MBPS
const uint64_t pipes[2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};
RF24 radio(7, 8);

int _success = 0;
int _fail = 0;
unsigned long _startTime = 0;

typedef struct{
  bool buttonA;
  bool buttonB;
  bool buttonC;
  bool buttonD;
}
remoteDef;
remoteDef remotePak;

typedef struct{
  bool lightSwitch;  
}
robotDef;
robotDef robotPak;

void setup(){
  Serial.begin(9600);
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(btnC, INPUT_PULLUP);
  pinMode(btnD, INPUT_PULLUP);
  pinMode(light, OUTPUT);

  digitalWrite(light, LOW);
  
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
  remotePak.buttonA = digitalRead(btnA);
  remotePak.buttonB = digitalRead(btnB);
  remotePak.buttonC = digitalRead(btnC);
  remotePak.buttonD = digitalRead(btnD);
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
    Serial.print(_startTime);
    Serial.print(" success ");
    Serial.print(_success);
    Serial.print(" timeout ");
    Serial.print(_fail);
    Serial.print(" Failed ");
    Serial.print(_ratio);
    Serial.print("% ");
    for (int _i = 0; _i < _ratio; _i++) Serial.print("*");
    Serial.print(" lightSwitch ");
    Serial.print(robotPak.lightSwitch);
    Serial.println();
    digitalWrite(light, !robotPak.lightSwitch);
    _success = 0;
    _fail = 0;
    _startTime = millis();
  }
}
