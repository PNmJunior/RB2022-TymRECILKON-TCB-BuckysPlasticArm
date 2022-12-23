#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <disp.h>
#include <motor.h>
//#include <komun.h>
//#include <komunBasic.h>
#include <enkoder.h>
#include <menu.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
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

disp segDisp;
motor mot;
// komun kom;
hw_timer_t *My_timer = NULL;
//enkoder enk;

byte posunIP = 0;

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
String sliderValue1 = "0";
String sliderValue2 = "0";
String sliderValue3 = "0";
String sliderValue4 = "0";
String sliderValue5 = "0";
String sliderValue6 = "0";
String sliderValue7 = "0";
String sliderValue8 = "0";

// Json Variable to Hold Slider Values
JSONVar sliderValues;
JSONVar slider1Values;
JSONVar slider2Values;
JSONVar slider3Values;
JSONVar slider4Values;
JSONVar slider5Values;
JSONVar slider6Values;
JSONVar slider7Values;
JSONVar slider8Values;


// Get Slider Values
String getSliderValues(int index = 100)
{
  String jsonString;
  switch (index)
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

  // jsonString = JSON.stringify(sliderValues);
  return jsonString;
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

float MC8;
float MC8old;
float MC1;
float MC2;
float MC3;
float MC4;
float MC5;
float MC6;
float MC7;

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    message = (char *)data;
    if (message.indexOf("1s") >= 0)
    {
      MC1 = sliderValue1.toFloat();
      sliderValue1 = message.substring(2);
      mot.inputProc(0, sliderValue1.toFloat());
      notifyClients(getSliderValues(1));
    }
    if (message.indexOf("2s") >= 0)
    {
      MC2 = sliderValue2.toFloat();
      sliderValue2 = message.substring(2);
      mot.inputProc(1, sliderValue2.toFloat());
      notifyClients(getSliderValues(2));
    }
    if (message.indexOf("3s") >= 0)
    {
      MC3 = sliderValue3.toFloat();
      sliderValue3 = message.substring(2);
      mot.inputProc(2, sliderValue3.toFloat());
      notifyClients(getSliderValues(3));
    }
    if (message.indexOf("4s") >= 0)
    {
      MC4 = sliderValue4.toFloat();
      sliderValue4 = message.substring(2);
      mot.inputProc(3, sliderValue4.toFloat());
      notifyClients(getSliderValues(4));
    }
    if (message.indexOf("5s") >= 0)
    {
      MC5 = sliderValue5.toFloat();
      sliderValue5 = message.substring(2);
      mot.inputProc(4, sliderValue5.toFloat());
      notifyClients(getSliderValues(5));
    }
    if (message.indexOf("6s") >= 0)
    {
      MC6 = sliderValue6.toFloat();
      sliderValue6 = message.substring(2);
      mot.inputProc(5, sliderValue6.toFloat());
      notifyClients(getSliderValues(6));
    }
    if (message.indexOf("7s") >= 0)
    {
      MC7 = sliderValue7.toFloat();
      sliderValue7 = message.substring(2);
      mot.inputProc(6, sliderValue7.toFloat());
      notifyClients(getSliderValues(7));
    }
    if (message.indexOf("8s") >= 0)
    {
      MC8 = sliderValue8.toFloat();
      sliderValue8 = message.substring(2);
      mot.inputProc(7, sliderValue8.toFloat());
      notifyClients(getSliderValues(8));
    }
    if (message.indexOf("9s") >= 0)
    {
      sliderValue1 = "0";
      MC1 = 0;
      sliderValue2 = "0";
      MC2 = 0;
      sliderValue3 = "0";
      MC3 = 0;
      sliderValue4 = "0";
      MC4 = 0;
      sliderValue5 = "0";
      MC5 = 0;
      sliderValue6 = "0";
      MC6 = 0;
      sliderValue7 = "0";
      MC7 = 0;
      sliderValue8 = "0";
      MC8 = 0;
      MotorStopAll();
      notifyClients(getSliderValues());
    }
    if (strcmp((char *)data, "getValues") == 0)
    {
      notifyClients(getSliderValues());
    }
  }
}

