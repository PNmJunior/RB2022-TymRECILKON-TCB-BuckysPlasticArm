#ifndef kn_disp
#define kn_disp


#include <Arduino.h>
#include <binary.h>

#define BezOmezeniDoby 0
#define RealA B10000000
#define RealB B1000000
#define RealC B100000
#define RealD B10000
#define RealE B1000
#define RealF B100
#define RealG B10
#define RealHP B1
#define A B10000000
#define B B1000000
#define C B100000
#define D B10000
#define E B1000
#define F B100
#define G B10
#define HP B1
#define DP HP
#define mWork 10
/// @brief 
struct textik
{
    long nextTime;
    long timeTrvani;
    char textNow[4];
    byte Aline[4];
    byte vystup[4] = {0,0,0,0};
    byte vystupInd = 0;
    byte pocetCislic = 4;
};


class disp
{
private:
    textik work[mWork];
    byte vystupWork;
    byte maxWork;
    const byte segNumber[10] = {
        A||B||C||D||E||F,   //0
        E||F,               //1
        A||B||D||E||G,      //2
        A||B||C||D||E,      //3
        B||C||F||G,         //4
        A||B||C||F||G,      //5
        A||B||C||F||G||E,   //6
        A||B||C,            //7
        A||B||C||D||E||F||G,//8
        A||B||C||F||G,      //9
        };
    const byte segAbc[26] = {
        A||B||C||E||F||G,   //A
        C||D||E||F||G,      //B
        D||E||G,            //C
        B||C||D||E||G,      //D
        A||D||E||F||G,      //E
        A||E||F||G,         //F
        A||B||C||D||F||G,   //G
        C||E||F||G,         //H
        E,                  //I
        B||C||D||E,         //J
        E||F||G,            //K
        D||E||F,            //L
        A||B||C||E||F,      //M
        C||E||G,            //N
        C||D||E||G,         //O
        A||B||E||F||G,      //P
        A||B||C||F,         //Q
        E||G,               //R
        A||C||D||F||G,      //S
        A||E||F,            //T
        B||C||D||E||F,      //U
        B||E||F,            //V
        B||C||E||F,         //W
        B||C||E||F||G,      //X
        B||E||F||G,         //Y
        A||B||D||E,         //Z
    };
    byte segReal[8];

public:
    void begin(byte sA,byte sB, byte sC, byte sD, byte sE, byte sF, byte sG, byte sH);
    byte toSegment(char znak);
    byte toReal(byte segm);
    //void addText4(char z0, char z1, char z2, char z3, long dobaTrvani, byte tecky =0,byte neg = 0);
    void addText4Char(char z0 , char z1 , char z2 , char z3 , long dobaTrvani , byte tecky ,byte neg );
    byte addText4(String text, long dobaTrvani, long tecky ,long neg );
    byte vystup();
    void vystupEX();
    byte index();
    long BitSet(byte index);
    
    //    disp();
    //~disp();
};


void disp::addText4Char(char z0 , char z1 , char z2 , char z3 , long dobaTrvani = 0, byte tecky =0,byte neg = 0)
{
    byte maxWork2 = maxWork;
    maxWork2 ++;
    if (maxWork2 == mWork)
    {
        maxWork2 = 0;
    }
    
    work[maxWork2].textNow[0] = z0;
    work[maxWork2].textNow[1] = z1;
    work[maxWork2].textNow[2] = z2;
    work[maxWork2].textNow[3] = z3;
    work[maxWork2].timeTrvani = dobaTrvani;
    for (int i = 0; i < 4; i++)
    {
        work[maxWork2].Aline[i] = toSegment(work->textNow[i]);
        if ((tecky & (B1000 > i))!=0)
        {
            work[maxWork2].Aline[i] |= DP;
        }
        if ((neg & (B1000 > i))!=0)
        {
            work[maxWork2].Aline[i] = ~work[maxWork2].Aline[i];
        }        
        work[maxWork2].vystup[i]= toReal(work->Aline[i]);
    }
    maxWork = maxWork2;

}

void disp::begin(byte sA,byte sB, byte sC, byte sD, byte sE, byte sF, byte sG, byte sH)//Posuvny registr
{
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
    addText4Char((char)'i',(char)'n',(char)'i',(char)'t');
}

byte disp::toSegment(char znak)
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
        return segAbc[znak - 'A'] || RealHP;
    }
    switch (znak)
    {
    case ':':
        return D||G;
        break; 
    case '-':
        return G;
        break;
    case '_':
        return D;
        break;
    case '~':
        return G||DP;
        break;  
    case '[':
    case '{':
    case '(':
        return A||D||E||F;
        break;
    case ']':
    case '}':
    case ')':
            return A||B||C||D;
        break;
    case '!':
        return B||C||DP;
        break; 
    case '"':
        return B||F;
        break;  
    case '#':
        return A||B||G||F;
        break;  
    case '%':
        return B||G||E||DP;
        break; 
    case '&':
        return A||B||G||F||DP;
        break;
    case '*':
        return B||C||G||DP;
        break;
    case '+':
        return B||C||G;
        break;
    case ',':
        return C;
        break;
    case '.':
        return DP;
        break;
    case '/':
        return B||G||E;
        break;
    case ';':
        return C||DP;
        break;
    case '<':
        return A||G||F;
        break;
    case '=':
        return A||G;
        break;
    /*case '''://39
        return B;
        break;*/
    case '>':
        return A||B||G;
        break;
    case '?':
        return A||B||D||E||G||DP;
        break;
    case '@':
        return A||B||C||D||E||F||G||DP;
        break;
    /*case '\':
        return C||G||F;
        break;*/
    case '^':
        return A||B||F;
        break;
    /*case '''://96
        return A||B||G;
        break;*/
    case '|':
        return B||C;
        break;
    /*case '$':
        return 0;
        break;*/
    case ' ':
        return 0;
        break;
    default:
        break;
    }
    return 0;
}


byte disp::toReal(byte segm)
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


long disp::BitSet(byte index)
{
    return 1<<index;
}

byte disp::addText4(String text = "    ", long dobaTrvani = 0, long tecky =0,long neg = 0)
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
    long jedna = B1111<<((nasob-1)*4);
    int u;
    
    for (int i = 0; i < nasob; i++)
    {
        u = 4*i;
        addText4Char(text.charAt(u+0),text.charAt(u+1),text.charAt(u+2),text.charAt(u+3), doba,(tecky & jedna>>u)>>((nasob-1-i)*4),(neg & jedna>>u)>>((nasob-1-i)*4));
    }
    return nasob ;
}


byte disp::vystup()
{

    return work[vystupWork].vystup[work[vystupWork].vystupInd];
}

void disp::vystupEX()
{
    if (work[vystupWork].vystupInd == 3)
    {
        if ((maxWork != vystupWork)&& (work[vystupWork].nextTime > millis()))
        {
            byte vymaz  = vystupWork;
            vystupWork ++;
            if (vystupWork == mWork)
            {
               vystupWork =0;
            }    
            work[vystupWork].nextTime = work[vystupWork].timeTrvani + millis();
        }
        work[vystupWork].vystupInd = 0;
    }
    else
    {
        work[vystupWork].vystupInd ++;
    }
}


byte disp::index()
{
    return work[vystupWork].vystupInd;
}


/*
disp::disp()
{

}

disp::~disp()
{
*/
#endif