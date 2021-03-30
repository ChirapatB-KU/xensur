#include <Wire.h>
int ldr1 = A4;
int ldr2 = A5;
int ldr3 = A6;
int ldr1t = 0;
int ldr2t = 0;
int ldr3t = 0;
int score = 0;
int state = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ldr1,INPUT);
  pinMode(ldr2,INPUT);
  pinMode(ldr3,INPUT);

  Serial.begin(9600);           // start serial for output
    
  printf("LDR started\n");
}

void receiveState(int howMany)
{
  int x = Wire.read();    // receive byte as an integer
  if(x == 1){
    score = 0;
    state = 1;
  }else{
    state = 0;
  }
  Serial.println(x);         // print the integer
}

void sentScore(){
  Wire.write(score);
}

void calibrate(){
  int ldrVal1 = analogRead(ldr1);
  int ldrVal2 = analogRead(ldr2);
  int ldrVal3 = analogRead(ldr3);

  ldr1t = ldrVal1+50;
  ldr2t = ldrVal2+50;
  ldr3t = ldrVal3+50;
}

void loop() {
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveState); // register event
  
  int ldrVal1 = analogRead(ldr1);
  int ldrVal2 = analogRead(ldr2);
  int ldrVal3 = analogRead(ldr3);

  if(state==1){
      if(ldrVal1>=ldr1t||ldrVal2>=ldr2t||ldrVal3>=ldr3t){
          score++;
      }
  }
  
//     printf("%d %d %d\n", ldrVal1, ldrVal2, ldrVal3);
//     delay(500);
}
