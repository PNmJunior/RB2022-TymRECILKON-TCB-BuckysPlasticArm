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
    volatile bool butt;
    volatile bool tlac;
    volatile byte funkce;
     int Enk();
     bool Butt();
     bool Tlac();
     void begin(byte _pinEnA, byte _pinEnB, byte _pinEnButt, byte _pinTlac,void (*ISR_callback)(void));
};


 enkoder enk;



void IRAM_ATTR EnkoderPootoceni2()
{
    enk.stavEnk = enk.stavEnk >> 2;
    if (digitalRead(enk.pinEnA))
    {
        enk.stavEnk = enk.stavEnk | B1000;
        Serial.print('a');
    }
    if (digitalRead(enk.pinEnB))
    {
        enk.stavEnk = enk.stavEnk | B0100;
        Serial.print('b');
    }
    
    switch (enk.stavEnk)
    {
    
    case B0001:
    case B0111:
    case B1000:
    case B1110:
    enk.smerEnk = 1;
    break;
    case B0010:
    case B0100:
    case B1011:
    case B1101:
    enk.smerEnk = -1;
    break;
    case B0011:
    case B1100:
    enk.smerEnk = 1;
    break;
    case B0110:
    case B1001:
    enk.smerEnk = -1;
    break;    
    default:
    enk.smerEnk = 0;
        break;
    }  
    Serial.println(enk.smerEnk);
}


void IRAM_ATTR EnkoderPootoceni(void * arg)
{
    
    enkoder *en = (enkoder*)arg;
    en->stavEnk = en->stavEnk >> 2;
    en->funkce ++;
    if (digitalRead(en->pinEnA))
    {
        en->stavEnk = en->stavEnk | B1000;
    }
    if (digitalRead(en->pinEnB))
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


void enkoder::begin(byte _pinEnA, byte _pinEnB, byte _pinEnButt, byte _pinTlac,void (*ISR_callback)(void))
{
    pinEnA = _pinEnA;
    pinEnB = _pinEnB;
    pinEnButt = _pinEnButt;
    pinTlac = _pinTlac;
    pinMode(pinEnA,INPUT);
    pinMode(pinEnB,INPUT);
    pinMode(pinEnButt,INPUT);
    pinMode(pinTlac,INPUT);
    //touchAttachInterruptArg(pinEnB, EnkoderPootoceni,this, CHANGE);
    //touchAttachInterruptArg(pinEnButt,EnkoderButton,this, FALLING);
    //touchAttachInterruptArg(pinTlac,Tlacitko,this, FALLING);
    attachInterrupt(digitalPinToInterrupt(pinEnA),EnkoderPootoceni2,HIGH);
    //attachInterrupt(pinEnA,ISR_callback,CHANGE);

}


int enkoder::Enk()
{
    int l  =smerEnk;
    smerEnk = 0;
    Serial.print("Enk()");Serial.println(l);
    return l;
}


bool enkoder::Butt()
{
    Serial.print("Butt()");
    if (butt == true)
    {
        Serial.println("1");
        butt = false;
        return true;
    }
    Serial.println("0");
    return false;
}


bool enkoder::Tlac()
{
    Serial.print("Tlac()");
    if (tlac == true)
    {
        Serial.println("1");
        tlac = false;
        return true;
    }
    Serial.println("0");
    return false;
}


#endif