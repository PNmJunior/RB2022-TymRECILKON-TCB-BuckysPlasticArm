#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <tajnosti.h>//Jsou zde definované jmého a heslo wifi
#include <disp.h>
#include <motor.h>
#include <komun.h>
#include <komunBasic.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

//nove definice
#define pin_PR_Clock 16
#define pin_PR_Data1 18
#define pin_PR_Data2 4
#define pin_PR_Latch 17
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
komun kom;


void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    uint8_t i;

    for(i = 0; i < 8; i++) {
        if(bitOrder == LSBFIRST)
            digitalWrite(dataPin, !!(val & (1 << i)));
        else
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));

        digitalWrite(clockPin, HIGH);
        digitalWrite(clockPin, LOW);
    }
}

void MotorStopAll()//Vždy zastaví chod robota. Automaticky posílá data posuvnému registru.
{
  for (int i = 0; i < 8; i++)
  {
    mot.input(i);
  }

}



void IRAM_ATTR HlavniPreruseni()
{
  byte d1 = mot.vystup();
  byte d2 = segDisp.vystup();
  byte d3 = segDisp.index();
  digitalWrite(pin_PR_Latch, LOW);
  for(uint8_t i = 0; i < 8; i++)
  {
    //digitalWrite(pin_PR_Data1, !!(val & (1 << i)));//LSBFIRST
    digitalWrite(pin_PR_Data1, !!(d1 & (1 << (7 - i))));//MSBFIRST
        //digitalWrite(pin_PR_Data2, !!(val & (1 << i)));//LSBFIRST
    digitalWrite(pin_PR_Data2, !!(d2 & (1 << (7 - i))));//MSBFIRST
    digitalWrite(pin_PR_Clock, HIGH);
    digitalWrite(pin_PR_Clock, LOW);
  }

  mot.updatePWM();

  switch (d3)
  {
  case 0:
    digitalWrite(pin_LedD4,1);
    digitalWrite(pin_LedD1,0);
    break;
  case 1:
    digitalWrite(pin_LedD1,1);
    digitalWrite(pin_LedD2,0);
    break;
  case 2:
    digitalWrite(pin_LedD2,1);
    digitalWrite(pin_LedD3,0);
    break;
  case 3:
    digitalWrite(pin_LedD3,1);
    digitalWrite(pin_LedD4,0);
    break;
  
  default:
    digitalWrite(pin_LedD1,1);
    digitalWrite(pin_LedD2,1);
    digitalWrite(pin_LedD3,1);
    digitalWrite(pin_LedD4,1);
    break;
  }

  digitalWrite(pin_PR_Latch, HIGH); 
  segDisp.vystupEX();


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
      if(MC1 <= 20.00 && MC1 >= -20.00)
      {
        MC1 = 0.0;
        sliderValue1 = "0";
      }
      notifyClients(getSliderValues(1));
      
    }
    if (message.indexOf("2s") >= 0) {
      sliderValue2 = message.substring(2);
      MC2 = sliderValue2.toFloat();
      if(MC2 == 5.00 || MC2 == -5.00)
      {
        MC2 = 0.0;
        sliderValue2 = "0";
      }
      notifyClients(getSliderValues(2));
      
    }    
    if (message.indexOf("3s") >= 0) {
      sliderValue3 = message.substring(2);
      MC3 = sliderValue3.toFloat();
      if(MC4 != 0.0)
      {
        MC4 = 0.0;
        sliderValue4 = "0";
        notifyClients(getSliderValues(4));
      }
      notifyClients(getSliderValues(3));
    }
        if (message.indexOf("4s") >= 0) {
      sliderValue4 = message.substring(2);
      MC4 = sliderValue4.toFloat();
      if(MC3 != 0.0)
      {
        MC3 = 0.0;
        sliderValue3 = "0";
        notifyClients(getSliderValues(3));
      }
      notifyClients(getSliderValues(4));

    }
        if (message.indexOf("5s") >= 0) {
      sliderValue5 = message.substring(2);
      MC5 = sliderValue5.toFloat();
      if(MC6 != 0.0)
      {
        MC6 = 0.0;
        sliderValue6 = "0";
        notifyClients(getSliderValues(6));
      }
      notifyClients(getSliderValues(5));
    }
        if (message.indexOf("6s") >= 0) {
      sliderValue6 = message.substring(2);
      MC6 = sliderValue6.toFloat();
      if(MC5 != 0.0)
      {
        MC5 = 0.0;
        sliderValue5 = "0";
        notifyClients(getSliderValues(5));
      }
      notifyClients(getSliderValues(6));
    }
        if (message.indexOf("7s") >= 0) {
      sliderValue7 = message.substring(2);
      MC7 = sliderValue7.toFloat();
      if(MC8 != 0.0)
      {
        MC8 = 0.0;
        sliderValue8 = "0";
        notifyClients(getSliderValues(8));
      }
      notifyClients(getSliderValues(7));
    }
        if (message.indexOf("8s") >= 0) {
      sliderValue8 = message.substring(2);
      MC8 = sliderValue8.toFloat();
      if(MC7 != 0.0)
      {
        MC7 = 0.0;
        sliderValue7 = "0";
        notifyClients(getSliderValues(7));
      }
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


void onEvent2(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
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
  ws.onEvent(onEvent2);
  server.addHandler(&ws);
}


void setup() {
  // put your setup code here, to run once:

  mot.beginStart();
    pinMode(pin_LedD1,OUTPUT);
  pinMode(pin_LedD2,OUTPUT);
  pinMode(pin_LedD3,OUTPUT);
  pinMode(pin_LedD4,OUTPUT);
    digitalWrite(pin_LedD1,1);
    digitalWrite(pin_LedD2,1);
    digitalWrite(pin_LedD3,1);
    digitalWrite(pin_LedD4,1);
  segDisp.begin(DP,D,E,C,G,B,F,A);//zmenit


//zahajeni ...

  mot.beginTimer();
  
  mot.begin(0,0,LEDC_CHANNEL_0,false,false,255,0);
  mot.begin(1,0,LEDC_CHANNEL_0,false,false,255,0);
  mot.begin(2,0,LEDC_CHANNEL_0,false,false,255,0);
  mot.begin(3,0,LEDC_CHANNEL_0,false,false,255,0);
  mot.begin(4,0,LEDC_CHANNEL_0,false,false,255,0);
  mot.begin(5,0,LEDC_CHANNEL_0,false,false,255,0);
  mot.begin(6,0,LEDC_CHANNEL_0,false,false,255,0);
  mot.begin(7,0,LEDC_CHANNEL_0,false,false,255,0);

  if (mot.beginEnd() == 0)
  {
    segDisp.addText4("Err1");
  }
  //Nas vstupu pro led segment

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
    MotorStopAll();
    server.reset();
    MotorStopAll();
    
  });

          server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request){
    MotorStopAll();
  });

   server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/joy.html", "text/html");
  });
  
  server.serveStatic("/", SPIFFS, "/");

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");


    String ip100 = WiFi.localIP().toString();
}

void loop(void) 
{
  dataSend p;
  p.leng = 0;

    for (int i = 0; i < 8; i++)
    {
        if (mot.TStop(i) != 0)
        {
            if (mot.TStop(i) < millis())
            {
                mot.input(i,mStop);
                p = ADDdataRow(p,motorSend(i,0,0));
            }
        }
    }
    kom.send(p);
  ws.cleanupClients();
}
