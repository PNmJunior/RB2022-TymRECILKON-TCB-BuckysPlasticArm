#include <disp.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "AsyncWebSynchronization.h"
#include <Arduino_JSON.h>
#include <komunBasic.h>


typedef std::function<void(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)> AwsEventHandler;

class komun
{
private:
    AsyncWebSocket *ws;
    AsyncWebServer *server;
    //void onEvent2(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
    //void onEvent2(AwsEventHandler);
    dataSend (*work)(dataFace);
    byte *fron;    
    /* data */
public:
    void begin(AsyncWebSocket *WS,AsyncWebServer *Server, dataSend (*_Work)(dataFace));
    void go(void *arg, uint8_t *data, size_t len);
    //void send(dataFace in, bool fronta = true);
    //dataSend ADDdataRow(dataSend *a, dataSend *b);
    //dataSend dataRow(dataFace in);
};


void komun::go(void *arg, uint8_t *data, size_t len)
{
  dataSend zaloha;
  zaloha.leng = 0;
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  void *work2 = work;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    zaloha= makeOutput(data,len,work);

  }

  if (zaloha.leng != 0)
  {
    ws->binaryAll(zaloha.data,zaloha.leng);
    free(zaloha.data);
  }  
}


void komun::begin(AsyncWebSocket *WS,AsyncWebServer *Server, dataSend (*_Work)(dataFace))
{
  ws = WS;
  server  = Server;
  work = _Work;
}
