#include <Wire.h>
int ldr1 = A4;
int ldr2 = A5;
int ldr3 = A6;
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

void loop() {
    int ldrVal1 = analogRead(ldr1);
    int ldrVal2 = analogRead(ldr2);
    int ldrVal3 = analogRead(ldr3);
    
    printf("%d %d %d\n", ldrVal1, ldrVal2, ldrVal3);
    delay(500);
}
