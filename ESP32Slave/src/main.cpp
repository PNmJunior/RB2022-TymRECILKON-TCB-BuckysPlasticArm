#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <wifi.h>

const char* host = "BuckysPlasticArm";
const char* ssid = wifiName;
const char* password = wifiHeslo;

WebServer server(80);

/*
 * Login page
 */

const char* loginIndex =
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32 Login Page</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
             "<td>Username:</td>"
             "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
"</script>";

/*
 * Server Index Page
 */

const char* serverIndex =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

/*
 * setup function
 */

//Motor definice
#define Mpocet 8
int Mpiny[Mpocet] = {32,33,25,26,27,13,23,19};
byte Mdata = 0;
#define MStopLow 0
#define MStopHight 10
#define MStop MStopLow
#define Mvpred 1
#define Mvzad -1
#define MEindex -1
#define MEmod -2
int Mfreq = 5000; 
int Mresolution = 8; 
int MlatchPin = 18;
int MdataPin = 17;
int MclockPin = 16;


void PosunReg(byte dataNov )
{
  if (dataNov != Mdata)
  {
    Mdata = dataNov;
    digitalWrite(MlatchPin, LOW);
    shiftOut(MdataPin, MclockPin, MSBFIRST, Mdata);
    digitalWrite(MlatchPin, HIGH);  
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
  touchRead(2);
  touchRead(3);

//inicializace PWM
  // configure LED PWM functionalitites
  for (size_t i = 0; i < Mpocet; i++)
  {
      ledcSetup(i, Mfreq, Mresolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(Mpiny[i], i);
  }
  
  //Inicializace OTA
  Serial.begin(115200);

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

  MotorRun(0,Mvpred,(int)255);
}


void loop(void) {
  server.handleClient();
  delay(1);
}
