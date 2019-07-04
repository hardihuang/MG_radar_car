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
	LED A - 连接 D5
	LED B - 连接 D4
	LED C - 连接 D3
	LED D - 连接 D2
*/


//引入库文件
#include <SPI.h>
#include <RF24.h>

//RF24L01配置
RF24 radio(A3, A2); // CE, CSN
const byte address[6] = "00010";

int ledA = 5;
int ledB = 2;
int ledC = 3;
int ledD = 4;

//接收的数据结构
struct dataStruct {
  unsigned long _micros;
  bool btnA;
  bool btnB;
  bool btnC;
  bool btnD;
} myData;

void setup() {
  Serial.begin(9600);
  radioSetup();
  ledSetup();
}

void loop(){
  getData();
}

void getData(){//获取数据内容并打印在屏幕上
  if (radio.available()){
    radio.read( &myData, sizeof(myData) );
	
	//点亮对应LED
	if(myData.btnA){digitalWrite(ledA,HIGH);}else{digitalWrite(ledA,LOW);}
	if(myData.btnB){digitalWrite(ledB,HIGH);}else{digitalWrite(ledB,LOW);}
	if(myData.btnC){digitalWrite(ledC,HIGH);}else{digitalWrite(ledC,LOW);}
	if(myData.btnD){digitalWrite(ledD,HIGH);}else{digitalWrite(ledD,LOW);}
	
	//打印内容
	Serial.print("btnA: ");
    Serial.print(myData.btnA);
    Serial.print(" ||btnB: ");
    Serial.print(myData.btnB);
    Serial.print(" ||btnC: ");
    Serial.print(myData.btnC);
    Serial.print(" ||btnD: ");
    Serial.println(myData.btnD);
	
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
