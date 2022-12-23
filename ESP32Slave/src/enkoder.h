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

#define casZakliku 50

class enkoder
{
private:

public:
    volatile byte pinEnA;
    volatile byte pinEnB;
    volatile byte pinEnButt;
    volatile byte pinTlac;
    volatile long casEnkoder;
    volatile long casEnkoderA;
    volatile long casEnkoderB;
    volatile long casEnkoderAold;
    volatile long casEnkoderBold;
    volatile long casButtLow;
    volatile long casButtHigh;
    volatile long casTlacLow;
    volatile long casTlacHigh;
    volatile long casButtHighOld;
    volatile long casButtLowOld;
    volatile long casTlacHighOld;
    volatile long casTlacLowOld;
    volatile long casTlacRozdil;
    volatile long casButtRozdil;
    int Enk();
    bool Butt();
    bool Tlac();
    void begin(byte _pinEnA, byte _pinEnB, byte _pinEnButt, byte _pinTlac);
    long ButtCas();
    long TlacCas();
};

//enkoder.h
enkoder enk;

void IRAM_ATTR EnkoderPootoceni2A()
{
    enk.casEnkoderA  =millis();
}



void IRAM_ATTR EnkoderPootoceni2B()
{
    enk.casEnkoderB  =millis();
}


void IRAM_ATTR EnkoderButton2()
{
    long t=millis();
    if (digitalRead(enk.pinEnButt))
    {
        //enk.casButtHigh = t;
    }
    else if(t- enk.casButtHigh > casZakliku)
    {
        enk.casButtLow = t;
    }
}


void IRAM_ATTR Tlacitko2()
{
    long t=millis();
    if (digitalRead(enk.pinTlac))
    {
        enk.casTlacHigh = t;
    }
    else if(t- enk.casTlacHigh > casZakliku)
    {
        enk.casTlacLow = t;
    }
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
    
    attachInterrupt(digitalPinToInterrupt(pinEnA),EnkoderPootoceni2A,FALLING);
    attachInterrupt(digitalPinToInterrupt(pinEnB),EnkoderPootoceni2B,FALLING);
    attachInterrupt(digitalPinToInterrupt(pinEnButt),EnkoderButton2,HIGH);
    attachInterrupt(digitalPinToInterrupt(pinTlac),Tlacitko2,CHANGE);
}


int enkoder::Enk()
{
    //Serial.print("A:");Serial.print(casEnkoderA);Serial.print("B:");Serial.print(casEnkoderB);Serial.print("T:");Serial.print(casEnkoder); delay(1000);
    if (casEnkoderA == casEnkoderB || casEnkoderA == 0 || casEnkoderB == 0)
    {
        return 0;
    }
    else if (casEnkoderAold != casEnkoderA && casEnkoderBold != casEnkoderB)
    {
        casEnkoderAold = casEnkoderA ; 
        casEnkoderBold = casEnkoderB;
        if (millis() - casEnkoder < 80)
        {
            return 0;
        }
        casEnkoder = millis();
        if (casEnkoderA > casEnkoderB)
        {
            return 1;
        }
        else
        {
             return -1;
        }

    }
    
    return 0;
}


bool enkoder::Butt()
{
    bool vys = 0;
    if (casButtHigh != casButtHighOld)
    {
        /* code */
    }
    
    if (casButtHigh != casButtHighOld)
    {
        if (casButtHigh - casButtHighOld > casZakliku)
        {
            vys = 1;
            casButtRozdil = casButtHigh - casButtLow ;
            casButtHighOld = casButtHigh;
        }
        
        Serial.print("Butt:");
        Serial.println(casButtHigh);
    }
    
    return vys;
}


long inline enkoder::ButtCas()
{
    return casButtRozdil;
}

bool enkoder::Tlac()
{
    bool vys = 0;
    if (casTlacHigh != casTlacHighOld)
    {
        if (casTlacHigh - casTlacHighOld > casZakliku)
        {
            vys = 1;
            casTlacRozdil = casTlacHigh - casTlacLow ;
        }
        casTlacHighOld = casTlacHigh;
        Serial.print("Tlac:");
        Serial.println(vys);
    }
    
    return vys;
}

long inline enkoder::TlacCas()
{
    return casTlacRozdil;
}


#endif