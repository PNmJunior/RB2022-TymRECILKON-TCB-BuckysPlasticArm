#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <tajnosti.h>//Jsou zde definované jmého a heslo wifi
#include "SPIFFS.h"
#include <Arduino_JSON.h>



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
int Mfreq = 2000; 
int Mresolution = 8; 
int MlatchPin = 18;
int MdataPin = 17;
int MclockPin = 16;
float MprocDataIn[9][2];
float MprocDataWork[9][2];

//definice indexu motorů
#define M0 0
#define M1 1 
#define M2 2 
#define M3 3
#define M4 4
#define M5 5
#define M6 6
#define M7 7
#define Mled 8

//definice Motoru
#define MotPodLivy M1
#define MotPodPravy M0
#define Mot1 M6
#define Mot2 M5
#define Mot3 M7
#define Mot4 M4
#define MotKleste M3
#define LEDka Mled
#define MotZaloha M2

//Negace směru motorů
#define MotNegPodLivi 1
#define MotNegPodPravi 0
#define MotNeg1 0
#define MotNeg2 1
#define MotNeg3 1
#define MotNeg4 1
#define MotNegKleste 0
#define MotNegZaloha 0

int MotorNegace[8] = {MotNegPodPravi,MotNegPodLivi,MotNegZaloha ,MotNegKleste,MotNeg4,MotNeg2, MotNeg1,MotNeg3 };

//Ostatní definice k motorům
#define LEDzelena Mvpred
#define LEDcervena Mvzad
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

#define MathKanalu 2*

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


#define ContrSendDataVolno 0
#define ContrSendDataRow 1
#define ContrSendDataStopAll 2
int ContrSendData =0;
unsigned long ContrSendDataTime =0;
bool ContrSendDataRemove = false;
int  ContrSendDataPocet = 0;

void WaitContrSendData(int cekat, int zmena)
{
  while (cekat != ContrSendData)
  {
    delay(1);
  }
  ContrSendData = zmena;
}



void IRAM_ATTR myledcWrite(uint8_t chan, uint32_t duty)
{
  ledcWrite(chan,duty);
}

void MotorStopAll(unsigned long cekani = 0, bool mazZac = false, bool mazKonec= false)//Vždy zastaví chod robota. Automaticky posílá data posuvnému registru.
{
  ContrSendDataRemove = mazZac;
WaitContrSendData(ContrSendDataVolno,ContrSendDataStopAll);
  Mdata = 0;
  //portDISABLE_INTERRUPTS();
  PosunReg();
  for (int i = 0; i < Mpocet; i++)
  {
    myledcWrite( MathKanalu i, 0);
  }

  if (cekani != 0)
  {
    unsigned long abn = cekani + millis();
    while (abn >=micros())
    {
      ContrSendDataTime = abn - millis();
      delay(1);
      
    }
    
  }
  ContrSendDataTime = 0;

  ContrSendData = ContrSendDataVolno;
  ContrSendDataRemove = mazKonec;
  //portENABLE_INTERRUPTS();
}


void MotorSetProc(int index, byte minimum = 0, byte maximum=255)// nastavení MotorRunProc
{
  MprocDataIn[index][0] = minimum;
  MprocDataIn[index][1] = maximum;
  MprocDataWork[index][0] = MprocDataIn[index][0] ;
  MprocDataWork[index][1] = (MprocDataIn[index][1]  -MprocDataIn[index][0])/100.0 ;
}



