#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  //H mustek
  pinMode(32,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(19,OUTPUT);

//posuvny registr
  pinMode(18,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(16,OUTPUT);

  //dotikovy senzor
  touchRead(2);
  touchRead(3);
}

void loop() {
  // put your main code here, to run repeatedly:
}