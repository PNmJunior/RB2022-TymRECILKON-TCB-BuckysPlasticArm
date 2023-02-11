#include <Arduino.h>
#define J1x PA0
#define J1y PA1
#define J1t PB0
#define J2x PA2
#define J2y PA3
#define J2t PB1
#define J3x PA4
#define J3y PA5
#define J3t PB10
#define J4x PA6
#define J4y PA7
#define J4t PB11

#define ledPin PC13 //13

#define J1 0
#define J2 1
#define J3 2
#define J4 3
#define Jx 0
#define Jy 1
#define Jt 2

const int polePinu[4][3] = {
  {J1x,J1y,J1t},
  {J2x,J2y,J2t},
  {J3x,J3y,J3t},
  {J4x,J4y,J4t}
};

int poleOld[4][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0}
};
int poleNow[4][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0}
};


void modpinJoys(int Joys)
{
  pinMode(polePinu[Joys][0],INPUT_ANALOG);
  pinMode(polePinu[Joys][1],INPUT_ANALOG);
  pinMode(polePinu[Joys][2],INPUT_PULLUP);
}


void nacteni(int Joys)
{
  poleNow[Joys][0] = analogRead(polePinu[Joys][0]);
  poleNow[Joys][0] = analogRead(polePinu[Joys][0]);
  poleNow[Joys][0] = digitalRead(polePinu[Joys][0]);
}


void nacteni()
{
  for (int i = 0; i < 4; i++)
  {
    nacteni(i);
  }
}


bool zmena(int Joys)
{
  if (poleNow[Joys][0] != poleOld[Joys][0] || poleNow[Joys][1] != poleOld[Joys][1] || poleNow[Joys][2] != poleOld[Joys][2])
  {
    poleOld[Joys][0] = poleNow[Joys][0];
    poleOld[Joys][1] = poleNow[Joys][1];
    poleOld[Joys][2] = poleNow[Joys][2];
    return 1;
  }
  return 0;
}

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(19200); // PA10  (RX) PA9 (TX) 
  pinMode(ledPin,OUTPUT);
  //Android pomoci CDC
   analogReadResolution(12);
  Serial.begin(9600);
  for (int i = 0; i < 4; i++)
  {
    modpinJoys(i);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  nacteni();
  for (int i = 0; i < 4; i++)
  {
    //if(zmena(i))
    if(true)
    {
      Serial1.print("J");
      Serial1.print(i + 1);
      Serial1.print(": x=");
      Serial1.print(poleNow[i][Jx]);
      Serial1.print(", y=");
      Serial1.print(poleNow[i][Jy]);
      Serial1.print(", t=");
      Serial1.println(poleNow[i][Jt]);
    }
  }
  Serial.println("Ahoj");
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
}