/*
========================================
  24L01无线通信 V0.2
  高禛言
========================================



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

*/


//引入库文件
#include <SPI.h>
#include <RF24.h>

//RF24L01配置
RF24 radio(A3, A4); // CE, CSN
const byte address[6] = "00010";


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

void setup() {
  Serial.begin(9600);
  radioSetup();

}

void loop(){
  getData();
}

void getData(){//获取数据内容并打印在屏幕上
  if (radio.available()){
    radio.read( &myData, sizeof(myData) );
	
	//点亮对应LED

  
	
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