void MotorBegin(int Platch, int Pdata, int Pclock, int piny[Mpocet])//inicializace potřebných zaležitostí k motorům
{
  MlatchPin =Platch;
  MdataPin = Pdata;
  MclockPin = Pclock;

  for (int i = 0; i < Mpocet; i++)
  {
    Mpiny[i] = piny[i];
    ledcSetup(MathKanalu i, Mfreq, Mresolution);
    ledcWrite(MathKanalu i, 255);
  }
  pinMode(MlatchPin,OUTPUT);
  pinMode(MdataPin,OUTPUT);
  pinMode(Pclock,OUTPUT);

  Mdata = B11111111;
  PosunReg();
  for (int i = 0; i < Mpocet; i++)
  {
    pinMode(Mpiny[i],OUTPUT);
    ledcAttachPin(Mpiny[i], MathKanalu i);
  }
    for (int i = 0; i < Mpocet; i++)
  {
    MotorSetProc(i,0,255);
  }
MotorSetProc(LEDka,0,255);

  pinMode(15,OUTPUT);
  pinMode(2,OUTPUT);
  ledcSetup(5, Mfreq, Mresolution);
    ledcWrite(5, 255);
    ledcAttachPin(15, 5);
      ledcSetup(7, Mfreq, Mresolution);
    ledcWrite(7, 255);
    ledcAttachPin(2,7);
  
}







const char* host = "BuckysPlasticArm";
const char* ssid = wifiName;
const char* password = wifiHeslo;



//WebServer server(80);
AsyncWebServer server(80);

AsyncWebSocket ws("/ws");

String message = "";
String sliderValue1 = "0";
String sliderValue2 = "0";
String sliderValue3 = "0";
String sliderValue4 = "0";
String sliderValue5 = "0";
String sliderValue6 = "0";
String sliderValue7 = "0";
String sliderValue8 = "0";


//Json Variable to Hold Slider Values
JSONVar sliderValues;
JSONVar slider1Values;
JSONVar slider2Values;
JSONVar slider3Values;
JSONVar slider4Values;
JSONVar slider5Values;
JSONVar slider6Values;
JSONVar slider7Values;
JSONVar slider8Values;

//Get Slider Values
String getSliderValues (int a =100){
  String jsonString;

  switch (a)
  {
  case 100:
      sliderValues["sV1"] = String(sliderValue1);
  sliderValues["sV2"] = String(sliderValue2);
  sliderValues["sV3"] = String(sliderValue3);
  sliderValues["sV4"] = String(sliderValue4);
  sliderValues["sV5"] = String(sliderValue5);
  sliderValues["sV6"] = String(sliderValue6);
  sliderValues["sV7"] = String(sliderValue7);
  sliderValues["sV8"] = String(sliderValue8);
    jsonString = JSON.stringify(sliderValues);
    break;
    case 1:
    slider1Values["sV1"] = String(sliderValue1);
    jsonString = JSON.stringify(slider1Values);
    break;
    case 2:
    slider2Values["sV2"] = String(sliderValue2);
    jsonString = JSON.stringify(slider2Values);
    break;
    case 3:
    slider3Values["sV3"] = String(sliderValue3);
    jsonString = JSON.stringify(slider3Values);
    break;
    case 4:
    slider4Values["sV4"] = String(sliderValue4);
    jsonString = JSON.stringify(slider4Values);
    break;
    case 5:
    slider5Values["sV5"] = String(sliderValue5);
    jsonString = JSON.stringify(slider5Values);
    break;
    case 6:
    slider6Values["sV6"] = String(sliderValue6);
    jsonString = JSON.stringify(slider6Values);
    break;
    case 7:
    slider7Values["sV7"] = String(sliderValue7);
    jsonString = JSON.stringify(slider7Values);
    break;
    case 8:
    slider8Values["sV8"] = String(sliderValue8);
    jsonString = JSON.stringify(slider8Values);
    break;
    
  default:
    break;
  }
  
  //jsonString = JSON.stringify(sliderValues);
  return jsonString;
}


void initFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else{
   Serial.println("SPIFFS mounted successfully");
  }
}

void notifyClients(String sliderValues) {
  ws.textAll(sliderValues);
}

float MC8;
float MC8old;
float MC1;
float MC2;
float MC3;
float MC4;
float MC5;

float MC6;
float MC7;




