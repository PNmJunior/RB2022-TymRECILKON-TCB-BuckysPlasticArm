#ifndef komPot
#define komPot

#include <Arduino.h>

#define SouborZnak ";"
#define nahradaSouborZnak " ///1||| "
#define SubSouborZnak ":"
#define nahradaSubSouborZnak " ///2||| " 


struct udaj
{
    String str;
};


class KomProtokolSend
{
private:
    /* data */
public:
    udaj balicekInt(int a);
    udaj balicekText(String a);
    String addSoubor(udaj a);
    String addSubSoubor(udaj a);
    String motor(int mot, int hod);
    KomProtokolSend(/* args */);
    ~KomProtokolSend();
};

KomProtokolSend::KomProtokolSend(/* args */)
{
}

KomProtokolSend::~KomProtokolSend()
{
}


inline udaj KomProtokolSend::balicekInt(int a)
{
    udaj i;
    i.str = String(a);
    return i ;
}


udaj KomProtokolSend::balicekText(String a)
{
    udaj i;
    i.str = a;
    i.str.replace(SouborZnak, nahradaSouborZnak);
    i.str.replace(SubSouborZnak, nahradaSubSouborZnak);
    return i;
}


inline String KomProtokolSend::addSoubor(udaj a)
{
    return SouborZnak + a.str;
}


inline String KomProtokolSend::addSubSoubor(udaj a)
{
    return SubSouborZnak + a.str;
}


inline String KomProtokolSend::motor(int mot, int hod)
{
    return addSoubor(balicekText("m"))+addSubSoubor(balicekInt(mot)) + addSubSoubor(balicekInt(hod));
}

KomProtokolSend SendSystem;


class komunProtokol
{
private:
    int sizeM;
    int posledniSoubor;
    String str;
    void ZvetstiZasobnik();
    void pocetVAktualSouboru2();
public:
    int pocetSouboru = 0;
    int pocetPrvku = 0;
    int *indexyStart;
    int *pocetSubsouboru; //0=subsoubor; 0> hlavni soubor s poctem subsouboru
    int pocet = 0;
    const int end = 10000;
    const int subSoubor = 0;
    void begin(String STR);
    int readInt();
    String readStr();
    bool isMesterSoubor();
    bool isEND();
    int pocetVAktualSouboru();
    int indentifikace_pocet();
    String sendAktSoubor();

    komunProtokol();
    ~komunProtokol();
};

komunProtokol::komunProtokol()
{

}
komunProtokol::~komunProtokol()
{

}


String komunProtokol::sendAktSoubor()
{
    return str.substring(indexyStart[posledniSoubor], indexyStart[posledniSoubor + pocetSubsouboru[posledniSoubor]]);
}


inline void komunProtokol::pocetVAktualSouboru2()
{
    if (isMesterSoubor())
    {
        posledniSoubor = pocet;
    }
}

inline int komunProtokol::pocetVAktualSouboru()
{
    return pocetSubsouboru[posledniSoubor];
}


inline int komunProtokol::indentifikace_pocet()
{
    return pocetSubsouboru[pocet];
}

inline bool komunProtokol::isMesterSoubor()
{
    if (pocetSubsouboru[pocet] == 0)
    {
        return 0;
    }
    return 1;
}


inline bool komunProtokol::isEND()
{
    if(pocet > pocetPrvku){return 0;}
    return 1;
}


int komunProtokol::readInt()
{
    if(pocet > pocetPrvku){return end;}
    pocetVAktualSouboru2();
    return str.substring(indexyStart[pocet] + 1,indexyStart[++pocet ]).toInt();
}


String komunProtokol::readStr()
{
    if(pocet > pocetPrvku){return "end";}
    pocetVAktualSouboru2();
    String a = str.substring(indexyStart[pocet] + 1,indexyStart[++pocet ]);
    a.replace(nahradaSouborZnak,SouborZnak);
    a.replace(nahradaSubSouborZnak,SubSouborZnak);
    return a;
}


inline void komunProtokol::ZvetstiZasobnik()
{
    if (sizeM == pocetPrvku)
    {
        sizeM += 10;
        indexyStart = (int *)realloc(indexyStart,sizeM + 1);
        pocetSubsouboru = (int *)realloc(pocetSubsouboru,sizeM+1);
    }
}


void komunProtokol::begin(String STR)
{
    str = STR;
    char pism;
    sizeM = 10;
    indexyStart = (int *)malloc(sizeM + 1);
    pocetSubsouboru = (int *)malloc(sizeM + 1);
    for (int i = 0; i < str.length(); i++)
    {
        pism = str.charAt(i);
        if (pism == ';')
        {
            pocetSouboru ++;
            pocetPrvku ++;
            ZvetstiZasobnik();
            posledniSoubor = pocetPrvku -1;
            indexyStart[posledniSoubor] = i;
            pocetSubsouboru[posledniSoubor] = 1;
        }
        else if (pism == ':')
        {
            pocetPrvku ++;
            ZvetstiZasobnik();
            indexyStart[pocetPrvku -1] = i;
            pocetSubsouboru[pocetPrvku -1] = 0;
            pocetSubsouboru[posledniSoubor] ++;
        }
    }
    indexyStart[pocetPrvku] = str.length();
    pocetSubsouboru[pocetPrvku] = end;
}


#endif komPot