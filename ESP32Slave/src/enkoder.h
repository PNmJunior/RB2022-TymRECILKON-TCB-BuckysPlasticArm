#ifndef enkod
#define enkod

#include <Arduino.h>

class enkoder
{
private:
    byte stavEnkOld;
    byte stavEnk;
    bool butt;
    bool tlac;
    void EnA();
    void EnB();
    void EnK();
    void TlacK();
    
public:
    int Enk();
    bool Butt();
    bool Tlac();
    void begin();
};

void  IRAM_ATTR enkoder::EnA()
{

}
void  IRAM_ATTR enkoder::EnB()
{

}
void  IRAM_ATTR enkoder::EnK()
{

}
void  IRAM_ATTR enkoder::TlacK()
{

}

void enkoder::begin()
{
    attachInterrupt(5, this->EnA, CHANGE);
    attachInterrupt(6, EnB, CHANGE);
    attachInterrupt(7, TlacK, Mode);
}


int enkoder::Enk()
{
    byte p = stavEnk<<2 | stavEnkOld;
    int l  =0;
    switch (p)
    {
    case B0001:
    case B0111:
    case B1110:
    case B1000:
        l =  1;
        break;
    case B0100:
    case B1101:
    case B1011:
    case B0010:
        l = -1;
    default:
        break;
    }
    stavEnk = stavEnkOld;
    return l;
}

bool enkoder::Butt()
{
    if (butt == true)
    {
        butt = false;
        return true;
    }
    return false;
}

bool enkoder::Tlac()
{
    if (tlac == true)
    {
        tlac = false;
        return true;
    }
    return false;
}


#endif
