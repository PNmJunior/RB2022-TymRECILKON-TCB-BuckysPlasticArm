#include <Arduino.h>
#include <consol.h>

#ifndef managerCl
#define managerCl

struct managerClientInfo
{
    long time;
    long timeInput;
    int problem;
    long timeProblem;
    long timeProblemMax;
    long ban_enable_time;//bad = -1, enable  = 0, time = millis end
};


class managerClient
{
private:
    managerClientInfo info[50];
public:
    bool timeTrue(int index,long timeInput);
    bool timeOut(int index);
    void ban();
    void enable();
    void banTime(long t);
    void begin(consol *con);
    consol *cons;
    managerClient(/* args */);
    ~managerClient();
};

inline bool managerClient::timeTrue(int index, long timeInput)
{
    if (index > 50 || index < 0)
    {
        return false;
    }
    if (info[index].time == 0)//start
    {
        info[index].time = millis();
        info[index].timeInput = timeInput;
        cons->print("novyKl");
    }


    long tR = (long)millis() - info[index].time;
    long tV = timeInput - info[index].timeInput;
    int rozdil = abs(tR - tV*100);
    cons->print(rozdil);
    if(rozdil < 400)
    {
        info[index].time = millis();
        info[index].timeInput = timeInput;
        return true;
    }
    else 
    {
        info[index].timeProblem += rozdil;
        info[index].problem ++;
    }
    return false;
}

inline bool managerClient::timeOut(int index)
{
    if (index > 50 || index < 0)
    {
        return true;
    }
    Serial.print(millis());Serial.print('-');Serial.println(info[index].time);
Serial.println(millis() - info[index].time);
    
    if (abs( (long)millis() - info[index].time) < 400)
    {
        Serial.println("CCCC");
        return false;
    }
    Serial.println("DDDD");
    
    return true;
}

inline void managerClient::ban()
{

}

inline void managerClient::begin(consol *con)
{
    cons = con;
    for (int i = 0; i < 50; i++)
    {
        info[i].problem = 0;
        info[i].time = 0;
        info[i].timeInput = 0;
        info[i].timeProblem = 0;
        info[i].timeProblemMax = 0;
    }
}

managerClient::managerClient(/* args */)
{

    
}

managerClient::~managerClient()
{
}




#endif