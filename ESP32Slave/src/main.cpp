#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <tajnosti.h>//Jsou zde definované jmého a heslo wifi

const char* host = "BuckysPlasticArm";
const char* ssid = wifiName;
const char* password = wifiHeslo;

//WebServer server(80);
AsyncWebServer server(80);
/*
 * setup function
 */

//Motor definice
#define Mpocet 8
int Mpiny[Mpocet] = {32,33,23,19,27,13,26,25};//bila-modra,cerv-fial,zel-bila,zlut-seda,fial-bila,mod-fial,fial-bila,seda-cerv,
byte Mdata = 0;
//32-bila-modra,33-cerv-fial,23-zel-bila,19-zlut-seda,27-fial-bila,13-mod-fial,26-seda-cerv,25-bila-zel
#define MStopLow 0
#define MStopHight 10
#define MStop MStopLow
#define Mvpred 1
#define Mvzad -1
#define MEindex -1
#define MEmod -2
int Mfreq = 1000; 
int Mresolution = 8; 
int MlatchPin = 18;
int MdataPin = 17;
int MclockPin = 16;
float MprocDataIn[8][2];
float MprocDataWork[8][2];

//definice indexu motorů
#define MotPodLivi 1
#define MotPodPravi 0
#define Mot1 6
#define Mot2 2
#define Mot3 3
#define Mot4 5
#define MotKleste 4
#define LEDka 7

//Negace směru motorů
#define MotNegPodLivi 0
#define MotNegPodPravi 1
#define MotNeg1 0
#define MotNeg2 1
#define MotNeg3 0
#define MotNeg4 1
#define MotNegKleste 0
#define LEDkaNeg 0

int MotorNegace[8] = {MotNegPodPravi,MotNegPodLivi, MotNeg2,MotNeg3,MotNegKleste, MotNeg4, MotNeg1,LEDkaNeg };

//Ostatní definice k motorům
#define LEDmod Mvpred
#define KlesSevrit Mvzad
#define KlesRozevrit Mvpred
#define Mot1Vlevo Mvzad
#define Mot1Vprevo Mvpred
#define Dolu Mvpred
#define Nahoru Mvzad

//test2 motorů
long timeTest1=0;
byte mmm=0;

void PosunReg()//odešle data k posuvnému registru
{
    digitalWrite(MlatchPin, LOW);
    shiftOut(MdataPin, MclockPin, MSBFIRST, Mdata);
    digitalWrite(MlatchPin, HIGH);  
}


void PosunReg(byte dataNov )//Pokud nastane změna výstupu posuvného registru, pošlou se data
{
  if (dataNov != Mdata)
  {
    Mdata = dataNov;
    PosunReg();
  }
}


void PosRegIndex(int index, bool A_N)//Vyrobení změny ve výstupu pos. registru
{
  if (A_N)
  {
    PosunReg((int)(Mdata | (1 << index)));
  }
  else
  {
    PosunReg(Mdata & (~(1 << index)));
  }
}


void MotorStopAll()//Vždy zastaví chod robota. Automaticky posílá data posuvnému registru.
{
  Mdata = 0;
  PosunReg();
  for (int i = 0; i < Mpocet; i++)
  {
    ledcWrite(i, 0);
  }
}


void MotorSetProc(int index, byte minimum, byte maximum)// nastavení MotorRunProc
{
  MprocDataIn[index][0] = minimum;
  MprocDataIn[index][1] = maximum;
  MprocDataWork[index][0] = MprocDataIn[index][0] ;
  MprocDataWork[index][1] = MprocDataIn[index][1]  -MprocDataIn[index][0] ;
}


void MotorBegin(int Platch, int Pdata, int Pclock, int piny[Mpocet])//inicializace potřebných zaležitostí k motorům
{
  MlatchPin =Platch;
  MdataPin = Pdata;
  MclockPin = Pclock;

  for (int i = 0; i < Mpocet; i++)
  {
    Mpiny[i] = piny[i];
    ledcSetup(i, Mfreq, Mresolution);
    ledcWrite(i, 255);
  }
  pinMode(MlatchPin,OUTPUT);
  pinMode(MdataPin,OUTPUT);
  pinMode(Pclock,OUTPUT);

  Mdata = B11111111;
  PosunReg();
  for (int i = 0; i < Mpocet; i++)
  {
    pinMode(Mpiny[i],OUTPUT);
    ledcAttachPin(Mpiny[i], i);
  }
    for (int i = 0; i < Mpocet; i++)
  {
    MotorSetProc(i,0,255);
  }
}


void MotorRunRaw(int index, int mod=MStop, int rychlost =0)
{
  if (rychlost < 0)
  {
    rychlost = 0;
  }
  else if (rychlost > 255)
  {
    rychlost = 255;
  }
  if (MotorNegace[index]==1)
  {
    if (mod == Mvpred)
    {
      mod = Mvzad;
    }
    else if (mod == Mvzad)
    {
      mod = Mvpred;
    }
  }
  
  if (mod == MStopLow)
  {
    ledcWrite(index, 0);
    PosRegIndex(index, 0);
  }
  else if (mod == MStopHight)
  {
    ledcWrite(index, 255);
    PosRegIndex(index, 1);
  }
  else if (mod == Mvpred)
  {
    ledcWrite(index, rychlost);
    PosRegIndex(index, 0 );
  }
  else if (mod == Mvzad)
  {
    ledcWrite(index,255- rychlost);
    PosRegIndex(index, 1);
  }
}


void MotorRunProc(int index, float proc)//Možnost nastavení procent výkonu, kde je ovládaný reálný výkon motoru...
{
  if(proc == 0.0)
  {
    MotorRunRaw(index,MStop);
  }
  else
  {
    float o  =(abs(proc)*MprocDataWork[index][1]/100.0)+MprocDataWork[index][0];
    int u = o;
    if (u >255)
    {
      u = 255;
    }
    else if (u < 0)
    {
      u = 0;
    }
    if (proc < 0.0)
    {
      MotorRunRaw(index,Mvzad, u);
    }
    else
    {
      MotorRunRaw(index, Mvpred, u);
    }
  }
}


void setup() {
  // put your setup code here, to run once:
MotorBegin(18,17,16,Mpiny);

  MotorStopAll();
  //dotykový senzor
  touchRead(2);
  touchRead(3);

  Serial.begin(9600);

   WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}


void loop(void) 
{
  delay(1);
  timeTest1 ++;
  //byte mmm=0;
  //test
  /*
  if (timeTest1 == 500)
  {
    MotorRunRaw(mmm,Mvpred,(int)150);
  }
    if (timeTest1 == 1000)
  {
    MotorRunRaw(mmm,MStopLow,(int)150);
  }
    if (timeTest1 == 1500)
  {
    MotorRunRaw(mmm,Mvzad,(int)150);
  }
    if (timeTest1 == 2000)
  {
    MotorRunRaw(mmm,MStopHight,(int)150);
  
    if (mmm == 7)
    {
      mmm=0;
    }
    else
    {
      mmm++;
    }
    timeTest1=0;
    
  }*/

}
