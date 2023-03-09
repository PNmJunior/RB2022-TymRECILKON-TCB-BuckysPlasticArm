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

const byte NastMotPin[8]= {32,33,25,23,19,13,12,26};//system

const ledc_channel_t NastMotLEDC_CHANNEL[8]= {
  LEDC_CHANNEL_0,//pin 32
  LEDC_CHANNEL_1,//pin 33
  LEDC_CHANNEL_2,//pin 25
  LEDC_CHANNEL_3,//pin 23
  LEDC_CHANNEL_4,//pin 19
  LEDC_CHANNEL_5,//pin 13
  LEDC_CHANNEL_6,//pin 12
  LEDC_CHANNEL_7 //pin 26
};//system

const bool NastMotInverz[8] = {
  false,//pin 32
  false,//pin 33
  false,//pin 25
  false,//pin 23
  false,//pin 19
  false,//pin 13
  false,//pin 12
  false //pin 26
};//realne zapojení na konektoru, razeno postupne

const bool NastMotNeg[8] = {
  false,//M_LED
  false,//M_1
  false,//M_2
  false,//M_3
  false,//M_4
  false,//M_Kleste
  false,//M_Levy
  false //M_Pravy
};//nutnost zmenit smer, razeno podle prevodniku

const byte NastMotMaxMin[8][2] = {
  {255,0},//M_LED
  {255,100},//M_1
  {255,100},//M_2
  {255,50},//M_3
  {255,50},//M_4
  {170,130},//M_Kleste
  {255,50},//M_Levy
  {245,50} //M_Pravy
};//razeno podle prevodniku


disp segDisp;
motor mot;
// komun kom;
hw_timer_t *My_timer = NULL;
//enkoder enk;

byte posunIP = 0;

byte pocetClientu = 0 ;

void IRAM_ATTR HlavniPreruseni()
{
  if(!mot.work)
  {
    mot.work = true;
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
  mot.work = false;
  }
}

String ssid ;
String password ;

// WebServer server(80);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
String message = "";


void MotorStopAll() // Vždy zastaví chod robota.
{
  String aaa;
  for (int i = 0; i < 8; i++)
  {
    mot.input(i);
    aaa += SendSystem.motor(i,0);
  }
  ws.textAll(aaa);
}


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

double rozdilPravy(int uhel)
{
  int uhelRozdil = uhel % 360;
    if(uhelRozdil < 0)
    {
        uhelRozdil = uhelRozdil + 360;
    }
    if(uhelRozdil == 0 )
    {
        return -1.0;
    }
    else if(uhelRozdil > 0 && uhelRozdil < 90)
    {
        return ((double)uhelRozdil-45.0)/45.0;
    }
    else if(uhelRozdil >= 90 && uhelRozdil <= 180)
    {
        return 1.0;
    }
    else if(uhelRozdil > 180 && uhelRozdil < 225)
    {
        return (202.5 - (double)uhelRozdil)/22.5;
    }
    else if(uhelRozdil >= 255 && uhelRozdil <= 270)
    {
        return -1.0;
    }
    else if(uhelRozdil > 270 && uhelRozdil <= 315)
    {
        return ((double)uhelRozdil-315.0)/45.0;
    }
    else if(uhelRozdil > 315 && uhelRozdil <= 360)
    {
        return (315.0 - (double)uhelRozdil)/45.0;
    }
    return 0;
}


double rozdilLevy(int uhel)
{
    int uhelRozdil = uhel % 360;
    if(uhelRozdil < 0)
    {
        uhelRozdil = uhelRozdil + 360;
    }
    if((uhelRozdil >= 0 && uhelRozdil <= 90)|| uhelRozdil == 360)
    {
        return 1.0;
    }
    else if(uhelRozdil > 90 && uhelRozdil <= 180)
    {
        return (135.0 - (double)uhelRozdil)/45.0;
    }
    else if(uhelRozdil > 180 && uhelRozdil <= 225)
    {
        return ((double)uhelRozdil-225.0)/45.0;
    }
    else if(uhelRozdil > 225 && uhelRozdil < 270)
    {
        return (225.0 - (double)uhelRozdil)/45.0;
    }
    else if(uhelRozdil >= 270 && uhelRozdil <= 315)
    {
        return -1.0;
    }
    else if(uhelRozdil > 315 && uhelRozdil <= 360)
    {
        return ((double)uhelRozdil-337.5)/22.5;
    }
    return 0;
}


int joysticZaok(int i, int mez = 10)
{
  if (abs(i) < mez)
  {
    return 0;
  }
  else if (i > 100)
  {
    return 100;
  }
  else if (i < (-100))
  {
    return i;
  }
  return i;
}

