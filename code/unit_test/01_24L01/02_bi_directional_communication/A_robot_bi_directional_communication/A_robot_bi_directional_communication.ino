#include <SPI.h>
#include <RF24.h>

#define red 5
#define green 4
#define yellow 3
#define blue 2
#define btn 6

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
  pinMode(btn, INPUT_PULLUP);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(blue, OUTPUT);
  
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(blue, LOW);
  
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
  if ( radio.available()){
    radio.read(&remotePak, sizeof(remotePak));
    robotPak.lightSwitch = digitalRead(btn);
    radio.stopListening();
    radio.write( &robotPak, sizeof(robotPak) );
    Serial.print("Sent response ");
    radio.startListening();
    
    digitalWrite(red, !remotePak.buttonA);
    digitalWrite(green, !remotePak.buttonB);
    digitalWrite(yellow, !remotePak.buttonC);
    digitalWrite(blue, !remotePak.buttonD);
  }
  Serial.println(" ");
}
