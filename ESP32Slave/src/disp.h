#ifndef kn_disp
#define kn_disp

#include <Arduino.h>
#include <binary.h>
#include <WiFi.h>
#define mWork 250

struct textVelky
{
    byte Start;
    byte End;
    byte pocet;
};


struct textik
{
    long timeTrvani;
    char textNow[4];
    byte Aline[4];
    byte vystup[4] = {0,0,0,0};
};


class disp
{
private:
    volatile long nextTime;
    volatile textik work[mWork];
    volatile byte vystupWork;
    volatile byte maxWork;
    byte segReal[8];
    volatile byte vystupInd = 0;
    HardwareSerial *ser;
    void nextTimeSet(byte vW);
    bool nextTimeIs();

public:
    void nextTimeSetOKolik(long oCas);
    void nextTimeSetNull();
    void begin(HardwareSerial *_ser,byte sA,byte sB, byte sC, byte sD, byte sE, byte sF, byte sG, byte sH,char show0,char show1,char show2, char show3);
    byte toSegment(char znak);
    byte toReal(byte segm);
    byte addText4Char(char z0 , char z1 , char z2 , char z3 , long dobaTrvani , byte tecky ,byte neg, byte setIndex );
    textVelky addText4(String text, long dobaTrvani, long tecky ,long neg );
    textVelky addText4IP(IPAddress ip, long dobaTrvani);
    byte vystup();
    void vystupEX();
    byte index();
    long BitSet(byte index);
    void del();

    byte indexView();
    bool isIndexView(byte ind);
    bool isIndexView(textVelky ppp);
    String textIndex(byte ind);

    const byte a  = B10000000;
    const byte b  = B1000000;
    const byte c  = B100000;
    const byte d  = B10000;
    const byte e  = B1000;
    const byte f  = B100;
    const byte g  = B10;
    const byte hp  = B1;
    const byte dp  = hp;

    const byte segNumber[10] = 
    {
        a|b|c|d|e|f,   //0
        e|f,               //1
        a|b|d|e|g,      //2
        a|b|c|d|g,      //3
        b|c|f|g,         //4
        a|c|d|f|g,      //5
        a|c|d|e|f|g,   //6
        a|b|c,            //7
        a|b|c|d|e|f|g,//8
        a|b|c|f|g,      //9
    };

    const byte segAbc[26] = 
    {
        a|b|c|e|f|g,   //A
        c|d|e|f|g,      //B
        d|e|g,            //C
        b|c|d|e|g,      //D
        a|d|e|f|g,      //E
        a|e|f|g,         //F
        a|b|c|d|f|g,   //G
        c|e|f|g,         //H
        e,                  //I
        b|c|d|e,         //J
        e|f|g,            //K
        d|e|f,            //L
        a|b|c|e|f,      //M
        c|e|g,            //N
        c|d|e|g,         //O
        a|b|e|f|g,      //P
        a|b|c|f,         //Q
        e|g,               //R
        a|c|d|f|g,      //S
        a|e|f,            //T
        b|c|d|e|f,      //U
        b|e|f,            //V
        b|c|e|f,         //W
        b|c|e|f|g,      //X
        b|e|f|g,         //Y
        a|b|d|e,         //Z
    };
};


inline void disp::nextTimeSetOKolik(long oCas)
{
    nextTime = oCas + millis();
}


inline void disp::nextTimeSet(byte vW)
{
    nextTimeSetOKolik(work[vW].timeTrvani);
}


inline void disp::nextTimeSetNull()
{
    nextTime = 0;
}


inline bool disp::nextTimeIs()
{
    if (nextTime < millis())
    {
        return true;
    }
    return false;    
}


inline byte disp::toReal(byte segm)
{
    byte alfa = 0;
    for (int i = 0; i < 8; i++)
    {
        if ((segm & (B10000000 >> i)) != 0)
        {
            alfa |= segReal[i];
        }
    }
    return alfa;
}


