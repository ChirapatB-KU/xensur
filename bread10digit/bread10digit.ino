const int ledCount = 10; // the number of LEDs in the bar graph
 
int ledPins[] = {
32, 33, 34, 35, 25, 26, 27, 14,12,13
}; // an array of pin numbers to which LEDs are attached
 
void setup()
{
  // loop over the pin array and set them all to output and high
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
  {
    pinMode(ledPins[thisLed], OUTPUT);
    digitalWrite(ledPins[thisLed], HIGH);
  }
}
 
void loop()
{
 
  // loop over the LED array:
  for (int thisLed = 0; thisLed <= ledCount; thisLed++)
  {
    digitalWrite(ledPins[thisLed], LOW);
    delay(500);
  }
 
  for (int thisLed = 0; thisLed < ledCount; thisLed++)
  {
    digitalWrite(ledPins[thisLed], HIGH);
    delay(500);
  }
}
