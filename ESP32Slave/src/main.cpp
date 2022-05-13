#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <tajnosti.h>
#include <webHTMLtext.h>

const char* host = "BuckysPlasticArm";
const char* ssid = wifiName;
const char* password = wifiHeslo;

WebServer server(80);

/*
 * setup function
 */

//Motor definice
#define Mpocet 8
int Mpiny[Mpocet] = {32,33,23,19,27,13,26,25};//bila-modra,cerv-fial,zel-bila,zlut-seda,fial-bila,mod-fial,fial-bila,seda-cerv,
byte Mdata = 0;
//32-bila-modra,33-cerv-fial,23-zel-bila,19-zlut-seda,27-fial-bila,13-mod-fial,26-fial-bila,25-seda-cerv,
#define MStopLow 0
#define MStopHight 10
#define MStop MStopLow
//26-seda-cerv,25-bila-zel
#define Mvpred 1
#define Mvzad -1
#define MEindex -1
#define MEmod -2
int Mfreq = 1000; 
int Mresolution = 8; 
int MlatchPin = 18;
int MdataPin = 17;
int MclockPin = 16;

//test2=test motorů
long timeTest1=0;
byte mmm=0;

void PosunReg()
{
    digitalWrite(MlatchPin, LOW);
    shiftOut(MdataPin, MclockPin, MSBFIRST, Mdata);
    digitalWrite(MlatchPin, HIGH);  
}

void PosunReg(byte dataNov )
{
  if (dataNov != Mdata)
  {
    Mdata = dataNov;
    PosunReg();
  }
}


void PosRegIndex(int index, bool A_N)
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

void MotorStopAll()
{
  Mdata = 0;
  PosunReg();
  for (int i = 0; i < Mpocet; i++)
  {
    ledcWrite(i, 0);
  }
  
}




void MotorRun(int index, int mod, int rychlost)
{
  if (rychlost < 0)
  {
    rychlost = 0;
  }
  else if (rychlost > 255)
  {
    rychlost = 255;
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
  Serial.print(index);Serial.print(",");Serial.print(Mpiny[index]);Serial.print(",");Serial.print(mod);Serial.print(","),Serial.println(rychlost);
}


void setup() {
  // put your setup code here, to run once:
  //H mustek
  pinMode(32,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(19,OUTPUT);

//posuvny registr
  pinMode(18,OUTPUT);
  pinMode(17,OUTPUT);
  pinMode(16,OUTPUT);

  //dotikovy senzor
  //touchRead(2);
  //touchRead(3);

//inicializace PWM
  // configure LED PWM functionalitites
  for (size_t i = 0; i < Mpocet; i++)
  {
      ledcSetup(i, Mfreq, Mresolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(Mpiny[i], i);
  //Serial.print(i);Serial.print(",");Serial.println(Mpiny[i]);
  
  }

  MotorStopAll();
  Serial.begin(9600);
  //Inicializace OTA
  

  // Connect to WiFi network
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

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();

  //MotorRun(0,Mvpred,(int)255);
}


void loop(void) {
  server.handleClient();
  delay(1);
  timeTest1 ++;
  //byte mmm=0;
  /*
  if (timeTest1 == 250)
  {
    MotorRun(mmm,Mvpred,(int)255);
  }
    if (timeTest1 == 500)
  {
    MotorRun(mmm,MStopLow,(int)255);
  }
    if (timeTest1 == 7500)
  {
    MotorRun(mmm,Mvzad,(int)255);
  }
    if (timeTest1 == 1000)
  {
    MotorRun(mmm,MStopHight,(int)255);
  
    if (mmm == 7)
    {
      mmm=0;
    }
    else
    {
      mmm++;
    }
    
  }*/


/*

    if (timeTest1 == 1000)
  {
    MotorRun(mmm,Mvpred,(int)255);
  }
    if (timeTest1 == 2000)
  {
    MotorRun(mmm,MStopLow,(int)255);
  }
    if (timeTest1 == 3000)
  {
    MotorRun(mmm,Mvzad,(int)255);
  }
    if (timeTest1 == 4000)
  {
    MotorRun(mmm,MStopHight,(int)255);
  }

  if (timeTest1 == 5000)
  {
    MotorRun(mmm,Mvpred,(int)100);
  }
    if (timeTest1 == 6000)
  {
    MotorRun(mmm,MStopLow,(int)100);
  }
    if (timeTest1 == 7000)
  {
    MotorRun(mmm,Mvzad,(int)100);
  }
    if (timeTest1 == 8000)
  {
    MotorRun(mmm,MStopHight,(int)100);
    timeTest1 = 0;
    if (mmm == 7)
    {
      mmm=0;
    }
    else
    {
      mmm++;
    }
    
  }*/
}