void onEvent2(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
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

void initWebSocket()
{
  ws.onEvent(onEvent2);
  server.addHandler(&ws);
}

void addWifi(String ssid, String password)
{
  preferences.putString(ssid.c_str(), password);
}

String getPassswordWifi(String ssid, String WNPass = WifiNotPassword)
{
  return preferences.getString(ssid.c_str(), WNPass);
}

bool isSaweWifi(String ssid)
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
      if(isSaweWifi(WiFi.SSID(i)))
      {
        w.addPolozkaData(i,i,String(WiFi.SSID(i)),2000);
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
  Serial.println("1");
  segDisp.begin(&Serial, B00000001, B00000100, B00010000, B01000000, B00100000, B00000010, B00001000, B10000000);
  Serial.println("2");
  // zahajeni ...
  
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &HlavniPreruseni, true);
  timerAlarmWrite(My_timer, 5000, true);
  timerAlarmEnable(My_timer); // Just Enable
  
  Serial.println("3");
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
 Serial.println("4");
  if (mot.beginEnd() == 0)
  {
    //segDisp.addText4("Err1");
  }

  MotorStopAll(); Serial.println("5");

  enk.begin(pin_EnkA, pin_EnkB, pin_EnkDP, pin_Tlac);
  Serial.print('v');
  //attachInterrupt(pin_EnkA,readEncoderISR,CHANGE);
  Serial.print('u');
  //segDisp.addText4("1111222233334444555566667777888899990000aaaabbbbccccddddeeeeffffgggghhhhiiiijjjjkkkkllllmmmmnnnnooooppppqqqqrrrrssssttttuuuuvvvvwwwwxxxxyyyyzzzz",250000);
  //segDisp.addText4("({[ )}] :-_~!#%&*+,./;<=>?@^|$",1000000);
  //segDisp.addText4("|$. ",30000);
  //segDisp.addText4Char(34,39,92,96,30000);
  //segDisp.addText4Char(127,127,127,127,30000);
  //segDisp.addText4Char('a','b','a','b',5000,B0110);
  //segDisp.addText4Char('a','b','a','b',5000,B0000,B0110);
  //segDisp.addText4("abababab",500,B10010110);
  //segDisp.addText4("abababab",7000,0,B10010110);
  //delay(12000);
  //segDisp.del();
  //segDisp.addText4("dell",30000);

  komunFace komF;
  komF.displej = &segDisp;
  komF.enk = &enk;
  komF.ser = &Serial;

  editText.begin(komF);
  
  menu Vr1;
  Vr1.begin(komF);
  Serial.println("6");
  /*
  menu prv ;
  prv.begin(komF);
  Serial.println("7");
  prv.addPolozkaData(1,2,"A",1000);
  prv.addPolozkaData(-15,-1,"B",1000);
  prv.addPolozkaData(-5,-100,"C",1000);
  Serial.println("8");
  Serial.println(prv.work());
  Serial.println("10");
  */
  //Serial.println(editText.WriteTextVyber("Ahojky"));

   
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
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

  initFS();
  WiFi.mode(WIFI_STA);
/*
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }
*/

  MenuWifiSet(komF);
  password.remove(password.length()-1);//Upravuje heslo, odstaranuje char 13 z konce
  WiFi.begin(ssid.c_str(), password.c_str());
  // Wait for connection
  Serial.println("7");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("8");
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  initWebSocket();
  segDisp.addText4IP(WiFi.localIP(),10000);

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
  String ip100 = WiFi.localIP().toString();
}

byte oooo = 0;

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
  

  if(0 !=mot.vystup())
  {
    Serial.println("oooo");
  }

    if(mot.vyst !=mot.vystup())
  {
    Serial.println("nnnn");
  }
  // Až dokončím komunikační protokol, bude fungovat zastavovaní brždění
  // dataSend p;
  // p.leng = 0;

 // Serial.print(digitalRead(pin_EnkA));Serial.println(digitalRead(pin_EnkB));
//Serial.print(digitalRead(pin_EnkDP));Serial.println(digitalRead(pin_Tlac));
// Serial.println(mot.vystup(),2);
if (digitalRead(pin_EnkDP) == 0)
{
  Serial.println(mot.vystup(),2);
  Serial.print("0p:");Serial.println(digitalRead(pin_m0));
  Serial.print("1p:");Serial.println(digitalRead(pin_m1));
  Serial.print("2p:");Serial.println(digitalRead(pin_m2));
  Serial.print("3p:");Serial.println(digitalRead(pin_m3));
  Serial.print("4p:");Serial.println(digitalRead(pin_m4));
  Serial.print("5p:");Serial.println(digitalRead(pin_m5));
  Serial.print("6p:");Serial.println(digitalRead(pin_m6));
  Serial.print("7p:");Serial.println(digitalRead(pin_m7));
  
  
}

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
