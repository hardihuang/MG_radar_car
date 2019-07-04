/*
========================================
  24L01无线通信 V0.1
  by Hardi Huang  (2019.05.05)
========================================


【说明】
---------------------------------------
本代码使用arduino nano作为单片机，使用Nrf24L01-2.4GHz模块作为无线通信模块
每块arduino连接一个无线模块，一方为发射端(连接4个按键)，另一方为接收端(连接4只led灯)
按下发射端的某个按键，接收端即可点亮相对应的led灯

将模块和arduino按照下方接线方式连接即可



【24L01模块接线】(模块引脚参考资料文件夹图片)
---------------------------------------
	1 - GND
	2 - VCC 连接3.3V
	3 - CE 连接D7
	4 - CSN 连接D8
	5 - SCK 连接D13
	6 - MOSI 连接D11
	7 - MISO 连接D12
	8 - UNUSED

【按键连接】
---------------------------------------
	按钮A - 连接 D2
	按钮B - 连接 D3
	按钮C - 连接 D4
	按钮D - 连接 D5
*/


//引入库文件
#include <SPI.h>
#include <RF24.h>

//RF24L01配置
//RF24 radio(7, 8);
RF24 radio(A3, A2); // CE, CSN         
const byte address[6] = "00010";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.

//定义输入按键引脚
int btnA = 2;
int btnB = 3;
int joystickA = A1;
int joystickB = A0;
int joystickC = A4;
int joystickD = A5;

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
}

/*-----主循环函数-----*/
void loop(){
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

void sendKey(){//读取并发送按键电平信号
  myData.btnA = !digitalRead(btnA);
  myData.btnB = !digitalRead(btnB);
  myData.joyStick_A_X = analogRead(joystickA);
  myData.joyStick_A_Y = analogRead(joystickB);
  myData.joyStick_B_X = analogRead(joystickC);
  myData.joyStick_B_Y = analogRead(joystickD);
  myData._micros = micros();
  radio.write(&myData, sizeof(myData));
}

void btnSetup(){//设置按键为输入并内部上拉
  pinMode(btnA, INPUT_PULLUP);
  pinMode(btnB, INPUT_PULLUP);
  pinMode(joystickA, INPUT);
  pinMode(joystickB, INPUT);
  pinMode(joystickC, INPUT);
  pinMode(joystickD, INPUT);
 
}

void radioSetup(){//RF24L01发射端初始化
  radio.begin();                  //Starting the Wireless communication
  radio.openWritingPipe(address); //Setting the address where we will send the data
  radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.stopListening();          //This sets the module as transmitter
}
