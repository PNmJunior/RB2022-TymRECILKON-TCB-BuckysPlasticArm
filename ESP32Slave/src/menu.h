#ifndef Menu
#define Menu

#include <Arduino.h>
#include <disp.h>
#include <enkoder.h>


struct polozka
{
    int indexButt;
    int indexTlac;
    String segD;
    long cas;
};


struct menuSez
{
    byte pocet;
    polozka *polozky;
    
};


class menu
{
private:
    byte pocet;
    polozka *polozky;
    disp *displej;
    enkoder *enk;
public:
    void addPolozka(polozka a);
    int work();
    void begin();
    String WriteText(String a);
    String SetChar(String a,byte ind);
    void Print(String a, byte index);
    void menuFree();
};


void menu::addPolozka(polozka a)
{
    polozky =(polozka*) realloc(polozky,(pocet+1) * sizeof(polozka));
    polozky[pocet]  = a;
    pocet++;
}


int menu::work()
{
    enk->Tlac();
    enk->Butt();
    for (size_t i = 0; i < pocet; i++)
    {
        displej->addText4(polozky[i].segD,polozky[i].cas);
    }
    int alfa  = 0;
    long casZnovu;
    bool tl;
    bool bt;
    displej->addText4(polozky[0].segD,polozky[0].cas);
    do
    {
        int u  =enk->Enk();
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
            displej->dell();
            casZnovu = polozky[alfa].cas + millis() + 2000;
            displej->addText4(polozky[alfa].segD,polozky[alfa].cas);
        }
        else if (casZnovu < millis())
        {
            displej->dell();
            casZnovu = polozky[alfa].cas + millis() + 2000;
            displej->addText4(polozky[alfa].segD,polozky[alfa].cas);
        }
        tl = enk->Tlac();
        bt = enk->Butt();
        
    } while (tl == false && bt == false);
    if (tl == true)
    {
        return polozky[alfa].indexTlac;
    }
    return polozky[alfa].indexButt;
}


void menu::Print(String a, byte index)
{
    
    byte indexStart = index/4;
    indexStart = index *4;
    byte indexEnd  =(indexStart + 3);
    if (indexEnd > a.length())
    {
        indexEnd = a.length();
    }
    displej->dell();
    
    displej->addText4( a.substring(indexStart,indexEnd));
}


String menu::SetChar(String a,byte index)
{
    enk->Tlac();
    enk->Butt();
    bool tl;
    bool bt;
    char pism = 'A';
    Print(a,index);
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
            a[index] = pism;
            Print(a,index);
        }
                tl = enk->Tlac();
        bt = enk->Butt();
    } while (tl == false && bt == false);
    if (pism >= 'A' && pism <= 'Z')
    {
        if (tl)
        {
            pism -= 'a'-'A';
        }
    }
    if (pism >= 'a' && pism <= 'z')
    {
        if (bt)
        {
            pism += 'a'-'A';
        }
    }
    a[index] = pism;
    if (pism == 127)
    {
        a.remove(index);
    }
    
    
    Print(a,index);
    return a;
    
}


String menu::WriteText(String a = " ")
{
    enk->Tlac();
    enk->Butt();
    byte index = 0;
    String mezara = a;
    byte s = millis();
    bool o;
    
    Print(a,index);
    do
    {
        int u = enk->Enk();
        if (u != 0)
        {
            if (u == 1)
            {
                if (index + 1 < a.length())
                {
                    index ++;
                }
                else if (index + 1 == a.length() && a[index] != '_')
                {
                    a += "_";
                    index  = a.length()-1;
                }                
            }
            else if (u == -1)
            {
                if (index != 0)
                {
                    index --;
                }
            }
            mezara =a;
            o = true;
            mezara[index] = ' ';
            s = millis() +2000;
        }
        if (enk->Butt())
        {
            a = SetChar(a,index);
            if (a.length() == index)
            {
                index = a.length() -1;
            }
            
        }

        if (millis()-s>1000)
        {
            s  =millis();
            if (o)
            {
                Print(a,index);
            }
            else
            {
                Print(mezara,index);
                
            }
            o = !o;
        }
   } while (enk->Tlac() == false);
    return a;
}

void menu::menuFree()
{
    free(polozky);
}

#endif