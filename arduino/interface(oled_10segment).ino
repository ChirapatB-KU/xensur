#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // pixel ความกว้าง
#define SCREEN_HEIGHT 64 // pixel ความสูง 

// กำหนดขาต่อ I2C กับจอ OLED
#define OLED_RESET     -1 //ขา reset เป็น -1 ถ้าใช้ร่วมกับขา Arduino reset
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int var = 60; // ตัวแปรสำหรับทดสอบแสดงผล
const int ledCount = 6; // the number of LEDs in the bar graph
int ledPins[] = {32, 33, 25, 27, 14,12};

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
}

void screen(){
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
    OLED.clearDisplay();
    OLED.print("TIME OUT");
    OLED.display();
    delay(4000);
    OLED.clearDisplay();
    OLED.setCursor(0, 0);
    OLED.print("Your Score");


     
    OLED.display();
}

void setup() {
  Serial.begin(9600);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("ArdinoAll OLED Start Work !!!");
    digit_setup();
    Starting();
    screen();
  }
}

void loop() {
  digit_timeout();
}