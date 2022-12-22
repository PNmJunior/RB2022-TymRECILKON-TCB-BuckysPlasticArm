#ifndef Menu
#define Menu

#include <Arduino.h>
#include <disp.h>
#include <enkoder.h>

struct komunFace
{
    disp *displej;
    enkoder *enk;
    HardwareSerial *ser;
};


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
    String nadpis;
    long casNadpis;
public:
    polozka newPolozka(int _indexButt,int _indexTlac,String _segD,long _cas);
    void addPolozkaData(int _indexButt,int _indexTlac,String _segD,long _cas);
    void addPolozka(polozka pol);
    int work();
    void begin(komunFace face, String nadp , long casNadp);
    void begin(disp *_displej,enkoder *_enk, HardwareSerial *_ser, String nadp , long casNadp);
};


void menu::begin(disp *_displej,enkoder *_enk, HardwareSerial *_ser, String nadp = "", long casNadp = 2000)
{
    pocet = 0;
    polozky = (polozka *)malloc(sizeof(polozka));
    displej = _displej;
    enk  = _enk;
    ser = _ser;
    nadpis = nadp;
    casNadpis = casNadp;
}


void menu::begin(komunFace face, String nadp = "", long casNadp = 2000)
{
    begin(face.displej,face.enk, face.ser,nadp, casNadp);
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
    polozky[pocet].cas = pol.cas;
    polozky[pocet].indexButt = pol.indexButt;
    polozky[pocet].indexTlac = pol.indexTlac;
    polozky[pocet].segD = pol.segD;
    pocet++;
}


void menu::addPolozkaData(int _indexButt,int _indexTlac,String _segD,long _cas)
{
    addPolozka(newPolozka(_indexButt, _indexTlac, _segD, _cas));
}


int menu::work()
{
    if(nadpis != "")
    {
        ser->println(nadpis);
        displej->addText4(nadpis, casNadpis);
    }
    for (size_t i = 0; i < pocet; i++)
    {
        displej->addText4(polozky[i].segD,polozky[i].cas);
    }
    enk->Tlac();
    enk->Butt();
    while (ser->available())
    {
        ser->read();
    }
    int alfa  = 0;
    long casZnovu;
    bool tl;
    bool bt;
    displej->addText4(polozky[0].segD,polozky[0].cas);
    do
    {
        int u  =enk->Enk();
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
        bt = enk->Butt();
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
    int m = polozky[alfa].indexButt;
    if (tl == true)
    {
        m= polozky[alfa].indexTlac;
    }
    for (size_t i = 0; i < pocet; i++)
    {
        free(polozky[i].segD);
    }
    
    free(polozky);
    return m;
}





class writeTextSegDisp
{
private:
    disp *displej;
    enkoder *enk;
    HardwareSerial *ser;
    void Print(String st, byte index);
public:
    void begin(komunFace face);
    void begin(disp *_displej,enkoder *_enk, HardwareSerial *_ser);
    String WriteText(String st);
    String WriteTextSerial(String st);
    String WriteTextVyber(String st);
    String SetChar(String st,byte ind);
};

void writeTextSegDisp::begin(disp *_displej,enkoder *_enk, HardwareSerial *_ser)
{
    displej = _displej;
    enk  = _enk;
    ser = _ser;
}

void writeTextSegDisp::begin(komunFace face)
{
    begin(face.displej,face.enk,face.ser);
}

void writeTextSegDisp::Print(String st, byte index)
{
    byte indexStart = index / 4;
    indexStart = indexStart * 4;
    displej->del();
    displej->addText4( st.substring(indexStart, indexStart + 4));
}


String writeTextSegDisp::SetChar(String st,byte index)
{
    enk->Tlac();
    enk->Butt();
    bool tl;
    bool bt;
    char pism = st.charAt(index);
    Print(st,index);
    char s = 0;
    do
    {
        int u = enk->Enk();
        if (u != 0)
        {

            if (u == 1 && pism <= 127)
            {
                pism ++;
                
            }
            else if (u == -1 && pism >= ' ')
            {
                pism --;
            }
            st[index] = pism;
            Print(st,index);
        }
        tl = enk->Tlac();
        bt = enk->Butt();
        if (ser->available())
        {
            s = ser->read();
        }
        
    } while (tl == false && bt == false && s ==0);
    if (s != 0)
    {
        pism = s;
        tl = false;
        bt = false;
    }
    
    if (pism >= 'A' && pism <= 'Z')
    {
        if (tl)
        {
            pism += 'a'-'A';
            st[index] = pism;
        }
    }
    if (pism >= 'a' && pism <= 'z')
    {
        if (bt)
        {
            pism -= 'a'-'A';
            st[index] = pism;
        }
    }
    
    if (pism == 127)
    {
        st.remove(index,1);
    }
    Print(st,index);
    return st;
}


String writeTextSegDisp::WriteText(String st = " ")
{
    enk->Tlac();
    enk->Butt();
    byte index = 0;
    String mezara = st;           
    mezara[0] = ' ';
    long s = millis();
    bool o;
    while (ser->available())
    {
        ser->read();
    }
    Print(st,index);
    do
    {
        int u = enk->Enk();
        if (u != 0)
        {
            if (u == 1)
            {
                index ++;
                if (index  == st.length())
                {
                    st += "_";
                }                
            }
            else if (u == -1)
            {
                if (index != 0)
                {
                    index --;
                }
            }
            mezara =st;            
            mezara[index] = ' ';
            o = true;
            s = millis();
            Serial.print("Index:");
            Serial.println(index);
        }
        if (enk->Butt())
        {
            st = SetChar(st,index);
            if (st.length() == index)
            {
                index = st.length() -1;
            }
            mezara =st;            
            mezara[index] = ' ';   
        }
        if ( s < millis())
        {
            s = millis() +200;
            if (o)
            {
                Print(st,index);
            }
            else
            {
                Print(mezara,index);
            }
            o = !o;
        }       
   } while (enk->Tlac() == false);
    return st;
}


String writeTextSegDisp::WriteTextSerial(String st)
{
    String p;
    displej->addText4("SeWr");
    bool a;
    long t;
    char k = 0;
    ser->println(st);
    do
    {
        while (ser->available()>0)
        {
            ser->read();
        }
        ser->println("Muzes poslat text:");
        while (ser->available()==0)
        {
            t = millis();
        }
        while (ser->available()>0)
        {
            char o = ser->read();
            p += o;
            t = millis();
            Serial.println(ser->available());
            Serial.println(o);
            Serial.println(o,10);
            delay(10);            
        }
        ser->println("Tvuj text:");
        ser->println(p);
        ser->println("Chces to ulozit? A/N/C:");
        while (ser->available()==0)
        {
            delay(50);
        } 
        k = ser->read()  ;
        if (k == 'C' || k == 'c' || k == 'n' || k == 'N')
        {
            p.clear();
        }
    } while (!(k == 'C' || k == 'c' || k == 'A' || k == 'a'));
    return p;
}


String writeTextSegDisp::WriteTextVyber(String st)
{
    menu rozhodnuti;
    rozhodnuti.begin(displej,enk,ser,st,1000);
    rozhodnuti.addPolozkaData(1,1,"Enk",1000);
    rozhodnuti.addPolozkaData(2,2,"Seri",1000);
    rozhodnuti.addPolozkaData(3,3,"Clos",1000);
    String p;
    switch (rozhodnuti.work())
    {
    case 1:
        return WriteText(st);
        break;
    case 2:
        return WriteTextSerial(st);
        break;
    default:
        break;
    }
    return p;
}


writeTextSegDisp editText;

#endif