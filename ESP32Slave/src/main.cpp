#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <tajnosti.h>//Jsou zde definované jmého a heslo wifi
#include "SPIFFS.h"
#include <Arduino_JSON.h>
/*
#include "soc/dport_reg.h"
#include "soc/ledc_reg.h"
#include "soc/ledc_struct.h"
#include "esp32-hal-ledc.h"
#include "esp32-hal-ledc.C"


 /   #define LEDC_CHAN(g,c) LEDC.channel_group[(g)].channel[(c)]
    #define LEDC_TIMER(g,t) LEDC.timer_group[(g)].timer[(t)]


*/
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

//Get Slider Values
String getSliderValues(){
  sliderValues["sliderValue1"] = String(sliderValue1);
  sliderValues["sliderValue2"] = String(sliderValue2);
  sliderValues["sliderValue3"] = String(sliderValue3);
  sliderValues["sliderValue4"] = String(sliderValue4);
  sliderValues["sliderValue5"] = String(sliderValue5);
  sliderValues["sliderValue6"] = String(sliderValue6);
  sliderValues["sliderValue7"] = String(sliderValue7);
  sliderValues["sliderValue8"] = String(sliderValue8);

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

float MC8;
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
      notifyClients(getSliderValues());
      
    }
    if (message.indexOf("2s") >= 0) {
      sliderValue2 = message.substring(2);
      MC2 = sliderValue2.toFloat();
      notifyClients(getSliderValues());
      
    }    
    if (message.indexOf("3s") >= 0) {
      sliderValue3 = message.substring(2);
      MC3 = sliderValue3.toFloat();
      notifyClients(getSliderValues());
    }
        if (message.indexOf("4s") >= 0) {
      sliderValue4 = message.substring(2);
      MC4 = sliderValue4.toFloat();
      notifyClients(getSliderValues());
    }
        if (message.indexOf("5s") >= 0) {
      sliderValue5 = message.substring(2);
      MC5 = sliderValue5.toFloat();
      notifyClients(getSliderValues());
    }
        if (message.indexOf("6s") >= 0) {
      sliderValue6 = message.substring(2);
      MC6 = sliderValue6.toFloat();
      notifyClients(getSliderValues());
    }
        if (message.indexOf("7s") >= 0) {
      sliderValue7 = message.substring(2);
      MC7 = sliderValue7.toFloat();
      notifyClients(getSliderValues());
    }
        if (message.indexOf("8s") >= 0) {
      sliderValue8 = message.substring(2);
      MC8 = sliderValue8.toFloat();
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
float MprocDataIn[8][2];
float MprocDataWork[8][2];

//definice indexu motorů
#define MotPodLivi 1
#define MotPodPravi 0
#define Mot1 6
#define Mot2 2
#define Mot3 3
#define Mot4 4
#define MotKleste 7
#define LEDka 5

//Negace směru motorů
#define MotNegPodLivi 0
#define MotNegPodPravi 1
#define MotNeg1 0
#define MotNeg2 0
#define MotNeg3 0
#define MotNeg4 0
#define MotNegKleste 1
#define LEDkaNeg 1

int MotorNegace[8] = {MotNegPodPravi,MotNegPodLivi, MotNeg2,MotNeg3,MotNeg4,LEDkaNeg, MotNeg1,MotNegKleste };

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


void IRAM_ATTR myledcWrite(uint8_t chan, uint32_t duty)
{
  ledcWrite(chan,duty);
  /*
    if (chan > 15)
    {
        return;
    }
    uint8_t group = (chan / 8), channel = (chan % 8);
    LEDC_CHAN(group, channel).duty.duty = duty << 4; //25 bit (21.4)
    if (duty)
    {
        LEDC_CHAN(group, channel).conf0.sig_out_en = 1; //This is the output enable control bit for channel
        LEDC_CHAN(group, channel).conf1.duty_start = 1; //When duty_num duty_cycle and duty_scale has been configured. these register won't take effect until set duty_start. this bit is automatically cleared by hardware.
        if (group)
        {
            LEDC_CHAN(group, channel).conf0.low_speed_update = 1;
        }
        else
        {
            LEDC_CHAN(group, channel).conf0.clk_en = 1;
        }
    }
    else
    {
        LEDC_CHAN(group, channel).conf0.sig_out_en = 0; //This is the output enable control bit for channel
        LEDC_CHAN(group, channel).conf1.duty_start = 0; //When duty_num duty_cycle and duty_scale has been configured. these register won't take effect until set duty_start. this bit is automatically cleared by hardware.
        if (group)
        {
            LEDC_CHAN(group, channel).conf0.low_speed_update = 1;
        }
        else
        {
            LEDC_CHAN(group, channel).conf0.clk_en = 0;
        }
    }
    */
}

void MotorStopAll()//Vždy zastaví chod robota. Automaticky posílá data posuvnému registru.
{
  Mdata = 0;
  //portDISABLE_INTERRUPTS();
  PosunReg();
  for (int i = 0; i < Mpocet; i++)
  {
    myledcWrite( MathKanalu i, 0);
  }
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
  //portDISABLE_INTERRUPTS();
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

void blikblik(int pocet = 1)
{
  for (int i = 0; i < pocet; i++)
  {
    MotorRunRaw(LEDka,LEDmod,255);
    delay(100);
    MotorRunRaw(LEDka,MStop);
    delay(100);
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
  initFS();
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
  
 initWebSocket();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

      server.on("/2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index2.html", "text/html");
  });
  
  server.serveStatic("/", SPIFFS, "/");

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");

  //Nasvení procentní nastavení
  MotorSetProc(0,155,255);
  MotorSetProc(1,155,255);
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
      delay(300);
    break;
    default:
    delay(800);
      break;
    }
  }
}

float MC6odl;
unsigned long MC6time;
#define MC6timeSpeed 500
void loop(void) 
{
  if(MC1 == 0 &&MC1 == 0 &&MC2 == 0 &&MC3 == 0 &&MC4 == 0 &&MC5 == 0 &&MC5 == 0 &&MC6 == 0 &&MC7 == 0 &&MC8 == 0)
  {
    MotorStopAll();
  }
  else
  {
 MotorRunProc(MotPodPravi,MC1);
  MotorRunProc(MotPodLivi,MC2);
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
      MotorRunRaw(MotKleste,Mvpred, 50);
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
      MotorRunRaw(MotKleste,Mvzad, 70);
    }
  }
  else
  {
    MotorRunRaw(MotKleste,MStop);
  }
  //MotorRunProc(MotKleste,MC6);
  MotorRunProc(Mot1,MC7);
  MotorRunProc(LEDka,MC8);
  }
 
  ws.cleanupClients();

  
}
