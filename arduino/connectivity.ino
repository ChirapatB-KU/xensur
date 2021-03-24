#include "WiFi.h"
#include "PubSubClient.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

WiFiClient espClient;
PubSubClient client(espClient);

#define SCREEN_WIDTH 128 // pixel ความกว้าง
#define SCREEN_HEIGHT 64 // pixel ความสูง 

// กำหนดขาต่อ I2C กับจอ OLED
#define OLED_RESET     -1 //ขา reset เป็น -1 ถ้าใช้ร่วมกับขา Arduino reset
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int var = 60; // ตัวแปรสำหรับทดสอบแสดงผล
const int ledCount = 6; // the number of LEDs in the bar graph
int ledPins[] = {32, 33, 25, 27, 14,12};
byte start = 1;
byte stopGame = 0;

void digit_timeout(){
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
    {
      pinMode(ledPins[thisLed], OUTPUT);
      digitalWrite(ledPins[thisLed], HIGH);
       delay(100);
       digitalWrite(ledPins[thisLed], LOW);
    }
}

void digit_setup(){
  digit_timeout();
  delay(500);
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
    {
      digitalWrite(ledPins[thisLed], HIGH);
    }
}

void digit_play(int var){
  switch(var){
        case 50:
          digitalWrite(ledPins[0], LOW);
          break;
        case 40:
          digitalWrite(ledPins[1], LOW);
          break;
        case 30:
          digitalWrite(ledPins[2], LOW);
          break;
        case 20:
          digitalWrite(ledPins[3], LOW);
          break;
        case 10:
          digitalWrite(ledPins[4], LOW);
          break;
        case 0:
          digitalWrite(ledPins[5], LOW);
          break;
      }
}

void Starting(){
    OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
    OLED.setTextColor(WHITE, BLACK);  //กำหนดข้อความสีขาว ฉากหลังสีดำ
    OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
    OLED.setTextSize(2); // กำหนดขนาดตัวอักษร
    OLED.print("xenSur");
    OLED.setCursor(0, 20);
    OLED.setTextSize(1);
    OLED.print("starting...");
    OLED.display();
    delay(7000);
    
    //Wire.endTransmission();    // stop transmitting
}

void screen(){
    Wire.begin();
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write("1");              // sends one byte
    Wire.endTransmission();

    Wire.begin();
    Wire.beginTransmission(80); // transmit to device #
    Wire.write("1");
    Wire.endTransmission();   
    OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
    OLED.setCursor(0, 0);
    OLED.setTextSize(2);
    OLED.print("PLAY ");
    while(var>=0){
      OLED.setCursor(60, 0);
      OLED.println(var, DEC);
      OLED.display(); // สั่งให้จอแสดงผล
      digit_play(var);  
      
      if(var <= 9){
        OLED.clearDisplay();
        OLED.setCursor(0, 0);
        OLED.setTextSize(2);
        OLED.print("PLAY ");
        OLED.setCursor(60, 0);
        OLED.println(var, DEC);
        OLED.display(); // สั่งให้จอแสดงผล
      }
      var--;
      delay(1000);
    }
    Wire.begin();
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(stopGame);              // sends one byte
    Wire.endTransmission();    // stop transmitting
    OLED.clearDisplay();
    OLED.print("TIME OUT");
    OLED.display();
    delay(4000);
    Wire.begin();        // join i2c bus (address optional for master)
    Wire.requestFrom(4, 6);    // request 6 bytes from slave device #4
    int score = Wire.read(); // receive bytes
    OLED.clearDisplay();
    OLED.setCursor(0, 0);
    OLED.print("Your Score");
    OLED.println(score, DEC);

     
    OLED.display();
}

void setup() {
  // put your setup code here, to run once:
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  }

  // connect to wifi
  const char* ssid = "xensurnetwork";
  const char* pass = "helloworld";

  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  Serial.print("Connecting to WiFi");

  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print("...");
  }
  Serial.println("\nConnected to the WiFi network");
  ////////////////////////////////////////////////////////////////

  //connect to MQTT
  
  const char* mqttServer = "test.mosquitto.org";
  const int mqttPort = 1883;

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while(!client.connected()){
    Serial.println("Connecting to MQTT...");

    if(client.connect("xensurInput")){
      Serial.println("connected");  
    }else{
      Serial.println("retrying...");
      delay(2000);
    }
  }
  client.publish("xensurStatus", "device ONLINE.");
  client.subscribe("xensurInput");
}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  for(int i=0; i<length; i++){
    Serial.print((char)payload[i]);
  }  
  Serial.println("");

  if(payload[0]=='s'){
    Serial.println("ArdinoAll OLED Start Work !!!");
    digit_setup();
    Starting();
    screen();
  
    for(int i=0; i<4; i++){
      digit_timeout();  
    } 
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("xensurInput")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("xensurInput");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
    reconnect();  
  }
  client.loop();

}