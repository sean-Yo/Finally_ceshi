/////////////////////
/*
  GY33----MINI
  VCC----VCC
  GND----GND
  1:RX---TX,send A5 01 A6 to GY-33
  2:TX---RX
  3:MINI_TX---FT232_RX
*/
//////////////////
#include<Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(A8,A9); // RX, TX
volatile int sure=0,s,x,k,value1,value2,area,color;
volatile uint8_t s_h,s_l,x_h,x_l,k_h,k_l;
unsigned char Re_buf[11], counter = 0;
unsigned char sign = 0;
byte rgb[3] = {0};
//激光
float JG_R1[5];
float JG_R2[5];
float JG_B[5];
//光电对管
#define switch1 3 
#define switch2 4
void setup() {
    Wire.begin(8);
  Serial.begin(9600);
 // Serial.print("hello");
  mySerial.begin(9600);
  delay(1);
  mySerial.write(0XA5);
  mySerial.write(0X01);    //初始化,连续输出模式
  mySerial.write(0XA6);    //初始化,连续输出模式
  //激光
  Serial1.begin(19200);
  Serial2.begin(19200);
  Serial3.begin(19200);
  //光电对管
  pinMode(switch1,INPUT);
  pinMode(switch2,INPUT);
    Wire.onReceive(receive);
  Wire.onRequest(request);
}

void loop() {
  unsigned char i = 0, sum = 0;
  if (sign == 1)
  {
    //Serial.println("h");
    sign = 0;
    for (i = 0; i < 7; i++)
      sum += Re_buf[i];
    if (sum == Re_buf[i] )     //检查帧头，帧尾
    {
      rgb[0] = Re_buf[4];
      rgb[1] = Re_buf[5];
      rgb[2] = Re_buf[6];
      Serial.print("r:");
      Serial.print( rgb[0], DEC);
      Serial.print(",g:");
      Serial.print( rgb[1], DEC);
      Serial.print(",b:");
      Serial.println( rgb[2], DEC);
    }
  }
  //Serial.println("e");
  while (mySerial.available()) {
    byte  readedchar = mySerial.read();
   // Serial.write(readedchar);
    Re_buf[counter] = (unsigned char)readedchar;
    if (counter == 0 && Re_buf[0] != 0x5A) return; // 检查帧头
    counter++;
    if (counter == 8)             //接收到数据
    {
      counter = 0;               //重新赋值，准备下一帧数据的接收
      sign = 1;
    }
  }
area=rgb[0]*rgb[0]+rgb[1]*rgb[1]+rgb[2]*rgb[2];
if(area>13143&&area<14143)
{
 color=1;//red
  
}
if(area>18174&&area<19174)
{
  color=2;
  //blue
}
if(area>8710&&area<9710)
{
  color=3;
  //绿色
}
if(area>15569&&area<16569)
{
  color=4;//黄色
}
Serial.print(color);
   //#第一激光
    Serial1.write("F");
    for (int i = 0; i < 2; i++)
    JG_R1[i] = Serial1.parseFloat();//0.01m 2.33m
    s =100*JG_R1[0];//变成233cm
    //分低八位高八位
    s_h = s>>8;s_l=(s&0xff);
    
    Serial.print(s);
    Serial.print("  ");
    
    //#第二激光
    Serial2.write("F");
    for (int i = 0; i < 2; i++)
    JG_R2[i] = Serial2.parseFloat();
    x =100*JG_R2[0];
    //分低八位高八位
    x_h = x>>8;x_l=(x&0xff);
    
    Serial.print(x);
    Serial.print("  ");
    
    //#第三激光
    Serial3.write("F");
    for (int i = 0; i < 2; i++)
    JG_B[i] = Serial3.parseFloat();
    k=100*JG_B[0];
    //分低八位高八位
    k_h = k>>8;k_l=(k&0xff);
    
    Serial.println(k);
    
    //#光电对管
    value1=digitalRead(switch1);
    value2=digitalRead(switch2);
    Serial.print(value1);
    Serial.println(value2);
}
void request()
{
  //Serial.println("666");
  //bytes 1- 8
  Wire.write(s_h);
  Wire.write(s_l);
  Wire.write(x_h);
  Wire.write(x_l);
  Wire.write(k_h);
  Wire.write(k_l);
  Wire.write(value1);
  Wire.write(value2);
  Wire.write(color);
}

void receive()
{
  int a=Wire.read();
  if(a==8)
  {
    sure = 1;
  }
}
