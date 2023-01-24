#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <disp.h>
#include <motor.h>
#include <komunProtokol.h>
#include <enkoder.h>
#include <menu.h>
#include "SPIFFS.h"
#include "SPIFFS.h"      //nastaveni webu
#include <Preferences.h> //Ulozeni hesla


Preferences preferences;

String WifiNotPassword = "@";

// nove definice
#define pin_PR_Clock 17
#define pin_PR_Data1 18
#define pin_PR_Data2 4
#define pin_PR_Latch 16
#define pin_Servo2 22
#define pin_Servo3 23
#define pin_Servo1 2
#define pin_m0 32
#define pin_m1 33
#define pin_m2 25
#define pin_m3 23
#define pin_m4 19
#define pin_m5 13
#define pin_m6 12
#define pin_m7 26
#define pin_LedD1 5
#define pin_LedD2 15
#define pin_LedD3 27
#define pin_LedD4 14
#define pin_Tlac 35
#define pin_EnkA 39
#define pin_EnkB 34
#define pin_EnkDP 36


#define M_LED 0//5
#define M_1 1//0
#define M_2 2//1
#define M_3 3//2
#define M_4 4//3
#define M_Kleste 5//4
#define M_Levy 6//6
#define M_Pravy 7//7

const byte Prevodnik[8] = {5,0,1,2,3,4,6,7};


disp segDisp;
motor mot;
// komun kom;
hw_timer_t *My_timer = NULL;
//enkoder enk;

byte posunIP = 0;

byte pocetClientu = 0 ;

void MotorStopAll() // Vždy zastaví chod robota.
{
  for (int i = 0; i < 8; i++)
  {
    mot.input(i);
  }
}

void IRAM_ATTR HlavniPreruseni()
{
  byte d1 = mot.vystup();
  //Serial.println(d1);
  //byte d2 = B10000001;
  byte d2 = segDisp.vystup();
  //Serial.println(d2,2);
  byte d3 = segDisp.index();

  digitalWrite(pin_PR_Latch, LOW);
  //delayMicroseconds(10);
  for (uint8_t i = 0; i < 8; i++)//7 - i
  {
    if ((d1>>i)&1)
    {
      digitalWrite(pin_PR_Data1, 1);
    }
    else
    {
      digitalWrite(pin_PR_Data1, 0);
    }
    if ((d2>>i)&1)
    {
      digitalWrite(pin_PR_Data2, 1);     
    }
    else
    {
      digitalWrite(pin_PR_Data2, 0);
    }
    
    //delayMicroseconds(10);
    digitalWrite(pin_PR_Clock, HIGH);
    //delayMicroseconds(10);
    mot.setPWM(i);
    digitalWrite(pin_PR_Clock, LOW);
    //delayMicroseconds(10);
  }

  mot.updatePWM();

  switch (d3)
  {
  case 0:
    digitalWrite(pin_LedD4, 1);
    digitalWrite(pin_LedD1, 0);
    break;
  case 1:
    digitalWrite(pin_LedD1, 1);
    digitalWrite(pin_LedD2, 0);
    break;
  case 2:
    digitalWrite(pin_LedD2, 1);
    digitalWrite(pin_LedD3, 0);
    break;
  case 3:
    digitalWrite(pin_LedD3, 1);
    digitalWrite(pin_LedD4, 0);
    break;

  default:
    digitalWrite(pin_LedD1, 1);
    digitalWrite(pin_LedD2, 1);
    digitalWrite(pin_LedD3, 1);
    digitalWrite(pin_LedD4, 1);
    break;
  }

  digitalWrite(pin_PR_Latch, HIGH);

  timerRestart(My_timer);
  segDisp.vystupEX();
}

String ssid ;
String password ;

// WebServer server(80);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
String message = "";


// Json Variable to Hold Slider Values


// Get Slider Values
String getSliderValues(int index = 100)
{
  String st;
  if (index == 100)
  {
    for (size_t i = 0; i < 8; i++)
    {
      st += SendSystem.motor(i,mot.outProc(i) );
      //st += ";m:" + String(i) + ':'+String(sliderValue[i]);
    }
  }
  else if(index >= 0 && index < 8)
  {
    st = SendSystem.motor(index,mot.outProc(Prevodnik[index]) );
  }

  return st;
}

void initFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");
  }
}

void inline notifyClients(String sliderValues)
{
  ws.textAll(sliderValues);
}


