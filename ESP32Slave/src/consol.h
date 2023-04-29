#include <consolHeslo.h>
#ifndef consolHeslo
#define consolHeslo "ja"
#endif


#ifndef consolLib
#define consolLib
#include <Arduino.h>
#include <motor.h>
#include <ESPAsyncWebServer.h>

#define consolChar  '#'

class consol
{
private:
    /* data */
public:
    String work(String input, AsyncWebSocketClient *client);
    bool isPrikaz(String input);
    
    consol(/* args */);
    ~consol();
};

consol::consol(/* args */)
{
}

consol::~consol()
{
}

bool consol::isPrikaz(String input)
{
    if(input[0] == consolChar)
    {
        return true;
    }
    return false;
}



String consol::work(String input, AsyncWebSocketClient *client)
{
    return input+"text client" + String(client->id());
}

#endif