byte disp::addText4Char(char z0 , char z1 , char z2 , char z3 , long dobaTrvani = 0, byte tecky =0,byte neg = 0, byte setIndex = 255)
{
    byte maxWork2 = maxWork;
    if (setIndex == 255)
    {
        maxWork2 ++;
    }
    else if (setIndex < mWork)
    {
        maxWork2 = setIndex;
    }
    else
    {
        return 254;
    }
    
    if (maxWork2 == mWork)
    {
        maxWork2 = 0;
    }
    Serial.print(z0);
    Serial.print(z1);
    Serial.print(z2);
    Serial.print(z3);
    Serial.print('>');
    work[maxWork2].textNow[0] = z0;
    work[maxWork2].textNow[1] = z1;
    work[maxWork2].textNow[2] = z2;
    work[maxWork2].textNow[3] = z3;
    work[maxWork2].timeTrvani = dobaTrvani;
    String m   ="                index:";
    for (int i = 0; i < 4; i++)
    {
        work[maxWork2].Aline[i] = toSegment(work[maxWork2].textNow[i]);
        m[i*4] = work[maxWork2].textNow[i];
        if ((tecky & (B1000 >> i))!=0)
        {
            work[maxWork2].Aline[i] |= dp;
            m[i*4 +1] = '.';
        }
        if ((neg & (B1000 >> i))!=0)
        {
            work[maxWork2].Aline[i] = ~work[maxWork2].Aline[i];
            m[i*4 +2] = 'n';
        }        
        work[maxWork2].vystup[i]= toReal(work[maxWork2].Aline[i]);
    }
    m += String(maxWork2) + " cas:" + String(dobaTrvani);
    ser->println(m);
    if (setIndex == 255)
    {
        maxWork = maxWork2;//vložení textu do fronty
    }
    /*
    for (size_t i = 0; i < 4; i++)
    {
        
        ser->print(work[maxWork2].textNow[i]);
        ser->print("=>");
        ser->print(work[maxWork2].textNow[i],10);
        ser->print("=>");
        ser->print(work[maxWork2].textNow[i],2);
        ser->print("=>");
        ser->print(work[maxWork2].Aline[i],2);
        ser->print("=>");
        ser->println(work[maxWork2].vystup[i],2);
        
       //ser->println(work[maxWork2].vystup[i],2);
    }
    */

   return maxWork;
}


void disp::begin(HardwareSerial *_ser,byte sA,byte sB, byte sC, byte sD, byte sE, byte sF, byte sG, byte sH,char show0 = 'i',char show1 = 'n',char show2 = 'i', char show3 = 't')
{
    ser  = _ser;
    segReal[0] = sA;
    segReal[1] = sB;
    segReal[2] = sC;
    segReal[3] = sD;
    segReal[4] = sE;
    segReal[5] = sF;
    segReal[6] = sG;
    segReal[7] = sH;
    vystupWork = 0;
    maxWork = 0;
    addText4Char(show0,show1,show2,show3);
    vystupWork = maxWork;
    nextTimeSet(vystupWork);
}


inline byte disp::toSegment(char znak)
{
    if(znak >= '0' && znak <= '9')
    {
        return segNumber[znak - '0'];
    }
    if (znak >= 'a' && znak <= 'z')
    {
        return segAbc[znak - 'a'];
    }
    if (znak >= 'A' && znak <= 'Z')
    {
        return segAbc[znak - 'A'] | hp;
    }
    switch (znak)
    {
    case ' ':
        return 0;
        break;
    case ',':
        return c;
        break;
    case '.':
        return dp;
        break;
    case ':':
        return d|g;
        break; 
    case '-':
        return g;
        break;
    case '*':
        return b|c|g|dp;
        break;
    case '+':
        return b|c|g;
        break;
    case '/':
        return b|g|e;
        break;
    case '_':
        return d;
        break;
    case '~':
        return g|dp;
        break;  
    case '[':
    case '{':
    case '(':
        return a|d|e|f;
        break;
    case ']':
    case '}':
    case ')':
            return a|b|c|d;
        break;
    case '!':
        return b|c|dp;
        break; 
    case '"':
        return b|f;
        break;  
    case '#':
        return a|b|g|f;
        break;  
    case '%':
        return b|g|e|dp;
        break; 
    case '&':
        return a|b|g|f|dp;
        break;
    case ';':
        return c|dp;
        break;
    case '<':
        return a|g|f;
        break;
    case '=':
        return a|g;
        break;
    case 39://'
        return b;
        break;
    case '>':
        return a|b|g;
        break;
    case '?':
        return a|b|d|e|g|dp;
        break;
    case '@':
        return a|b|c|d|e|f|g|dp;
        break;
    case 127://dell  //vymaze znak
        return a|b|c|d|e|f|dp;
        break;
    case 92:/*\*/ 
        return c|g|f;
        break;
    case '^':
        return a|b|f;
        break;
    case 96://`
        return a;
        break;
    case '|':
        return b|c;
        break;
    case '$':
        return a|d|g;
        break;
    default:
        break;
    }
    return 0;
}


