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

public:
    volatile byte pinEnA;
    volatile byte pinEnB;
    volatile byte pinEnButt;
    volatile byte pinTlac;
    volatile int smerEnk; 
    volatile byte stavEnk;
    volatile long casEnkoder;
    volatile long casButt;
    volatile long casTlac;
    volatile long casEnkoderOld;
    volatile long casButtOld;
    volatile long casTlacOld;
    int Enk();
    bool Butt();
    bool Tlac();
    void begin(byte _pinEnA, byte _pinEnB, byte _pinEnButt, byte _pinTlac);
};

//enkoder.h
enkoder enk;

void IRAM_ATTR EnkoderPootoceni2()
{
    enk.casEnkoder  =millis();
    if (enk.casEnkoder - enk.casEnkoderOld > 100)
    {
        //digitalRead(enk.pinEnA) =  0
        if (digitalRead(enk.pinEnB))
        {
            enk.smerEnk = 1;
        }
        else
        {
            enk.smerEnk = -1;
        }
    }
    enk.casEnkoderOld = enk.casEnkoder;
}


void  EnkoderButton2()
{
    enk.casButt = millis();
}


void Tlacitko2()
{
    enk.casTlac = millis();
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
    
    attachInterrupt(digitalPinToInterrupt(pinEnA),EnkoderPootoceni2,FALLING);
    attachInterrupt(digitalPinToInterrupt(pinEnButt),EnkoderButton2,HIGH);
    attachInterrupt(digitalPinToInterrupt(pinTlac),Tlacitko2,HIGH);
}


int enkoder::Enk()
{
    bool vys = smerEnk;
    smerEnk = 0;
        Serial.print("Enk:");
        Serial.println(vys);
        if(vys != 0)
        {
            delay(1000);
        }
    
    return vys;
}


bool enkoder::Butt()
{
    bool vys = 0;
    if (casButt != casButtOld)
    {
        if (casButt - casButtOld > 200)
        {
            vys = 1;
        }
        casButtOld = casButt;
        Serial.print("Butt:");
        Serial.println(vys);
    }
    
    return vys;
}


bool enkoder::Tlac()
{
    bool vys = 0;
    if (casTlac != casTlacOld)
    {
        if (casTlac - casTlacOld > 200)
        {
            vys = 1;
        }
        casTlacOld = casTlac;
        Serial.print("Tlac:");
        Serial.println(vys);
    }
    
    return vys;
}


#endif