void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    message = (char*)data;
    if (message.indexOf("1s") >= 0) {
      sliderValue1 = message.substring(2);
      MC1 = sliderValue1.toFloat();
      notifyClients(getSliderValues(1));
      
    }
    if (message.indexOf("2s") >= 0) {
      sliderValue2 = message.substring(2);
      MC2 = sliderValue2.toFloat();
      notifyClients(getSliderValues(2));
      
    }    
    if (message.indexOf("3s") >= 0) {
      sliderValue3 = message.substring(2);
      MC3 = sliderValue3.toFloat();
      notifyClients(getSliderValues(3));
    }
        if (message.indexOf("4s") >= 0) {
      sliderValue4 = message.substring(2);
      MC4 = sliderValue4.toFloat();
      notifyClients(getSliderValues(4));
    }
        if (message.indexOf("5s") >= 0) {
      sliderValue5 = message.substring(2);
      MC5 = sliderValue5.toFloat();
      notifyClients(getSliderValues(5));
    }
        if (message.indexOf("6s") >= 0) {
      sliderValue6 = message.substring(2);
      MC6 = sliderValue6.toFloat();
      notifyClients(getSliderValues(6));
    }
        if (message.indexOf("7s") >= 0) {
      sliderValue7 = message.substring(2);
      MC7 = sliderValue7.toFloat();
      notifyClients(getSliderValues(7));
    }
        if (message.indexOf("8s") >= 0) {
      sliderValue8 = message.substring(2);
      MC8 = sliderValue8.toFloat();
      notifyClients(getSliderValues(8));
        }

        if (message.indexOf("9s") >= 0) {
      sliderValue1 = "0";
               MC1=0;
      sliderValue2 = "0";
               MC2=0;
      sliderValue3 = "0";
               MC3=0;
      sliderValue4 = "0";
               MC4=0;
      sliderValue5 = "0";
               MC5=0;
      sliderValue6 = "0";
               MC6=0;
      sliderValue7 = "0";
               MC7=0;
      sliderValue8 = "0";
               MC8=0;
      MotorStopAll();
      notifyClients(getSliderValues());
        }
    if (strcmp((char*)data, "getValues") == 0) {
      notifyClients(getSliderValues());
      
    }
  }
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
/*
 * setup function
 */



