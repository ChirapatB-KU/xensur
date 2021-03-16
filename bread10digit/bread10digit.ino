const int ledCount = 6; // the number of LEDs in the bar graph
//const int buttonPin = 8; 
//int buttonState = 0; 

int ledPins[] = {32, 33, 25, 27, 14,12}; 

void digit_setup(){
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
  {
       digitalWrite(ledPins[thisLed], HIGH);
       delay(500);
       digitalWrite(ledPins[thisLed], LOW);
  }
}

void digit_play(){
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
  {
    digitalWrite(ledPins[thisLed], HIGH);
  }
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
  {
    delay(10000);
    digitalWrite(ledPins[thisLed], LOW);
  }
}



void setup() {
  // loop over the pin array and set them all to output and high
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
  {
    pinMode(ledPins[thisLed], OUTPUT);
    digitalWrite(ledPins[thisLed], HIGH);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  digit_setup();
  delay(5000);
  digit_play();
  delay(500);
  digit_setup();
  delay(10000);

}