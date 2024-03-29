#include <SPI.h>
#include <RF24.h>
#define Urx A0
#define Ury A1
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#define OLED_RESET 4
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
Adafruit_SH1106 display(OLED_RESET);
#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif
//RF24L01配置
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00010";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
int dotX = 1;
//定义输入按键引脚
int btnA = 2;
int btnB = 3;
int joystickA = A0;
int joystickB = A1;
int joystickC = A2;
int joystickD = A3;

//发送的数据结构
struct dataStruct {
  unsigned long _micros;
  bool btnA;
  bool btnB;
  int joyStick_A_X;
  int joyStick_A_Y;
  int joyStick_B_X;
  int joyStick_B_Y;
} myData;

/*-----初始化函数-----*/
void setup() {
  Serial.begin(9600);
  btnSetup();
  radioSetup();
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  delay(1000);
  display.clearDisplay();
  
}

/*-----主循环函数-----*/
void loop() {
  sendKey();
  delay(50);
  Serial.print(myData.btnA);
  Serial.print(" ");
  Serial.print(myData.btnB);
  Serial.print(" ");
  Serial.print(myData.joyStick_A_X);
  Serial.print(" ");
  Serial.print(myData.joyStick_A_Y);
  Serial.print(" ");
  Serial.print(myData.joyStick_B_X);
  Serial.print(" ");
  Serial.print(myData.joyStick_B_Y);
  Serial.println(" ");
}

/*-----自定义函数-----*/

void sendKey() { //读取并发送按键电平信号
  myData.btnA = !digitalRead(btnA);
  myData.btnB = !digitalRead(btnB);
  myData.joyStick_A_X = analogRead(joystickA);
  myData.joyStick_A_Y = analogRead(joystickB);
  myData.joyStick_B_X = analogRead(joystickC);
  myData.joyStick_B_Y = analogRead(joystickD);
  myData._micros = micros();
  radio.write(&myData, sizeof(myData));
}

void btnSetup() { //设置按键为输入并内部上拉
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(joystickA, INPUT);
  pinMode(joystickB, INPUT);
  pinMode(joystickC, INPUT);
  pinMode(joystickD, INPUT);

}

void radioSetup() { //RF24L01发射端初始化
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}
