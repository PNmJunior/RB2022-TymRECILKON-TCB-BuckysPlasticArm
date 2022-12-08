#ifndef Menu
#define Menu

#include <Arduino.h>
#include <disp.h>
#include <enkoder.h>
#include <spolecneStruktury.h>

#define menuDobaZob 1000

//Menu
struct komunFace
{
    disp *displej;
    enkoder *enk;
    HardwareSerial *ser;
};

//Menu
struct polozka
{
    int indexButt;
    int indexTlac;
    //String segD;
    char *segD;
    long cas;
};

class menu
{
private:
    byte pocet;
    polozka *polozky;
    disp *displej;
    enkoder *enk;
    HardwareSerial *ser;
public:
    polozka newPolozka(int _indexButt,int _indexTlac,String _segD,long _cas);
    void addPolozkaData(int _indexButt,int _indexTlac,String _segD,long _cas);
    void addPolozka(polozka pol);
    int work();
    void begin(komunFace face);
    void begin(disp *_displej,enkoder *_enk, HardwareSerial *_ser);
    void menuFree();
};


void menu::begin(disp *_displej,enkoder *_enk, HardwareSerial *_ser)
{
    pocet = 0;
    polozky = (polozka *)malloc(sizeof(polozka));
    displej = _displej;
    enk  = _enk;
    ser = _ser;
}

void menu::begin(komunFace face)
{
    begin(face.displej,face.enk, face.ser);
}

polozka menu::newPolozka(int _indexButt,int _indexTlac,String _segD,long _cas)
{
    polozka p;
    p.cas = _cas;
    p.indexButt = _indexButt;
    p.indexTlac = _indexTlac;
    p.segD = (char *)malloc(_segD.length()+1);
    for (size_t i = 0; i < _segD.length(); i++)
    {
        p.segD[i] = _segD[i];
    }
    p.segD[_segD.length()] = 0;
    return p;
}


void menu::addPolozka(polozka pol)
{
    String x;
    polozky =(polozka*) realloc(polozky,(pocet+1) * sizeof(polozka));
    Serial.println('u');
    delay(1000);
    Serial.println(pocet);
    Serial.println(sizeof(polozka));
    Serial.println(sizeof(pol));
    Serial.println(sizeof(polozky));
    Serial.println(sizeof(polozky[0]));
    Serial.println(sizeof(x));
    polozky[pocet].cas = pol.cas;
    Serial.println("f1");
    polozky[pocet].indexButt = pol.indexButt;
     Serial.println("f2");
    polozky[pocet].indexTlac = pol.indexTlac;
     Serial.println("f3");
    //polozky[pocet].segD = String(pol.segD.c_str());
    polozky[pocet].segD = pol.segD;
    Serial.println('v');
    pocet++;
    Serial.println('w');
}

void menu::addPolozkaData(int _indexButt,int _indexTlac,String _segD,long _cas)
{
    addPolozka(newPolozka(_indexButt, _indexTlac, _segD, _cas));
}

int menu::work()
{
    ser->println("a");
    for (size_t i = 0; i < pocet; i++)
    {
        displej->addText4(polozky[i].segD,polozky[i].cas);
        //displej->addText4(String(i),polozky[i].cas);
    }
    ser->println("b");
    enk->Tlac();
    enk->Butt();
    ser->println("c");
    while (ser->available())
    {
        ser->read();
    }
    ser->println("d");
    int alfa  = 0;
    long casZnovu;
    bool tl;
    bool bt;
    ser->println("e");
    displej->addText4(polozky[0].segD,polozky[0].cas);
    ser->println("f");
    do
    {
        int u  =enk->Enk();
        ser->print("E:");ser->println(u);
        char s ;
        if (ser->available())
        {
            s = ser->read();
        }
        else
        {
            s = 0;
        }

        if (s != 0)
        {
            ser->print("S:");ser->println(s);
            switch (s)
            {
            case 'n':
            case 'N':
                u = 1;
                s = 0;
                break; 
            case 'd':
            case 'D':
                u = -1;
                s = 0;
                break;           
            default:
                break;
            }
        }
        if (u != 0)
        {
            alfa += u;
            if (alfa < 0)
            {
                alfa = pocet -1;
            }
            if (alfa >= pocet)
            {
                alfa = 0;
            }     
            displej->del();
            casZnovu = polozky[alfa].cas + millis() + 2000;
            displej->addText4(polozky[alfa].segD,polozky[alfa].cas);
        }
        /*else if (casZnovu < millis())
        {
            displej->del();
            casZnovu = polozky[alfa].cas + millis() + 2000;
            displej->addText4(polozky[alfa].segD,polozky[alfa].cas);
        }*/
        tl = enk->Tlac();
        ser->print("T:");ser->println(tl);
        bt = enk->Butt();
        ser->print("B:");ser->println(bt);
        if (s == 0 && ser->available())
        {
            s= ser->read();
        }
        if (s != 0)
        {
            switch (s)
            {
            case 't':
            case 'T':
                tl = true;
                s = 0;
                break;
            case 'b':
            case 'B':
                bt = true;
                s = 0;
                break;           
            default:
                break;
            }
        }
    } while (tl == false && bt == false);
    if (tl == true)
    {
        return polozky[alfa].indexTlac;
    }
    return polozky[alfa].indexButt;
}

void menu::menuFree()
{
    for (size_t i = 0; i < pocet; i++)
    {
        free(polozky[i].segD);
    }
    
    free(polozky);
}

#endif