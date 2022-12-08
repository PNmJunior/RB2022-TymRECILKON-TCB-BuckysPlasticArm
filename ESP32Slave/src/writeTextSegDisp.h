#ifndef _WriteTextSegDisp_
#define _WriteTextSegDisp_

#include <Arduino.h>
#include "menu.h"
#include "disp.h"
#include "enkoder.h"

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
    
    byte indexStart = index/4;
    indexStart = index *4;
    byte indexEnd  =(indexStart + 3);
    if (indexEnd > st.length())
    {
        indexEnd = st.length();
    }
    displej->del();
    
    displej->addText4( st.substring(indexStart,indexEnd));
}


String writeTextSegDisp::SetChar(String st,byte index)
{
    enk->Tlac();
    enk->Butt();
    bool tl;
    bool bt;
    char pism = 'A';
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
            pism -= 'a'-'A';
            st[index] = pism;
        }
    }
    if (pism >= 'a' && pism <= 'z')
    {
        if (bt)
        {
            pism += 'a'-'A';
            st[index] = pism;
        }
    }
    
    if (pism == 127)
    {
        st.remove(index);
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
    byte s = millis();
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
                if (index + 1 < st.length())
                {
                    index ++;
                }
                else if (index + 1 == st.length() && st[index] != '_')
                {
                    st += "_";
                    index  = st.length()-1;
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
            o = true;
            mezara[index] = ' ';
            s = millis() +2000;
        }
        if (enk->Butt())
        {
            st = SetChar(st,index);
            if (st.length() == index)
            {
                index = st.length() -1;
            }
            
        }

        if (millis()-s>1000)
        {
            s  =millis();
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

#endif