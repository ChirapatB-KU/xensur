int sensor = 35;
int led = 34;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  pinMode(sensor,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(laser,HIGH);
  boolean val = digitalRead(sensor);
  if(!val){
    digitalWrite(led,HIGH);
  }else{
    digitalWrite(led,LOW);
  }
  delay(10);
  Serial.println(val);
}