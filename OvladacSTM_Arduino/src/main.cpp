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


const int zmena = 2;
const int nullDet = 20;


#define Jx 1
#define Jy 0
#define Jt 2
const int polePinu[4][3] = {
  {J1x,J1y,J1t},
  {J2x,J2y,J2t},
  {J3x,J3y,J3t},
  {J4x,J4y,J4t}
};

#define Jx_min 0
#define Jx_max 1
#define Jx_stred 2
#define Jx_stred_max 3
#define Jx_stred_min 4
#define Jx_hod 5

#define Jy_max 6
#define Jy_stred 7
#define Jy_stred_max 8
#define Jy_stred_min 9
#define Jy_min 10
#define Jy_hod 11

#define Jt_hod 12
#define Jx_To_Jy 6

#define deatZone 0.05


int poleSys[4][13]
{
  {0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0}
};

#define SendOldJx 0
#define SendOldJy 1
#define SendOldJt 2

int sendOld[4][3]
{
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


typedef double resDuty;


resDuty mapD(resDuty x, resDuty in_min, resDuty in_max, resDuty out_min, resDuty out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int AnalogToProc(int j, int i = 0)
{
  if((poleSys[j][Jx_stred_max+i] >= poleSys[j][Jx_hod+i]) && (poleSys[j][Jx_stred_min+i] <= poleSys[j][Jx_hod+i]))
  {
    return 0;
  }
  else if(poleSys[j][Jx_hod+i] > poleSys[j][Jx_stred+i])
  {
    return mapD(poleSys[j][Jx_hod+i], poleSys[j][Jx_stred_max+i],poleSys[j][Jx_max+i],0,100);
  }
  else
  {
    return -100 + mapD(poleSys[j][Jx_hod+i], poleSys[j][Jx_min+i],poleSys[j][Jx_stred_min+i],0,100);
  }
  return 0;
}


void deatZoneVypocet(int j, int i = 0)
{
  poleSys[j][Jx_stred_max+i] = poleSys[j][Jx_stred+i] + (poleSys[j][Jx_max+i] - poleSys[j][Jx_stred+i])*deatZone;
  poleSys[j][Jx_stred_min+i] = poleSys[j][Jx_stred+i] + (poleSys[j][Jx_min+i] - poleSys[j][Jx_stred+i])*deatZone;
}


void kontrol(int j, int x, int y)
{
  if ( poleSys[j][Jx_min] > x)
  {
    poleSys[j][Jx_min] = x;
    deatZoneVypocet(j);
  }
  else if ( poleSys[j][Jx_max] < x)
  {
    poleSys[j][Jx_max] = x;
    deatZoneVypocet(j);
  }

  if ( poleSys[j][Jy_min] > y)
  {
    poleSys[j][Jy_min] = y;
    deatZoneVypocet(j,Jx_To_Jy);
  }
  else if ( poleSys[j][Jx_max] < y)
  {
    poleSys[j][Jy_max] = y;
    deatZoneVypocet(j,Jx_To_Jy);
  }
}


void Novy()
{
  bool vystup[4];
  int vystuppocet = 0;
  for (int j = 0; j < 4; j++)
  {
    int x = analogRead(polePinu[j][Jx]);
    int y = analogRead(polePinu[j][Jy]);
    int t = digitalRead(polePinu[j][Jt]);

    kontrol(j,x,y);
    poleSys[j][Jx_hod] = x;
    poleSys[j][Jy_hod] = y;
    poleSys[j][Jt_hod] = t;



    int X = (-1)*AnalogToProc(j);
    int Y = AnalogToProc(j,Jx_To_Jy);
    int T = 1-t;
    delay(1000);
    Serial.println(j);
    Serial.println(X);
    Serial.println(Y);
    Serial.println(T);


    if (abs(X - sendOld[j][SendOldJx]) > zmena || abs(Y - sendOld[j][SendOldJy]) > zmena || abs(T - sendOld[j][SendOldJt]) == 1)
    {
      vystup[j]=true;
      vystuppocet ++;

      sendOld[j][SendOldJx] = X;
      sendOld[j][SendOldJy] = Y;
      sendOld[j][SendOldJt] = T;
      
    }
    else
    {
      vystup[j]=false;
    }
  }
  Serial.println(vystuppocet);
  if (vystuppocet == 4)
  {
    Serial.print(
      SendSystem.joysticStart() +
      SendSystem.joystickAll(
      sendOld[J1][SendOldJx],sendOld[J1][SendOldJy],sendOld[J1][SendOldJt],
      sendOld[J2][SendOldJx],sendOld[J2][SendOldJy],sendOld[J2][SendOldJt],
      sendOld[J3][SendOldJx],sendOld[J3][SendOldJy],sendOld[J3][SendOldJt],
      sendOld[J4][SendOldJx],sendOld[J4][SendOldJy],sendOld[J4][SendOldJt]  
      )
      + SendSystem.joysticStop());
  }
  else if(vystuppocet >= 1)
  {
    String pl = SendSystem.joysticStart();
    for (int j = 0; j < 4; j++)
    {
      pl += SendSystem.joystic(j,sendOld[j][SendOldJx],sendOld[j][SendOldJy],sendOld[j][SendOldJt]);
    }
    Serial.print(pl + SendSystem.joysticStop());

  }
}


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(19200); // PA10  (RX) PA9 (TX) 
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin, HIGH);
  //Android pomoci CDC
  analogReadResolution(12);
  Serial.begin(115200);
  Serial.dtr(0);
  delay(1000);
  for (int j = 0; j < 4; j++)
  {
    int x = analogRead(polePinu[j][Jx]);
    int y = analogRead(polePinu[j][Jy]);
    poleSys[j][Jx_max] = x;
    poleSys[j][Jx_stred] = x;
    poleSys[j][Jx_min] = x;
    poleSys[j][Jx_hod] = x;
    poleSys[j][Jy_max] = y;
    poleSys[j][Jy_stred] = y;
    poleSys[j][Jy_min] = y;
    poleSys[j][Jy_hod] = y;
    deatZoneVypocet(j);
  }

  digitalWrite(ledPin, LOW);
  int timeS = millis() + 10000;
  while (millis() < timeS)
  {
    for (int j = 0; j <4; j++)
    {
      int x = analogRead(polePinu[j][Jx]);
      int y = analogRead(polePinu[j][Jy]);
      kontrol(j,x,y);
    }
    if ((( timeS - millis())%1000)< 500)
    {
      digitalWrite(ledPin, LOW);
    }
    else
    {
      digitalWrite(ledPin, HIGH);
    }
  }

  for (int j = 0; j < 4; j++)
  {
    pinMode(polePinu[j][Jt],INPUT_PULLUP);
    int t = digitalRead(polePinu[j][Jt]);
    poleSys[j][Jt_hod] = t;

    sendOld[j][SendOldJx] = poleSys[j][Jx_hod];
    sendOld[j][SendOldJy] = poleSys[j][Jy_hod];
    sendOld[j][SendOldJt] = poleSys[j][Jt_hod];
  }
}


void loop() {
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
  delay(50);
  Novy();
}
