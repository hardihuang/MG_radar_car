/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
int pos = 0;    // variable to store the servo position
int distance;
int data[180];

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600); 
}

void loop() {
  
  servoRotate();
  
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

void servoRotate(){

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    distance = getDistance();
    Serial.print("angle: ");
    Serial.print(pos);
    Serial.print(" distance: ");
    Serial.println(distance);
    myservo.write(pos);
    data[pos]=distance;
    /*
    for(int i=0;i<180;i++){
       Serial.print(i); 
       Serial.print(" "); 
       Serial.print(data[i]); 
       Serial.print(" || "); 
    }
    Serial.println();
    */
    delay(50);
  }
  delay(100000);
 
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    distance = getDistance();
    Serial.print("angle: ");
    Serial.print(pos);
    Serial.print(" distance: ");
    Serial.println(distance);
    myservo.write(pos);
    data[pos]=distance;
    delay(100);
  }
  

}