void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    message = (char *)data;
    //;m:0:2;m:1:-100;t:1:pocet:"joiuyytuiopiuytrddxhhi jj   m  ijmk"
    komunProtokol komP;
    komP.begin(message);
    //Serial.println("message:");
    //Serial.println(message);
    String outAll;
    String outClient;
    for (int s = 0; s < komP.pocetSouboru; s++)
    {
      String souName = komP.readStr();
      if (souName.length() != 1)
      {
        Serial.println("Problem s velikosti:");
        Serial.println(souName);
        Serial.println(souName.length());
        return;
      }
      int cisloM;
      int smer;
      switch (souName.charAt(0))
      {
      case 'm':

        if (komP.pocetVAktualSouboru() != 3)
        {
          Serial.println("Problem s velikosti M:");
          Serial.println(komP.pocetVAktualSouboru());
          return;
        }
        cisloM = komP.readInt();
        smer = komP.readInt();
        /*Serial.print("c:");
        Serial.println(cisloM);
        Serial.print("h:");
        Serial.println(smer);*/
        mot.inputProc(Prevodnik[cisloM], smer);
        outAll += komP.sendAktSoubor();
        break;
      case 'M':
        if (komP.pocetVAktualSouboru() != 2)
        {
          Serial.println("Problem s velikosti M:");
          Serial.println(komP.pocetVAktualSouboru());
          return;
        }
        outClient += getSliderValues(komP.readInt());
        break;
      case 'A':
        if (komP.pocetVAktualSouboru() != 1)
        {
          Serial.println("Problem s velikosti M:");
          Serial.println(komP.pocetVAktualSouboru());
          return;
        }
        outClient += getSliderValues();
        break;
      default:
        Serial.println("neznama vec");
        Serial.println(souName.charAt(0));
        outAll += getSliderValues();
        break;
      }
    }
    //komP.clear();
  
    Serial.println("outAll:");
    Serial.println(outAll);
    //ws._cleanBuffers();
    if (outAll != "")
    {
      Serial.println("outAllp1:");
      ws.textAll(outAll);
      //ws._clients;
      /*
      for(const auto& c: ws.getClients()){
        c->text(outAll);
      }*/
      Serial.println("outAllp2:");
    }
    Serial.print("outClient:");
    Serial.println(client->id());
    Serial.println(outClient);
    if (outClient != "")
    {
      //client->text(outClient);
      //ws.text(client->id(),outClient);
      ws.textAll(outClient);
    }
    //ws._cleanBuffers();
  }
}

void PrClent(IPAddress ip)
{
  segDisp.addText4IP(ip,2000);
  segDisp.addText4("Cl"+String(pocetClientu));
}

void onEvent2(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    segDisp.addText4("ConC",1000);
    pocetClientu ++;
    PrClent(client->remoteIP());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    segDisp.addText4("DisC",1000);
    pocetClientu --;
    PrClent(client->remoteIP());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len, client);
    break;
  case WS_EVT_PONG:
  Serial.printf("Ping from %s\n", client->remoteIP().toString().c_str());
    segDisp.addText4("ping",1000);
    PrClent(client->remoteIP());
  case WS_EVT_ERROR:
  Serial.printf("errr from %s\n", client->remoteIP().toString().c_str());
    segDisp.addText4("errr",1000);
    PrClent(client->remoteIP());
    break;
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent2);
  server.addHandler(&ws);
}

void inline addWifi(String ssid, String password)
{
  preferences.putString(ssid.c_str(), password);
}

String inline getPassswordWifi(String ssid, String WNPass = WifiNotPassword)
{
  return preferences.getString(ssid.c_str(), WNPass);
}

bool inline isSaweWifi(String ssid)
{
  return preferences.isKey(ssid.c_str());
}

void MenuWifiAdd(komunFace komF)
{
  int p;
  do
  {
    segDisp.del();
    segDisp.addText4Char('.','.','.','.');
    menu w;
    w.begin(komF,"Saznam Wifi",5000);
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
    {
      Serial.print(i);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      w.addPolozkaData(i,i+100,WiFi.SSID(i),500*WiFi.SSID(i).length());
    }
    w.addPolozkaData(-1,-1,"_upd",2000);
    w.addPolozkaData(-2,-2,"_clo",2000);
    p = w.work();
    if (p >= 100)
    {
      addWifi(WiFi.SSID(p-100),"");
    }
    else if (p >= 0)
    {
      String heslo = editText.WriteTextVyber("_");
      if (heslo != "" && heslo != "_")
      {
        addWifi(WiFi.SSID(p),heslo);
      }
      else
      {
        p = -1;
      }
    }
  } while (p == -1);
}

