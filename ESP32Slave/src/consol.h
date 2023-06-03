#include <consolHeslo.h>
#include <komunProtokol.h>
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
    AsyncWebSocket *socket = NULL;
    /* data */
public:
    String work(String input, AsyncWebSocketClient *client);
    bool isPrikaz(String input);
    void print(String input,AsyncWebSocketClient *client = NULL);
    void println(String input,AsyncWebSocketClient *client = NULL);
    void setSocket(AsyncWebSocket *_socket);
    void print(int input, AsyncWebSocketClient *client = NULL);

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

inline void consol::print(String input, AsyncWebSocketClient *client )
{
    Serial.print(input);
    if (socket != NULL)
    {
        if (client == NULL)
        {
            //socket->textAll(SendSystem.chat( input));
        }
        else
        {
            client->text(SendSystem.chat( input));
        }
    }
}

inline void consol::print(int input, AsyncWebSocketClient *client )
{
    print(String(input),client);
}

inline void consol::println(String input, AsyncWebSocketClient *client )
{
    print(input + "\r\n",client);
}

inline void consol::setSocket(AsyncWebSocket *_socket)
{
    socket = _socket;
}

String consol::work(String input, AsyncWebSocketClient *client)
{
    if(isPrikaz(input) == false)
    {
        return "Chyba detekce prikazu";
    }
    
    //sscanf(input.c_str(),"#%s %s %s %s %s %s %s %s %s %s");

    return input+"text client" + String(client->id());
}

#endif