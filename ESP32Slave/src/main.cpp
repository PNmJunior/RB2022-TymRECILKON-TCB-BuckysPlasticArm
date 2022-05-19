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
<<<<<<< HEAD
<<<<<<< HEAD

AsyncWebSocket ws("/ws");

String message = "";
String sliderValue0 = "0";
String sliderValue1 = "0";
String sliderValue2 = "0";
String sliderValue3 = "0";
String sliderValue4 = "0";
String sliderValue5 = "0";
String sliderValue6 = "0";
String sliderValue7 = "0";


//Json Variable to Hold Slider Values
JSONVar sliderValues;

//Get Slider Values
String getSliderValues(){
  sliderValues["sliderValue0"] = String(sliderValue0);
    sliderValues["sliderValue1"] = String(sliderValue1);
  sliderValues["sliderValue2"] = String(sliderValue2);
  sliderValues["sliderValue3"] = String(sliderValue3);
    sliderValues["sliderValue4"] = String(sliderValue4);
  sliderValues["sliderValue5"] = String(sliderValue5);
  sliderValues["sliderValue6"] = String(sliderValue6);
  sliderValues["sliderValue7"] = String(sliderValue7);
  String jsonString = JSON.stringify(sliderValues);
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

float MC0;
float MC1;
float MC2;
float MC3;
float MC4;
float MC5;

float MC6;
float MC7;


float MotProcNow[8];
float MotProcOld[8];

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    message = (char*)data;
    if (message.indexOf("0s") >= 0) {
      sliderValue0 = message.substring(2);
      notifyClients(getSliderValues());
      MC0 = sliderValue0.toFloat();
    }
    if (message.indexOf("1s") >= 0) {
      sliderValue1 = message.substring(2);
      notifyClients(getSliderValues());
      MC1 = sliderValue1.toFloat();
    }    
    if (message.indexOf("2s") >= 0) {
      sliderValue2 = message.substring(2);
      notifyClients(getSliderValues());
      MC2 = sliderValue2.toFloat();
    }
    if (message.indexOf("3s") >= 0) {
      sliderValue3 = message.substring(2);
      notifyClients(getSliderValues());
      MC3 = sliderValue0.toFloat();
    }
    if (message.indexOf("4s") >= 0) {
      sliderValue4 = message.substring(2);
      notifyClients(getSliderValues());
      MC4 = sliderValue1.toFloat();
    }    
    if (message.indexOf("5s") >= 0) {
      sliderValue5 = message.substring(2);
      notifyClients(getSliderValues());
      MC5 = sliderValue2.toFloat();
    }
    if (message.indexOf("6s") >= 0) {
      sliderValue6 = message.substring(2);
      notifyClients(getSliderValues());
      MC6 = sliderValue0.toFloat();
    }
    if (message.indexOf("7s") >= 0) {
      sliderValue7 = message.substring(2);
      notifyClients(getSliderValues());
      MC7 = sliderValue1.toFloat();
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
=======
>>>>>>> parent of cd38d66 (Funkční ovladani prvního motoru....)
=======
>>>>>>> parent of cd38d66 (Funkční ovladani prvního motoru....)
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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 46fecb9ffd800c449089d72d8c6300f0a5a4a358
MotorRunProc(0,MC0);
MotorRunProc(1,MC1);
MotorRunProc(2,MC2);
MotorRunProc(3,MC3);
MotorRunProc(4,MC4);
MotorRunProc(5,MC5);
MotorRunProc(6,MC6);
MotorRunProc(7,MC7);
//MotorRunRaw(0,Mvpred, 255);
//MotorRunProc(1,100);
ws.cleanupClients();

  //delay(1);
=======
  delay(1);
>>>>>>> parent of cd38d66 (Funkční ovladani prvního motoru....)
=======
  delay(1);
>>>>>>> parent of cd38d66 (Funkční ovladani prvního motoru....)
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
