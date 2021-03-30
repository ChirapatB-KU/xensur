#include <Wire.h>
int ldr1 = A4;
int ldr2 = A5;
int ldr3 = A6;
int ldr1t = 0;
int ldr2t = 0;
int ldr3t = 0;
int score = 0;
int state = 0;
int played = 0;

void receiveState(int howMany)
{
  int x = Wire.read();    // receive byte as an integer
  if(x == 1){
    score = 0;
    state = 1;
  }else if(x == 2){
    state = 2;
  }else{
    state = 0;
  }
  Serial.println(x);         // print the integer
}

void sentScore(){
  Wire.write(score);
}

void calibrate(){
  delay(2000);

  int ldrVal1 = analogRead(ldr1);
  int ldrVal2 = analogRead(ldr2);
  int ldrVal3 = analogRead(ldr3);

  ldr1t = ldrVal1+50;
  ldr2t = ldrVal2+50;
  ldr3t = ldrVal3+50;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(ldr1,INPUT);
  pinMode(ldr2,INPUT);
  pinMode(ldr3,INPUT);

  Serial.begin(9600);           // start serial for output

  calibrate();    
  printf("LDR started\n");
}

void loop() {
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveState); // register event
  
  int ldrVal1 = analogRead(ldr1);
  int ldrVal2 = analogRead(ldr2);
  int ldrVal3 = analogRead(ldr3);

  if(state==1){
      played = 1;
      if(ldrVal1>=ldr1t||ldrVal2>=ldr2t||ldrVal3>=ldr3t){
          score++;
      }
  }else if(state==2){
      calibrate();
  }else{
      if(played==1){
        Wire.begin(4);                // join i2c bus with address #8
        Wire.onRequest(sentScore); // register event
      }
  }
  
//     printf("%d %d %d\n", ldrVal1, ldrVal2, ldrVal3);
//     delay(500);
}