#include<disp.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "AsyncWebSynchronization.h"
#include <Arduino_JSON.h>

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <tajnosti.h>//Jsou zde definované jmého a heslo wifi
#include <disp.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

#define Amask                       B11000000
#define Amotor                      B00000000

#define Atext                       B01000000
#define AsetMotor                   B10000000
#define AsetView                    B11000000
#define Aposun  6
#define Bmask B00100000
#define BsMax_smerVpred_tMess        B00000000
#define BsMin_smerVzad_tSet          B00100000
#define Bposun  5
#define Cmask  B00010000
#define Cano_brzda_mChat_sRead       B00010000
#define Cne_brzda_mInfo_sWrite         B00000000
#define Cposun  4
#define INDEXmask B00001111

#define textNum 1
#define motorNum 0
#define text_setNum 1
#define text_messNum 0
#define text_set_ReadNum 1
#define text_set_WriteNum 0
#define text_mess_infoNum 0
#define text_mess_chatNum 1
#define motorNum 0
#define motor_Vpred 0
#define motor_Vzad 1
#define motor_i_Brzda 1
#define whatMotNum 2






/*
text>0
-mess
--info,chat
-set
--read,write
motor>00001111
-smer
--brzda
*/

struct dataFace
{
  byte AtypeNum;
  bool BtypeBool;
    bool CtypeBool;
    byte Index;
    byte Data; 
    String text;
    int len;
};


struct output
{
    byte row[2];
    byte Atype;
    byte Btype;
    byte Ctype;
dataFace data;
    
};

struct dataSend
{
  uint8_t *data;
  size_t leng;
};



typedef std::function<void(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)> AwsEventHandler;

class komun
{
private:
    AsyncWebSocket *ws;
    AsyncWebServer *server;
    //void onEvent2(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
    //void onEvent2(AwsEventHandler);
    dataSend (*work)(output*);
    byte *fron;
    
    
    /* data */
public:
    void begin(AsyncWebSocket *WS,AsyncWebServer *Server, void (*_Work)(output));
    void go(void *arg, uint8_t *data, size_t len);
    void send(dataFace in, bool fronta = true);
    dataSend ADDdataRow(dataSend *a, dataSend *b);
    dataSend dataRow(dataFace in);
    komun(/* args */);
    ~komun();
};

/*
AwsEventHandler komun::onEvent2(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) 
//void onEvent(AwsEventHandler)
{
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      go(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}*/

dataSend komun::dataRow(dataFace in)
{
  dataSend a;
  a.data = (byte*)malloc(in.len);
  a.data[0] = in.AtypeNum<<Aposun | in.BtypeBool <<Bposun |in.CtypeBool<<Cposun | in.Index;
  a.data[1] = in.Data;
  if (a.data[0] & Amask == Atext)
  {
    for (int i = 0; i < in.text.length(); i++)
    {
      a.data[i+2] = in.text.charAt(i);
    }
    
  }
  return a;

}

dataSend komun::ADDdataRow(dataSend *a, dataSend *b)
{
  dataSend c;
  c.leng = a->leng + b->leng;
  c.data = (byte *)malloc(c.leng);
  for (int i = 0; i < a->leng; i++)
  {
    c.data[i] = a->data[i];
  }
  for (int i = 0; i < b->leng; i++)
  {
    c.data[i+a->leng] = b->data[i];
  }
  free(a->data);
  free(b->data);
  return c;
  
}


void komun::go(void *arg, uint8_t *data, size_t len)
{
  dataSend zaloha;
  zaloha.leng = 0;
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    
    int i = 0;
    do
    {
      output a;
      a.row[0] = data[i];
      a.row[1] = data[i+1];
      a.Atype = a.row[0] & Amask;
      a.data.AtypeNum = a.Atype >> Aposun;
      a.Btype = a.row[0] & Bmask;
      byte p = a.Btype >> Bposun;
      a.data.BtypeBool = p;       
      a.Ctype = a.row[0] & Cmask;
      p = a.Ctype >> Cposun;
      a.data.CtypeBool = p;
      a.data.Index = a.row[0] & INDEXmask;
      a.data.Data = a.row[1];
      i += 2;
      a.data.len = 2;
      if (a.Atype == Atext)
      {
        if ((len-i)<=a.data.Data)
        {
          for (int u = 0; u < a.data.Data; u++)
          {
            a.data.text += data[i];
            i++;
            a.data.len ++;
          }
          
        }
        else 
        {
          //problem
        }
        
      }

      if (a.data.AtypeNum == textNum && a.data.BtypeBool == text_setNum)
      {
        if (a.data.CtypeBool == text_set_ReadNum)
        {
          /* code */
        }
        else
        {

        }
        
      }
      else
      {
        zaloha = ADDdataRow(&zaloha,&work(&a));
      }
      
      
      
    } while (i<len);

    if (zaloha.leng != 0)
    {
      ws->binaryAll(zaloha.data,zaloha.leng);
      free(zaloha.data);
    }
  }
}

void komun::begin(AsyncWebSocket *WS,AsyncWebServer *Server, void (*_Work)(output))
{
  ws = WS;
  server  = Server;
  work = _Work;
}


komun::komun(/* args */)
{
}

komun::~komun()
{
}

