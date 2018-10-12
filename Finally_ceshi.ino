#include<Wire.h>
#include <SoftwareSerial.h>//颜色传感器使用软串口
 SoftwareSerial mySerial(A8,A9); // RX, TX

volatile int sure=0,s,x,k,value1,value2,area,color,H;
volatile uint8_t s_h,s_l,x_h,x_l,k_h,k_l;
//颜色传感器
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
if(RGB_H_R()==2)
{
 color=1;//red
  
}
if(RGB_H_R()==201)
{
  color=2;
  //blue
}
if(RGB_H_R()==155)
{
  color=3;
  //绿色
}
if(RGB_H_R()==22)
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
    Serial.println(RGB_H_R());
}
u16 RGB_H_R()
{
  float RGB_min, RGB_max;
  float R,G,B;
  
      R=rgb[0];
      G=rgb[1];
      B=rgb[2];

RGB_max=(R>G?R:G)>B?(R>G?R:G):B;
RGB_min=(R<G?R:G)<B?(R<G?R:G):B;
  
      if(R==RGB_max)   
      H = (G-B)/(RGB_max-RGB_min);
      if (G==RGB_max) 
      H = 2 + (B-R)/(RGB_max-RGB_min);
      if (B==RGB_max)  
      H = 4 + (R-G)/(RGB_max-RGB_min);
      H = H * 60;
      if (H<0)
      H = H + 360;
      if(R>=240&&G>=240&&B>=240)
       H=400;
      return H;
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
  Wire.write(color);//传回颜色值
}

void receive()
{
  int a=Wire.read();
  if(a==8)
  {
    sure = 1;
  }
}