String joysticWork(int x,int mot_x, int y, int mot_y, int rozdil = 1)
{
  String i;
  int mx = Prevodnik[mot_x];
  int my = Prevodnik[mot_y];
  if (abs(x - mot.outProc(mx)) >= rozdil)
  {
    mot.inputProc(mx,x);
    i += SendSystem.motor(mx,mot.outProc(mx) );
  }
  if (abs(y - mot.outProc(my)) >= rozdil)
  {
    mot.inputProc(my,y);
    i += SendSystem.motor(my,mot.outProc(my) );
  }
  return i;
}


void ZpracovaniDat(String mess, AsyncWebSocketClient *client = NULL)
{
    komunProtokol komP;
    komP.begin(mess);
    //Serial.println("mess:");Serial.println(mess);
    String outAll;
    String outClient;
    for (int s = 0; s < komP.pocetSouboru; s++)
    {
      String souName = komP.readStr();
      int cisloM;
      int smer;
      switch (souName.charAt(0))
      {
      case 'j':
      {
        if (komP.pocetVAktualSouboru() != 5)
        {
          Serial.println("Problem s velikosti j:");
          Serial.println(komP.pocetVAktualSouboru());
          return;
        }
        outAll += komP.sendAktSoubor();
        Serial.println(komP.sendAktSoubor());
        
        int joy = komP.readInt();
        Serial.println(joy);
        int osa_x = komP.readInt();
        int osa_y = komP.readInt();
        int tl = komP.readInt();
        int osa_x_zaok = joysticZaok(osa_x);
        int osa_y_zaok = joysticZaok(osa_y);
        switch (joy)
        {
        case 1:
        {
          int osa_x_zaok2 = joysticZaok(osa_x);
          int osa_y_zaok2 = joysticZaok(osa_y);
          double d = sqrt((double)(osa_x_zaok2 * osa_x_zaok2 + osa_y_zaok2 * osa_y_zaok2));
          if (d > 100.0)
          {
            d = 100.0;
          }
          else if (d < 20)
          {
            d = 0.0;
          }
          double M_Levy_lokal = 0;
          double M_Pravy_lokal = 0;
          if (d != 0.0)
          {
            double rad = atan2(osa_y_zaok2,osa_x_zaok2);
            double uhel = (-180/PI)* rad;
            M_Levy_lokal = d * rozdilLevy(uhel);
            M_Pravy_lokal = d * rozdilPravy(uhel);
          }
          outAll += joysticWork(M_Levy_lokal,M_Levy,M_Pravy_lokal,M_Pravy);
          break;
        }
        case 2:
          outAll += joysticWork(osa_x_zaok,M_1,-osa_y_zaok,M_2);
          break;
        case 3:
          outAll += joysticWork(osa_x_zaok,M_LED,-osa_y_zaok,M_3);
          break;
        case 4:
          outAll += joysticWork(osa_x_zaok,M_Kleste,-osa_y_zaok,M_4);
          break;        
        default:
          break;
        }
        break;
      }
      case 'm':

        if (komP.pocetVAktualSouboru() != 3)
        {
          Serial.println("Problem s velikosti M:");
          Serial.println(komP.pocetVAktualSouboru());
          return;
        }
        cisloM = komP.readInt();
        smer = komP.readInt();
        /*Serial.print("c:");Serial.println(cisloM);Serial.print("h:");Serial.println(smer);*/
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
    //Serial.println("outAll:");Serial.println(outAll);
    if (outAll != "")
    {
      ws.textAll(outAll);
    }
    //Serial.print("outClient:");Serial.println(client->id());Serial.println(outClient);
    if (outClient != "" && client != NULL)
    {
      client->text(outClient);
    }
}


void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    message = (char *)data;
    ZpracovaniDat(message, client);
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

  for (byte i = 0; i < 8; i++)
  {
    mot.begin(Prevodnik[i],  NastMotPin[Prevodnik[i]], NastMotLEDC_CHANNEL[Prevodnik[i]], NastMotInverz[Prevodnik[i]], NastMotNeg[i], NastMotMaxMin[i][0], NastMotMaxMin[i][1]);
  }

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
  String p;
  for (int i = 0; i < 8; i++)
  {
    if (mot.TStop(i) != 0)
    {
      if (mot.TStop(i) < millis())
      {
        mot.input(i, mStop);
        p += SendSystem.motor(i,0);
      }
    }
  }
  if (p.length() != 0)
  {
    ws.textAll(p);
  }

  String l ;

  for (int i = 0; i < 8; i++)
  {
    //Oprava chyby, kdy knihovna říka, že motor jede při nulové rychlosti a motor se pohybuje.
    //Vznik teto nulové chyby nepodařilo vyřešit ani zaokrouhlovacím systémem
    if ((mot.outSmer(i) == mVpred ||mot.outSmer(i) == mVzad) && mot.outSpead(i) == 0)
    {
        mot.input(i, mStop);
        l += SendSystem.motor(i,0);
    }
  }
  if (l.length() != 0)
  {
    ws.textAll(l);
  }
  ws.cleanupClients();
}
