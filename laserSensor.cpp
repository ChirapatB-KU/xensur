#include <Wire.h>
int sensor1 = 35;
int led = 2;
//buzzer = ;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  //pinMode(buzzer,OUTPUT);
  pinMode(sensor,INPUT);
}
byte x = 0;

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(laser,HIGH);
  boolean val = digitalRead(sensor);
  if(!val){
    digitalWrite(led,HIGH);
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(1);              // sends one byte
    Wire.endTransmission();    // stop transmitting
    //tone(buzzer,450);delay(500);
    //noTone(buzzer);
  }else{
    digitalWrite(led,LOW);
    //noTone(buzzer);
  }
  delay(10);
  Serial.println(val);
}