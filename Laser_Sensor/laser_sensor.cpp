//int laser = ;
int sensor = 19;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(1200);
  //pinMode(laser,OUTPUT);
  pinMode(sensor,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(laser,HIGH);
  boolean val = digitalRead(sensor);
  Serial.println(val);
}