#include <consolHeslo.h>
#ifndef consolHeslo
#define consolHeslo "ja"
#endif


#ifndef consolLib
#define consolLib
#include <Arduino.h>
#include <motor.h>
#include <ESPAsyncWebServer.h>

class consol
{
private:
    /* data */
public:
    String work(String input, AsyncWebSocketClient *client);
    consol(/* args */);
    ~consol();
};

consol::consol(/* args */)
{
}

consol::~consol()
{
}

String consol::work(String input, AsyncWebSocketClient *client)
{
    return input+"text client" + String(client->id());
}

#endif