void KompenzaceIn(int index)
{
  int dvojiceA = index>>1;
  int dvojiceB;
  dvojiceA << 1;
  if (dvojiceA != index)
  {
    dvojiceB = dvojiceA +1;
  }
  else
  {
    dvojiceB = dvojiceA;
  }
  

  
}
void KompenzaceWork()
{

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

  if(index == LEDka)
  {
    if (mod == LEDzelena)
    {
      myledcWrite( 5, rychlost);
      myledcWrite( 7,0);
    }
    else if (mod ==LEDcervena)
    {
      myledcWrite( 7, rychlost);
      myledcWrite( 5,0);
    }
    else
    {
      myledcWrite( 5,0);
      myledcWrite( 7,0);
    }
  return;
  }

  if (ContrSendDataRemove == true)
  {
    return;
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
  //portDISABLE_INTERRUPTS();
  WaitContrSendData(ContrSendDataVolno, ContrSendDataRow);
  if (mod == MStopLow)
  {
    myledcWrite( MathKanalu index, 0);
    PosRegIndex(index, 0);
  }
  else if (mod == MStopHight)
  {
    myledcWrite( MathKanalu index, 255);
    PosRegIndex(index, 1);
  }
  else if (mod == Mvpred)
  {
    myledcWrite( MathKanalu index, rychlost);
    PosRegIndex(index, 0 );
  }
  else if (mod == Mvzad)
  {
    myledcWrite( MathKanalu index,255- rychlost);
    PosRegIndex(index, 1);
  }
  //portENABLE_INTERRUPTS();
  ContrSendData =0;
}


void MotorRunProc(int index, float proc)//Možnost nastavení procent výkonu, kde je ovládaný reálný výkon motoru...
{
  if(proc == 0.0)
  {
    MotorRunRaw(index,MStop);
  }
  
  else
  {
    float o  =(abs(proc)*MprocDataWork[index][1])+MprocDataWork[index][0];
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

void TestM()
{
  timeTest1 ++;
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
  }
}

void blikblik(int mod=LEDcervena,int pocet = 1, int cas=100)
{
  for (int i = 0; i < pocet; i++)
  {
    MotorRunRaw(LEDka,mod,255);
    delay(cas);
    MotorRunRaw(LEDka,MStop);
    delay(cas);
  }
  
}


void setup() {
  // put your setup code here, to run once:
  MotorBegin(18,17,16,Mpiny);

  MotorStopAll();

  Serial.begin(9600);
  initFS();
   WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
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
  
 initWebSocket();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

        server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    MotorStopAll(0, true, true);
    ContrSendDataPocet = 0;
    server.reset();
    MotorStopAll(100, true, false);
    
  });

          server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    MotorStopAll(100,true,false);
  });
  
  server.serveStatic("/", SPIFFS, "/");

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");

  //Nasvení procentní nastavení
  MotorSetProc(0,122,255);
  MotorSetProc(1,122,255);
  MotorSetProc(2,0,255);
  MotorSetProc(3,0,255);
  MotorSetProc(4,0,255);
  MotorSetProc(5,0,255);
  MotorSetProc(6,0,255);
  MotorSetProc(7,0,255);


    String ip100 = WiFi.localIP().toString();
  for (int i = 0; i < ip100.length(); i++)
  {
    
    switch (ip100.charAt(i))
    {
    case '0':
      blikblik();
    case '9':
      blikblik();
    case '8':
      blikblik();
    case '7':
      blikblik();
    case '6':
      blikblik();
    case '5':
      blikblik();
    case '4':
      blikblik();
    case '3':
      blikblik();
    case '2':
      blikblik();
    case '1':
      blikblik();
      blikblik(LEDzelena);
    break;
    default:
    blikblik(LEDzelena,2);
      break;
    }
  }
}

float MC6odl;
unsigned long MC6time;
#define MC6timeSpeed 500
void loop(void) 
{
  if(ContrSendData == ContrSendDataStopAll)
  {
    delayMicroseconds(100);
  }
  else if(MC1 == 0 &&MC1 == 0 &&MC2 == 0 &&MC3 == 0 &&MC4 == 0 &&MC5 == 0 &&MC5 == 0 &&MC6 == 0 &&MC7 == 0)
  {

    MotorStopAll();
    delayMicroseconds(10);
  }
  else
  {
 MotorRunProc(MotPodPravy,MC1);
  MotorRunProc(MotPodLivy,MC2);
  MotorRunProc(Mot2,MC3);
  MotorRunProc(Mot3,MC4);
  MotorRunProc(Mot4,MC5);
  if (MC6odl != MC6)
  {
    MC6odl = MC6;
    MC6time= millis() + MC6timeSpeed ;
  }
  
  if (MC6 ==100.0)
  {
    if (MC6time > millis())
    {
      MotorRunRaw(MotKleste,Mvpred, 200);
    }
    else
    {
      MotorRunRaw(MotKleste,Mvpred, 70);
    }
    
    
  }
  else if (MC6 ==-100.0)
  {
    if (MC6time > millis())
    {
    MotorRunRaw(MotKleste,Mvzad, 200);
    }
    else
    {
      MotorRunRaw(MotKleste,Mvzad, 80);
    }
  }
  else
  {
    MotorRunRaw(MotKleste,MStop);
  }
  //MotorRunProc(MotKleste,MC6);
  MotorRunProc(Mot1,MC7);
  
  }
  if(MC8 != MC8old)
  {
    MotorRunProc(LEDka,MC8);
    MC8old = MC8;
  }
  
  ws.cleanupClients();

  
}
