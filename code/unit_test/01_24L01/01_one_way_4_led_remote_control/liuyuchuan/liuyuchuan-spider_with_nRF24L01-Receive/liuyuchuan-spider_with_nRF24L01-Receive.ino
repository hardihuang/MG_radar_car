/*
========================================
  24L01无线通信 V0.2
  刘禹川
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
	3 - CE 连接A5
	4 - CSN 连接A4
	5 - SCK 连接D13
	6 - MOSI 连接D11
	7 - MISO 连接D12
	8 - UNUSED

【按键连接】
---------------------------------------
	LED A - 连接 D5
	LED B - 连接 D4
	LED C - 连接 D3
	LED D - 连接 D2
*/


//引入库文件
#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

volatile int t1;
volatile int t2;

//RF24L01配置
RF24 radio(A5, A4); // CE, CSN
const byte address[6] = "00100";

int ledA = 5;
int ledB = 2;
int ledC = 3;
int ledD = 4;

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

Servo servo_7;
Servo servo_6;
Servo servo_3;
Servo servo_2;
Servo servo_5;
Servo servo_4;
Servo servo_9;
Servo servo_8;

void setup() {
  Serial.begin(9600);
  radioSetup();
  ledSetup();
  servo_setup();
}

void loop(){
  getData();
  
}

void getData(){//获取数据内容并打印在屏幕上
  if (radio.available()){
    radio.read( &myData, sizeof(myData) );
	
	//move
  if(myData.joyStick_B_X>700){
    q();  
  }else if(myData.joyStick_B_X<200){
    h();
  }else if(myData.joyStick_B_Y>700){
    r1();
    r2();  
  }else if(myData.joyStick_B_Y<200){
    l1();
    l2();  
  }else{
     stay();
  }

  
	
	//打印内容
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
  delay(50);
    
  } 
}

void radioSetup(){//RF24L01接收端初始化
  radio.begin();
  radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
  radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
  radio.startListening();              //This sets the module as receiver
}

void ledSetup(){
	pinMode(ledA,OUTPUT);
	pinMode(ledB,OUTPUT);
	pinMode(ledC,OUTPUT);
	pinMode(ledD,OUTPUT);
}

//spider control

void stay(){
  servo_3.write(70);
  delay(100);
  servo_5.write(70);
  delay(100);
  servo_7.write(70);
  delay(100);
  servo_9.write(70);
  delay(100);
  servo_2.write(90);
  delay(100);
  servo_4.write(90);
  delay(100);
  servo_6.write(90);
  delay(100);
  servo_8.write(90);
  delay(100);  
}
void r2() {
  servo_7.write(30);
  delay(300);
  servo_6.write(80);
  delay(300);
  servo_7.write(70);
  delay(500);
  servo_3.write(30);
  delay(300);
  servo_2.write(80);
  delay(300);
  servo_3.write(70);
  delay(500);
  servo_5.write(30);
  delay(300);
  servo_4.write(60);
  delay(300);
  servo_5.write(70);
  delay(500);
  servo_9.write(30);
  delay(300);
  servo_8.write(80);
  delay(300);
  servo_9.write(70);
  delay(500);
}

void r1() {
  servo_2.write(130);
  delay(0);
  servo_4.write(130);
  delay(0);
  servo_6.write(130);
  delay(0);
  servo_8.write(130);
  delay(2000);
}

void h() {
  servo_9.write(30);
  delay(t1);
  servo_8.write(80);
  delay(t2);
  servo_9.write(70);
  delay(t1);
  h1();
  servo_3.write(30);
  delay(t1);
  servo_2.write(138);
  delay(t2);
  servo_3.write(70);
  delay(t1);
  servo_7.write(30);
  delay(t1);
  servo_6.write(140);
  delay(t2);
  servo_7.write(70);
  delay(t1);
  h2();
  servo_5.write(30);
  delay(t1);
  servo_4.write(70);
  delay(t2);
  servo_5.write(70);
  delay(t1);
}

void l2() {
  servo_9.write(30);
  delay(300);
  servo_8.write(100);
  delay(300);
  servo_9.write(70);
  delay(500);
  servo_5.write(30);
  delay(300);
  servo_4.write(100);
  delay(300);
  servo_5.write(70);
  delay(500);
  servo_3.write(30);
  delay(300);
  servo_2.write(100);
  delay(300);
  servo_3.write(70);
  delay(500);
  servo_7.write(30);
  delay(300);
  servo_6.write(120);
  delay(300);
  servo_7.write(70);
  delay(500);
}

void q() {
  servo_3.write(30);
  delay(t1);
  servo_2.write(55);
  delay(t2);
  servo_3.write(70);
  delay(t1);
  q1();
  servo_9.write(30);
  delay(t1);
  servo_8.write(120);
  delay(t2);
  servo_9.write(70);
  delay(t1);
  servo_5.write(30);
  delay(t1);
  servo_4.write(110);
  delay(t2);
  servo_5.write(70);
  delay(t1);
  q2();
  servo_7.write(30);
  delay(t1);
  servo_6.write(60);
  delay(t2);
  servo_7.write(70);
  delay(t1);
}

void q1() {
  servo_2.write(90);
  delay(0);
  servo_4.write(60);
  delay(0);
  servo_6.write(90);
  delay(0);
  servo_8.write(60);
  delay(300);
}

void my_0() {
  servo_2.write(120);
  delay(100);
  servo_4.write(90);
  delay(100);
  servo_6.write(60);
  delay(100);
  servo_8.write(90);
  delay(100);
}

void q2() {
  servo_2.write(120);
  delay(0);
  servo_4.write(90);
  delay(0);
  servo_6.write(120);
  delay(0);
  servo_8.write(90 );
  delay(500);
}

void l1() {
  servo_2.write(50);
  delay(0);
  servo_4.write(40);
  delay(0);
  servo_6.write(70);
  delay(0);
  servo_8.write(50);
  delay(2000);
}

void h1() {
  servo_8.write(80);
  delay(0);
  servo_6.write(70);
  delay(0);
  servo_4.write(90);
  delay(0);
  servo_2.write(70);
  delay(300);
}

void h2() {
  servo_8.write(120);
  delay(0);
  servo_6.write(90);
  delay(0);
  servo_4.write(120);
  delay(0);
  servo_2.write(85);
  delay(300);
}

void servo_setup(){
  servo_7.attach(7);
  servo_6.attach(6);
  servo_3.attach(3);
  servo_2.attach(2);
  servo_5.attach(5);
  servo_4.attach(4);
  servo_9.attach(9);
  servo_8.attach(8);
  t1 = 300;
  t2 = 300;
  servo_3.write(70);
  delay(100);
  servo_5.write(70);
  delay(100);
  servo_7.write(70);
  delay(100);
  servo_9.write(70);
  delay(100);
  servo_2.write(90);
  delay(100);
  servo_4.write(90);
  delay(100);
  servo_6.write(90);
  delay(100);
  servo_8.write(90);
  delay(100);
  my_0();
}