void MenuWifiSet(komunFace komF)
{
  int p;
  do
  {
    segDisp.del();
    segDisp.addText4Char('.','.','.','.');
    menu w;
    w.begin(komF,"UrciWifi",3000);
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++)
    {
      Serial.print(i);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.print((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      if(isSaweWifi(WiFi.SSID(i)))
      {
        w.addPolozkaData(i,i,String(WiFi.SSID(i)),2000);
        Serial.println("+");
      }
    }
    w.addPolozkaData(-1,-1,"_upd",2000);
    w.addPolozkaData(-2,-2,"_add",2000);
    p = w.work();
    if(p == -2)
    {
      MenuWifiAdd(komF);
      p = -1;
    }
    else if (p >= 0)
    {
      ssid = WiFi.SSID(p);
      password  = getPassswordWifi(ssid);
    }
  } while (p == -1);
}


void setup()
{
  // put your setup code here, to run once:
  
  pinMode(pin_PR_Clock, OUTPUT);
  pinMode(pin_PR_Data1, OUTPUT);
  pinMode(pin_PR_Data2, OUTPUT);
  pinMode(pin_PR_Latch, OUTPUT);

  pinMode(pin_LedD1, OUTPUT);
  pinMode(pin_LedD2, OUTPUT);
  pinMode(pin_LedD3, OUTPUT);
  pinMode(pin_LedD4, OUTPUT);
  digitalWrite(pin_LedD1, 1);
  digitalWrite(pin_LedD2, 1);
  digitalWrite(pin_LedD3, 1);
  digitalWrite(pin_LedD4, 1);
  Serial.begin(9600);
  Serial.println("Inicializace pinu");
  segDisp.begin(&Serial, B00000001, B00000100, B00010000, B01000000, B00100000, B00000010, B00001000, B10000000);
  Serial.println("Displaj.begin");
  // zahajeni ...
  
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &HlavniPreruseni, true);
  timerAlarmWrite(My_timer, 5000, true);
  timerAlarmEnable(My_timer); // Just Enable
  
  Serial.println("Inicializace a spusteni hlavniho presuseni");
  mot.beginStart();
  mot.beginTimer();

  mot.begin(0, pin_m0, LEDC_CHANNEL_0, false, false, 255, 0);
  mot.begin(1, pin_m1, LEDC_CHANNEL_1, false, false, 255, 0);
  mot.begin(2, pin_m2, LEDC_CHANNEL_2, false, false, 255, 0);
  mot.begin(3, pin_m3, LEDC_CHANNEL_3, false, false, 255, 0);
  mot.begin(4, pin_m4, LEDC_CHANNEL_4, false, false, 255, 0);
  mot.begin(5, pin_m5, LEDC_CHANNEL_5, false, false, 255, 0);
  mot.begin(6, pin_m6, LEDC_CHANNEL_6, false, false, 255, 0);
  mot.begin(7, pin_m7, LEDC_CHANNEL_7, false, false, 255, 0);
  if (mot.beginEnd() == 0)
  {
    segDisp.addText4("Err Motor");
  }

  MotorStopAll();

  Serial.println("Motor");

  enk.begin(pin_EnkA, pin_EnkB, pin_EnkDP, pin_Tlac);

  komunFace komF;
  komF.displej = &segDisp;
  komF.enk = &enk;
  komF.ser = &Serial;

  editText.begin(komF);
  
  menu Vr1;
  Vr1.begin(komF);
   
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
Serial.println("Dalsi Inicailizace");

/*
  File setWeb0 = SPIFFS.open("/setWeb0.txt");
  File setWeb1 = SPIFFS.open("/setWeb1.txt");
  File setWeb2 = SPIFFS.open("/setWeb2.txt");
  File setWeb3 = SPIFFS.open("/setWeb3.txt");
  File setWeb4 = SPIFFS.open("/setWeb4.txt");
  File setWeb5 = SPIFFS.open("/setWeb5.txt");
  File setWeb6 = SPIFFS.open("/setWeb6.txt");
  File setWeb7 = SPIFFS.open("/setWeb7.txt");
*/
  preferences.begin("credentials", false);
Serial.println("Soubory");
  initFS();
  WiFi.mode(WIFI_STA);
  Serial.println("WIFI_STA");
  MenuWifiSet(komF);
  password.remove(password.length()-1);//Upravuje heslo, odstaranuje char 13 z konce
  WiFi.begin(ssid.c_str(), password.c_str());
  // Wait for connection
  Serial.println("Pripojovani se");
  Serial.println();
  byte teckyCekani=0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
    segDisp.addText4(" ",0,segDisp.BitSet(3-(teckyCekani % 4)));
    teckyCekani++;
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  segDisp.addText4IP(WiFi.localIP(),10000);
  initWebSocket();
  Serial.println("initWebSocket");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    MotorStopAll();
    server.reset();
    MotorStopAll(); });
  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request)
            { MotorStopAll(); });

  server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/joy.html", "text/html"); });
  server.serveStatic("/", SPIFFS, "/");
  AsyncElegantOTA.begin(&server); // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");
}


void loop(void)
{
  int e = enk.Enk();
  if (e != 0)
  {
    if (e == 1)
    {
      posunIP ++;
    }
    else if (e == -1)
    {
      posunIP --;
    }
    segDisp.addText4IPIndex(posunIP%4,WiFi.localIP());    
  }
  
  // Až dokončím komunikační protokol, bude fungovat zastavovaní brždění
  // dataSend p;
  // p.leng = 0;

  for (int i = 0; i < 8; i++)
  {
    if (mot.TStop(i) != 0)
    {
      if (mot.TStop(i) < millis())
      {
        mot.input(i, mStop);
        // p = ADDdataRow(p,motorSend(i,0,0));
      }
    }
  }
  // kom.send(p);
  ws.cleanupClients();
}