inline long disp::BitSet(byte index)
{
    return 1<<index;
}


textVelky disp::addText4(String text = "    ", long dobaTrvani = 0, long tecky =0,long neg = 0)
{
    int nasob = text.length()/4;
    int poslednichZnaku = text.length()-(nasob*4);
    
    if (poslednichZnaku !=0)
    {
        for (int i = 0; i < 4-poslednichZnaku; i++)
        {
            text += ' ';
        }
        nasob++;
    }
    long doba = dobaTrvani/nasob;
    int u;  
    textVelky ppp;
    ppp.pocet = nasob;
    for (int i = 0; i < nasob; i++)
    {
        u = 4*i;
        ppp.End = addText4Char(text.charAt(u+0),text.charAt(u+1),text.charAt(u+2),text.charAt(u+3), doba,(tecky >>((nasob-1-i)*4))&B00001111,(neg >>((nasob-1-i)*4))&B00001111);
        if (i == 0)
        {
            ppp.Start = ppp.End;
        }
        
    }
    return ppp ;
}


inline byte disp::vystup()
{
    return work[vystupWork].vystup[vystupInd];
}


inline void disp::vystupEX()
{
    if (vystupInd == 3)
    {
        if ((maxWork != vystupWork)&& nextTimeIs())
        {
            vystupWork ++;
            if (vystupWork == mWork)
            {
               vystupWork =0;
            }    
            nextTimeSet(vystupWork);
        }
        vystupInd = 0;
    }
    else
    {
        vystupInd ++;
    }
}


inline byte disp::index()
{
    return vystupInd;
}


inline void disp::del()
{
    ser->println("Delete. Index z "+String(vystupWork) +" na "+String( maxWork));
    vystupWork = maxWork;
    work[vystupWork].timeTrvani = 0;
    nextTimeSetNull();
}


textVelky disp::addText4IP(IPAddress ip,long dobaTrvani = 1000)
{
    textVelky ppp;
    dobaTrvani = dobaTrvani /5;
    ppp.Start = addText4Char('I','P',':',' ',dobaTrvani);
    for (size_t i = 0; i < 4; i++)
    {
        String u = String( ip.operator[](i),10);
        while (u.length() < 4)
        {
            u = " " + u;
        }
        ppp.End = addText4Char(u[0],u[1],u[2],u[3],dobaTrvani,B1000>>i);
    }
    ppp.pocet = 5;
    return ppp;
}


inline byte disp::indexView()
{
    return vystupWork;
}


inline bool disp::isIndexView(byte ind)
{
    if (ind == vystupWork)
    {
        return true;
    }
    return false;
}


inline bool disp::isIndexView(textVelky ppp)
{
    if (ppp.Start <= vystupWork && ppp.End >= vystupWork)
    {
        return true;
    }
    return false;
}

inline String disp::textIndex(byte ind)
{
    String o;
    if (ind >= mWork)
    {
        return o;
    }
    for (size_t i = 0; i < 4; i++)
    {
        o += work[ind].textNow[i];
    }
    return o;
}

#endif