//https://primalcortex.wordpress.com/2020/10/11/stm32-blue-pill-board-arduino-core-and-usb-serial-output-on-platformio/
#include <Arduino.h>
#include <komunProtokol.h>


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
#define Jx 1
#define Jy 0
#define Jt 2


const int zmena = 2;
const int nullDet = 20;


const int polePinu[4][3] = {
  {J1x,J1y,J1t},
  {J2x,J2y,J2t},
  {J3x,J3y,J3t},
  {J4x,J4y,J4t}
};


int poleNow[4][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0}
};


int poleSet[4][3] = {
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0}
};


void SendPrint(String a)
{
  Serial.print(a);
  Serial1.print(a);
}


void SendPrintln(String a)
{
  Serial.println(a);
  Serial1.println(a);
}


void nacteni(int Joys)
{
  poleNow[Joys][0] = analogRead(polePinu[Joys][0]);
  poleNow[Joys][0] = analogRead(polePinu[Joys][0]);
  poleNow[Joys][0] = digitalRead(polePinu[Joys][0]);
}


typedef double resDuty;


resDuty mapD(resDuty x, resDuty in_min, resDuty in_max, resDuty out_min, resDuty out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


int AnalogToProc(int in, int set)
{
  int o = in- set;
  int out;
  if(abs(o) < nullDet)
  {
    return 0;
  }
  else if(o > 0)
  {
    out = mapD(o, 0,4096 - set,0,100);
  }
  else
  {
    out = (-1)*mapD((-1)*o,0,set,0,100);
  }
  if (out > 100)
  {
    return 100;
  }
  //SendPrint("in"); SendPrintln(String(in));SendPrint("  out"); SendPrintln(String(out));
  return out;
}


void Novy()
{
  for (int j = 0; j < 4; j++)
  {
    int x = analogRead(polePinu[j][Jx]);
    int y = analogRead(polePinu[j][Jy]);
    int t = digitalRead(polePinu[j][Jt]);

    int X = (-1)*AnalogToProc(x,poleSet[j][Jx]);
    int Y = AnalogToProc(y,poleSet[j][Jy]);
    int T = 1-t;

    if (abs(X - poleNow[j][Jx]) > zmena || abs(Y - poleNow[j][Jy]) > zmena || abs(T - poleNow[j][Jt]) == 1)
    {
      poleNow[j][Jx] = X;
      poleNow[j][Jy] = Y;
      poleNow[j][Jt] = T;
      //String pp = "J" + String(j) + "*" + String(poleNow[j][Jx]) + "*" + String(poleNow[j][Jy]) + "*" + String(poleNow[j][Jt]);
      //SendPrintln(pp);
      SendPrint( SendSystem.joystic(j + 1,poleNow[j][Jx],poleNow[j][Jy],poleNow[j][Jt]));
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(19200); // PA10  (RX) PA9 (TX) 
  pinMode(ledPin,OUTPUT);
  //Android pomoci CDC
  analogReadResolution(12);
  Serial.begin(9600);
  delay(2000);
  for (int j = 0; j < 4; j++)
  {
    //SendPrint("Set");SendPrint(String(j));
    for (int i = 0; i < 2; i++)
    {
      poleSet[j][i] = analogRead(polePinu[j][i]);
      //SendPrint("*");SendPrint(String(poleSet[j][i]));
    }
    pinMode(polePinu[j][Jt],INPUT_PULLUP);
    poleSet[j][Jt] = digitalRead(polePinu[j][Jt]);
    //SendPrint("*");SendPrintln(String(poleSet[j][Jt])); 
  }
}


void loop() {
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  delay(50);
  Novy();
}
