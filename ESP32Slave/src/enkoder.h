#ifndef enkod
#define enkod

#include <Arduino.h>
#include <functional>
/*
//https://github.com/PaulStoffregen/Encoder.git

//                           _______         _______       
//               Pin1 ______|       |_______|       |______ Pin1
// negative <---         _______         _______         __      --> positive
//               Pin2 __|       |_______|       |_______|   Pin2

//	new	    new	    old	    old
//	pin2	pin1	pin2	pin1	Result
//	----	----	----	----	------
//	0	    0	    0	    0	    no movement
//	0	    0	    0	    1	    +1
//	0	    0	    1	    0	    -1
//	0	    0	    1	    1	    +2  (assume pin1 edges only)
//	0	    1	    0	    0	    -1
//	0	    1	    0	    1	    no movement
//	0	    1	    1	    0	    -2  (assume pin1 edges only)
//	0	    1	    1	    1       +1
//	1	    0	    0	    0	    +1
//	1	    0	    0	    1	    -2  (assume pin1 edges only)
//	1	    0	    1	    0	    no movement
//	1	    0	    1	    1	    -1
//	1	    1	    0	    0	    +2  (assume pin1 edges only)
//	1	    1	    0	    1	    -1
//	1	    1	    1	    0	    +1
//	1	    1	    1	    1	    no movement
*/

class enkoder
{
private:


     byte pinEnA;
     byte pinEnB;
     byte pinEnButt;
     byte pinTlac;
public:
     int smerEnk; 
     byte stavEnk;
     bool butt;
     bool tlac;
     int Enk();
     bool Butt();
     bool Tlac();
     void begin(byte pinEnA, byte pinEnB, byte pinEnButt, byte pinTlac);
};


 enkoder enk;


void EnkoderPootoceni(void * arg)
{
    enkoder *en = (enkoder*)arg;
    en->stavEnk = en->stavEnk >> 2;
    if (digitalRead(1))
    {
        en->stavEnk = en->stavEnk | B1000;
    }
    if (digitalRead(2))
    {
        en->stavEnk = en->stavEnk | B0100;
    }
    switch (en->stavEnk)
    {
    
    case B0001:
    case B0111:
    case B1000:
    case B1110:
    en->smerEnk = 1;
    break;
    case B0010:
    case B0100:
    case B1011:
    case B1101:
    en->smerEnk = -1;
    break;
    case B0011:
    case B1100:
    en->smerEnk = 1;
    break;
    case B0110:
    case B1001:
    en->smerEnk = -1;
    break;    
    default:
    en->smerEnk = 0;
        break;
    }    
}


void  EnkoderButton(void * arg)
{
        enkoder *en = (enkoder*)arg;
    en->butt = true;
}


void Tlacitko(void * arg)
{
        enkoder *en = (enkoder*)arg;
    en->tlac = true;
}


void enkoder::begin(byte _pinEnA, byte _pinEnB, byte _pinEnButt, byte _pinTlac)
{
    pinEnA = _pinEnA;
    pinEnB = _pinEnB;
    pinEnButt = _pinEnButt;
    pinTlac = _pinTlac;
    pinMode(pinEnA,INPUT);
    pinMode(pinEnB,INPUT);
    pinMode(pinEnButt,INPUT);
    pinMode(pinTlac,INPUT);
    touchAttachInterruptArg(digitalPinToInterrupt(pinEnA), EnkoderPootoceni,this, CHANGE);
    touchAttachInterruptArg(digitalPinToInterrupt(pinEnButt),EnkoderButton,this, FALLING);
    touchAttachInterruptArg(digitalPinToInterrupt(pinTlac),Tlacitko,this, FALLING);

}


int enkoder::Enk()
{
    int l  =smerEnk;
    smerEnk = 0;
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