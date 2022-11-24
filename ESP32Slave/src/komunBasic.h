

//PLATFORMIO
#ifdef PLATFORMIO
#define charAtPNm charAt
#define concatPNm concat
#define StringPNm String
    #include <Arduino.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string>

//typedef uint8_t bool2;
typedef std::uint8_t byte;
typedef std::string String;
//tepedef std::string.at String.charAt;
#define charAtPNm at
#define concatPNm append
#define StringPNm std::to_string
#define B0 0
#define B00 0
#define B000 0
#define B0000 0
#define B00000 0
#define B000000 0
#define B0000000 0
#define B00000000 0
#define B1 1
#define B01 1
#define B001 1
#define B0001 1
#define B00001 1
#define B000001 1
#define B0000001 1
#define B00000001 1
#define B10 2
#define B010 2
#define B0010 2
#define B00010 2
#define B000010 2
#define B0000010 2
#define B00000010 2
#define B11 3
#define B011 3
#define B0011 3
#define B00011 3
#define B000011 3
#define B0000011 3
#define B00000011 3
#define B100 4
#define B0100 4
#define B00100 4
#define B000100 4
#define B0000100 4
#define B00000100 4
#define B101 5
#define B0101 5
#define B00101 5
#define B000101 5
#define B0000101 5
#define B00000101 5
#define B110 6
#define B0110 6
#define B00110 6
#define B000110 6
#define B0000110 6
#define B00000110 6
#define B111 7
#define B0111 7
#define B00111 7
#define B000111 7
#define B0000111 7
#define B00000111 7
#define B1000 8
#define B01000 8
#define B001000 8
#define B0001000 8
#define B00001000 8
#define B1001 9
#define B01001 9
#define B001001 9
#define B0001001 9
#define B00001001 9
#define B1010 10
#define B01010 10
#define B001010 10
#define B0001010 10
#define B00001010 10
#define B1011 11
#define B01011 11
#define B001011 11
#define B0001011 11
#define B00001011 11
#define B1100 12
#define B01100 12
#define B001100 12
#define B0001100 12
#define B00001100 12
#define B1101 13
#define B01101 13
#define B001101 13
#define B0001101 13
#define B00001101 13
#define B1110 14
#define B01110 14
#define B001110 14
#define B0001110 14
#define B00001110 14
#define B1111 15
#define B01111 15
#define B001111 15
#define B0001111 15
#define B00001111 15
#define B10000 16
#define B010000 16
#define B0010000 16
#define B00010000 16
#define B10001 17
#define B010001 17
#define B0010001 17
#define B00010001 17
#define B10010 18
#define B010010 18
#define B0010010 18
#define B00010010 18
#define B10011 19
#define B010011 19
#define B0010011 19
#define B00010011 19
#define B10100 20
#define B010100 20
#define B0010100 20
#define B00010100 20
#define B10101 21
#define B010101 21
#define B0010101 21
#define B00010101 21
#define B10110 22
#define B010110 22
#define B0010110 22
#define B00010110 22
#define B10111 23
#define B010111 23
#define B0010111 23
#define B00010111 23
#define B11000 24
#define B011000 24
#define B0011000 24
#define B00011000 24
#define B11001 25
#define B011001 25
#define B0011001 25
#define B00011001 25
#define B11010 26
#define B011010 26
#define B0011010 26
#define B00011010 26
#define B11011 27
#define B011011 27
#define B0011011 27
#define B00011011 27
#define B11100 28
#define B011100 28
#define B0011100 28
#define B00011100 28
#define B11101 29
#define B011101 29
#define B0011101 29
#define B00011101 29
#define B11110 30
#define B011110 30
#define B0011110 30
#define B00011110 30
#define B11111 31
#define B011111 31
#define B0011111 31
#define B00011111 31
#define B100000 32
#define B0100000 32
#define B00100000 32
#define B100001 33
#define B0100001 33
#define B00100001 33
#define B100010 34
#define B0100010 34
#define B00100010 34
#define B100011 35
#define B0100011 35
#define B00100011 35
#define B100100 36
#define B0100100 36
#define B00100100 36
#define B100101 37
#define B0100101 37
#define B00100101 37
#define B100110 38
#define B0100110 38
#define B00100110 38
#define B100111 39
#define B0100111 39
#define B00100111 39
#define B101000 40
#define B0101000 40
#define B00101000 40
#define B101001 41
#define B0101001 41
#define B00101001 41
#define B101010 42
#define B0101010 42
#define B00101010 42
#define B101011 43
#define B0101011 43
#define B00101011 43
#define B101100 44
#define B0101100 44
#define B00101100 44
#define B101101 45
#define B0101101 45
#define B00101101 45
#define B101110 46
#define B0101110 46
#define B00101110 46
#define B101111 47
#define B0101111 47
#define B00101111 47
#define B110000 48
#define B0110000 48
#define B00110000 48
#define B110001 49
#define B0110001 49
#define B00110001 49
#define B110010 50
#define B0110010 50
#define B00110010 50
#define B110011 51
#define B0110011 51
#define B00110011 51
#define B110100 52
#define B0110100 52
#define B00110100 52
#define B110101 53
#define B0110101 53
#define B00110101 53
#define B110110 54
#define B0110110 54
#define B00110110 54
#define B110111 55
#define B0110111 55
#define B00110111 55
#define B111000 56
#define B0111000 56
#define B00111000 56
#define B111001 57
#define B0111001 57
#define B00111001 57
#define B111010 58
#define B0111010 58
#define B00111010 58
#define B111011 59
#define B0111011 59
#define B00111011 59
#define B111100 60
#define B0111100 60
#define B00111100 60
#define B111101 61
#define B0111101 61
#define B00111101 61
#define B111110 62
#define B0111110 62
#define B00111110 62
#define B111111 63
#define B0111111 63
#define B00111111 63
#define B1000000 64
#define B01000000 64
#define B1000001 65
#define B01000001 65
#define B1000010 66
#define B01000010 66
#define B1000011 67
#define B01000011 67
#define B1000100 68
#define B01000100 68
#define B1000101 69
#define B01000101 69
#define B1000110 70
#define B01000110 70
#define B1000111 71
#define B01000111 71
#define B1001000 72
#define B01001000 72
#define B1001001 73
#define B01001001 73
#define B1001010 74
#define B01001010 74
#define B1001011 75
#define B01001011 75
#define B1001100 76
#define B01001100 76
#define B1001101 77
#define B01001101 77
#define B1001110 78
#define B01001110 78
#define B1001111 79
#define B01001111 79
#define B1010000 80
#define B01010000 80
#define B1010001 81
#define B01010001 81
#define B1010010 82
#define B01010010 82
#define B1010011 83
#define B01010011 83
#define B1010100 84
#define B01010100 84
#define B1010101 85
#define B01010101 85
#define B1010110 86
#define B01010110 86
#define B1010111 87
#define B01010111 87
#define B1011000 88
#define B01011000 88
#define B1011001 89
#define B01011001 89
#define B1011010 90
#define B01011010 90
#define B1011011 91
#define B01011011 91
#define B1011100 92
#define B01011100 92
#define B1011101 93
#define B01011101 93
#define B1011110 94
#define B01011110 94
#define B1011111 95
#define B01011111 95
#define B1100000 96
#define B01100000 96
#define B1100001 97
#define B01100001 97
#define B1100010 98
#define B01100010 98
#define B1100011 99
#define B01100011 99
#define B1100100 100
#define B01100100 100
#define B1100101 101
#define B01100101 101
#define B1100110 102
#define B01100110 102
#define B1100111 103
#define B01100111 103
#define B1101000 104
#define B01101000 104
#define B1101001 105
#define B01101001 105
#define B1101010 106
#define B01101010 106
#define B1101011 107
#define B01101011 107
#define B1101100 108
#define B01101100 108
#define B1101101 109
#define B01101101 109
#define B1101110 110
#define B01101110 110
#define B1101111 111
#define B01101111 111
#define B1110000 112
#define B01110000 112
#define B1110001 113
#define B01110001 113
#define B1110010 114
#define B01110010 114
#define B1110011 115
#define B01110011 115
#define B1110100 116
#define B01110100 116
#define B1110101 117
#define B01110101 117
#define B1110110 118
#define B01110110 118
#define B1110111 119
#define B01110111 119
#define B1111000 120
#define B01111000 120
#define B1111001 121
#define B01111001 121
#define B1111010 122
#define B01111010 122
#define B1111011 123
#define B01111011 123
#define B1111100 124
#define B01111100 124
#define B1111101 125
#define B01111101 125
#define B1111110 126
#define B01111110 126
#define B1111111 127
#define B01111111 127
#define B10000000 128
#define B10000001 129
#define B10000010 130
#define B10000011 131
#define B10000100 132
#define B10000101 133
#define B10000110 134
#define B10000111 135
#define B10001000 136
#define B10001001 137
#define B10001010 138
#define B10001011 139
#define B10001100 140
#define B10001101 141
#define B10001110 142
#define B10001111 143
#define B10010000 144
#define B10010001 145
#define B10010010 146
#define B10010011 147
#define B10010100 148
#define B10010101 149
#define B10010110 150
#define B10010111 151
#define B10011000 152
#define B10011001 153
#define B10011010 154
#define B10011011 155
#define B10011100 156
#define B10011101 157
#define B10011110 158
#define B10011111 159
#define B10100000 160
#define B10100001 161
#define B10100010 162
#define B10100011 163
#define B10100100 164
#define B10100101 165
#define B10100110 166
#define B10100111 167
#define B10101000 168
#define B10101001 169
#define B10101010 170
#define B10101011 171
#define B10101100 172
#define B10101101 173
#define B10101110 174
#define B10101111 175
#define B10110000 176
#define B10110001 177
#define B10110010 178
#define B10110011 179
#define B10110100 180
#define B10110101 181
#define B10110110 182
#define B10110111 183
#define B10111000 184
#define B10111001 185
#define B10111010 186
#define B10111011 187
#define B10111100 188
#define B10111101 189
#define B10111110 190
#define B10111111 191
#define B11000000 192
#define B11000001 193
#define B11000010 194
#define B11000011 195
#define B11000100 196
#define B11000101 197
#define B11000110 198
#define B11000111 199
#define B11001000 200
#define B11001001 201
#define B11001010 202
#define B11001011 203
#define B11001100 204
#define B11001101 205
#define B11001110 206
#define B11001111 207
#define B11010000 208
#define B11010001 209
#define B11010010 210
#define B11010011 211
#define B11010100 212
#define B11010101 213
#define B11010110 214
#define B11010111 215
#define B11011000 216
#define B11011001 217
#define B11011010 218
#define B11011011 219
#define B11011100 220
#define B11011101 221
#define B11011110 222
#define B11011111 223
#define B11100000 224
#define B11100001 225
#define B11100010 226
#define B11100011 227
#define B11100100 228
#define B11100101 229
#define B11100110 230
#define B11100111 231
#define B11101000 232
#define B11101001 233
#define B11101010 234
#define B11101011 235
#define B11101100 236
#define B11101101 237
#define B11101110 238
#define B11101111 239
#define B11110000 240
#define B11110001 241
#define B11110010 242
#define B11110011 243
#define B11110100 244
#define B11110101 245
#define B11110110 246
#define B11110111 247
#define B11111000 248
#define B11111001 249
#define B11111010 250
#define B11111011 251
#define B11111100 252
#define B11111101 253
#define B11111110 254
#define B11111111 255
#endif




#define Amask                       B11000000
#define Amotor                      B00000000

#define Atext                       B01000000
#define AwhatMotor                  B10000000
#define Aposun  6
#define Bmask B00100000
#define BsMax_smerVpred_tMess        B00000000
#define BsMin_smerVzad_tSet          B00100000
#define Bposun  5
#define Cmask  B00010000
#define Cano_brzda_mChat_sRead       B00010000
#define Cne_brzda_mInfo_sWrite         B00000000
#define Cposun  4
#define INDEXmask B00001111

#define textNum 1
#define motorNum 0
#define text_setNum 1
#define text_messNum 0
#define text_set_ReadNum 1
#define text_set_WriteNum 0
#define text_mess_infoNum 0
#define text_mess_chatNum 1
#define motorNum 0
#define motor_Vpred 0
#define motor_Vzad 1
#define motor_i_Brzda 1
#define whatMotNum 2


#define mVpred 1
#define mVzad -1
#define mStopLow 0
#define mStopHigh 2
#define mStop mStopLow
#define mBrzda 3



#ifndef kn_komBasic
#define kn_komBasic
/*
1 => na pozice je 1
0 => na pozici je 0
x => na stavu pozice nezalezi, def jsou po ...&0
A => pozice je po ...&1


Hlavicka:
text                01xxxxxx
-mess               xx0xxxxx
--info              xxx0xxxx
--chat              xxx1xxxx
-set                xx1xxxxx
--Read              xxx1xxxx
--Write             xxx0xxxx
motor               00xxxxxx
-smerVpred          xx0xxxxx
-smerVzad           xx1xxxxx
--BrzdaZap          xxx1xxxx
--BrzdaVyp          xxx0xxxx
whatMot             10xxxxxx

Index - Urcuje motor
Index               xxxxAAAA

Value - hodnota vykonu motora. Jde o druhy bait.
Data                xxxxxxxx AAAAAAAA
*/

/*
text
-V Data je umistěný velikost následujicího textového řetězce
    text_mess
    -proměná Index nemá význam
        text_mess_info
        -chybové hlášky ESP
        -Připojení/odpojení zařizeni
        text_mess_chat
        -komunikace mezi klienty
    text_set
    -Učel je zde přijat text, který bude moct být pomoci kličů rozdělen do tabulky, která bude spravovat nastavení stránky
    -Index zde má funkci volitele druhu nastaveni
        text_set_Read
        -Zádost o zaslani nastavení.
        -prom Data nemá význam
        text_set_Write
        -Přepsani nastavení.
motor
-V Data je umýstěn výkon potoru, nikoliv informace o směru
    motor_Vpred/Vzdad
    -informace o jizde v před
        motor_x_BrzdaAno/Ne
        -Půjde o automatický zpětný tah motoru.
        -Na straně klienta možnost spustit kod v ESP32
        -Klient,který funkci aktivuje zaroveň automaticky zastavý motor po určité době.
        -Ostatní clienti po dobu aktivace nemohou znova zapnout, ale mohou ji kdykoliv přerušit
        -ESP32 bude mýt funkci schopnou po dlouhé době zastavit brzdu.
whatMot
-dotaz na hodnotu pro daný motor
-Odpověď formatem motor.
*/

struct dataFace
{
  byte AtypeNum;
  bool BtypeBool;
    bool CtypeBool;
    byte Index;
    byte Data; 
    String text;
     int16_t len;
};


struct output
{
    byte row[2];
    byte Atype;
    byte Btype;
    byte Ctype;
dataFace data;
};


struct dataSend
{
  uint8_t *data;
  size_t leng;
};
/*
struct dataMotor
{
    byte rych;
    byte index;
    char smer;
    bool brzda;
};
*/

dataSend ADDdataRow(dataSend a, dataSend b)
{
  dataSend c;
  c.leng = a.leng + b.leng;
  c.data = (byte *)malloc(c.leng);
  for ( int16_t i = 0; i < a.leng; i++)
  {
    c.data[i] = a.data[i];
  }
  for ( int16_t i = 0; i < b.leng; i++)
  {
    c.data[i+a.leng] = b.data[i];
  }
  free(a.data);
  free(b.data);
  return c;
}

dataSend dataRow(dataFace in)
{
  dataSend a;
  a.data = (byte*)malloc(in.len);
  a.data[0] = in.AtypeNum<<Aposun | in.BtypeBool <<Bposun |in.CtypeBool<<Cposun | in.Index;
  a.data[1] = in.Data;
  if ((a.data[0] & Amask) == Atext)
  {
    for ( int16_t i = 0; i < in.text.length(); i++)
    {
      a.data[i+2] = in.text.charAtPNm(i);
    }
  }
  return a;
}
/*
dataSend motorSendDataMotor(dataMotor m)
{

    dataSend u;
    if ( m.index >= 8)
    {
        u.leng = 0;
        return u;
    }
    
    u.leng = 2;
    u.data =(byte*) malloc(u.leng);
    u.data[0] = Amotor | (INDEXmask & m.index) ;

    u.data[1] = m.rych;

    if (m.smer == mVpred)
    {
        u.data[0] = u.data[0] | BsMax_smerVpred_tMess;
    }
    else if(m.smer == mVzad)
    {
        u.data[0] = u.data[0] | BsMin_smerVzad_tSet;
    }
    else
    {
        u.data[0] = u.data[0] | BsMax_smerVpred_tMess;
        u.data[1] = 0;
    }

    if (m.brzda == true)
    {
        u.data[0] = u.data[0] | Cano_brzda_mChat_sRead;
    }
    else
    {
        u.data[0] = u.data[0] | Cne_brzda_mInfo_sWrite;
    }
    return u;
}*/

dataSend motorSend(byte index,  int16_t rychlost, byte brzda)
{
    dataSend u;
    u.leng = 2;
    u.data =(byte*) malloc(u.leng);
    u.data[0] = Amotor | (INDEXmask & index) ;
    if (rychlost >= 0)
    {
        byte uu = rychlost;
        u.data[1] = uu;
        u.data[0] = u.data[0] | BsMax_smerVpred_tMess;
    }
    else
    {
        rychlost = rychlost * (-1);
        byte uu = rychlost;
        u.data[1] = uu;
        u.data[0] = u.data[0] | BsMin_smerVzad_tSet;
    }

    if (brzda == true)
    {
        u.data[0] = u.data[0] | Cano_brzda_mChat_sRead;
    }
    else
    {
        u.data[0] = u.data[0] | Cne_brzda_mInfo_sWrite;
    }
    return u;
}

/*
dataSend motorMulti4SendDataMotor(dataMotor *m0= 0,dataMotor *m1= 0, dataMotor *m2 =0, dataMotor *m3 =0)
{
    dataSend d;
    d.leng = 0;
    if(m0 != 0)
    {
        d = ADDdataRow(d, motorSendDataMotor(*m0));
    }
    if(m1 != 0)
    {
        d = ADDdataRow(d, motorSendDataMotor(*m1));
    }
    if(m2 != 0)
    {
        d = ADDdataRow(d, motorSendDataMotor(*m2));
    }
    if(m3 != 0)
    {
        d = ADDdataRow(d, motorSendDataMotor(*m3));
    }
    return d;
}
*/


dataSend motorMulti4Send( int16_t index0,  int16_t rychlost0, byte brzda0, int16_t index1,  int16_t rychlost1, byte brzda1, int16_t index2,  int16_t rychlost2, byte brzda2, int16_t index3,  int16_t rychlost3, byte brzda3)
{
    dataSend d;
    d.leng = 0;
    if(index0 != -1)
    {
        d = ADDdataRow(d, motorSend(index0,rychlost0,brzda0));
    }
    if(index1 != -1)
    {
        d = ADDdataRow(d, motorSend(index1,rychlost1,brzda1));
    }
    if(index2 != -1)
    {
        d = ADDdataRow(d, motorSend(index2,rychlost2,brzda2));
    }
    if(index3 != -1)
    {
        d = ADDdataRow(d, motorSend(index3,rychlost3,brzda3));
    }
    return d;
}


dataSend motorMulti8Send( int16_t index0,  int16_t rychlost0, byte brzda0, int16_t index1,  int16_t rychlost1, byte brzda1, int16_t index2,  int16_t rychlost2, byte brzda2, int16_t index3,  int16_t rychlost3, byte brzda3, int16_t index4,  int16_t rychlost4, byte brzda4, int16_t index5,  int16_t rychlost5, byte brzda5, int16_t index6,  int16_t rychlost6, byte brzda6, int16_t index7,  int16_t rychlost7, byte brzda7)
{
    return ADDdataRow(motorMulti4Send(index0,rychlost0,brzda0,index1,rychlost1,brzda1,index2,rychlost2,brzda2,index3,rychlost3,brzda3), motorMulti4Send(index4,rychlost4,brzda4,index5,rychlost5,brzda5,index6,rychlost6,brzda6,index7,rychlost7,brzda7));
}
/*
dataSend motorMulti8SendDataMotor( dataMotor *m0= 0,dataMotor *m1= 0, dataMotor *m2 =0, dataMotor *m3 =0,dataMotor *m4= 0,dataMotor *m5= 0, dataMotor *m6 =0, dataMotor *m7 =0)
{
    return ADDdataRow(motorMulti4SendDataMotor(m0,m1,m2,m3), motorMulti4SendDataMotor(m4,m5,m6,m7));
}

dataMotor motorOut(dataFace o)
{
    dataMotor a;
    a.index = o.Index;
    a.brzda = o.CtypeBool;
    a.smer = o.BtypeBool;
    a.rych = o.Data;
    return a;
}*/

bool isMotorInDataFace(dataFace o)
{
    if (o.AtypeNum == motorNum)
    {
        return true;
    }
    return false;
    
}


dataSend wtahMotorSend(byte index)
{
    dataSend u;
    u.leng = 2;
    u.data =(byte*) malloc(u.leng);
    u.data[0] = AwhatMotor | (INDEXmask & index) ; 
    u.data[1] = 0;  
    return u; 
}


dataSend textSend(byte prvniByte,String t)
{
    dataSend u;
    u.leng = 2 + t.length();
    u.data =(byte*) malloc(u.leng);
    u.data[0] = prvniByte ; 
    u.data[1] = t.length(); 
    for ( int16_t i = 0; i < t.length(); i++)
    {
        u.data[i + 2] = t.charAtPNm(i);
    }
    return u;
}


dataSend textMessInfoSend(String t)
{
    return textSend(Atext | BsMax_smerVpred_tMess | Cne_brzda_mInfo_sWrite,t);
}


dataSend textMessChatSend(String t)
{
    return textSend(Atext | BsMax_smerVpred_tMess | Cano_brzda_mChat_sRead,t);
}


dataSend textSetReadSend(byte index, String t)
{
    return textSend(Atext | BsMin_smerVzad_tSet | Cano_brzda_mChat_sRead | (index & INDEXmask),t);
}


dataSend textSetWriteSend(byte index, String t)
{
    return textSend(Atext | BsMin_smerVzad_tSet | Cne_brzda_mInfo_sWrite | (index & INDEXmask),t);
}


byte dataOut(dataFace a)
{
    return a.Data;
}


byte IndexOut(dataFace a)
{
    return a.Index;
}


byte AOut(dataFace a)
{
    return a.AtypeNum;
}


bool BOut(dataFace a)
{
    return a.BtypeBool;
}


bool COut(dataFace a)
{
    return a.CtypeBool;
}


bool isTextStringOut(dataFace a)
{
    if (a.AtypeNum == textNum)
    {
        return true;
    }
    return false;
}


String textStringOut(dataFace a)
{
    String uu;
    for ( int16_t i = 0; i < a.Data; i++)
    {
        uu.concatPNm(StringPNm(a.text[i]));
    }
    return uu;
}


dataSend makeOutput(uint8_t *data, size_t len, dataSend work(dataFace ))
{
     int16_t i = 0;
    dataSend zaloha;
    zaloha.leng = 0;
    do
    {
        output a;
        a.row[0] = data[i];
        a.row[1] = data[i+1];
        a.Atype = a.row[0] & Amask;
        a.data.AtypeNum = a.Atype >> Aposun;
        a.Btype = a.row[0] & Bmask;
        byte p = a.Btype >> Bposun;
        a.data.BtypeBool = p;       
        a.Ctype = a.row[0] & Cmask;
        p = a.Ctype >> Cposun;
        a.data.CtypeBool = p;
        a.data.Index = a.row[0] & INDEXmask;
        a.data.Data = a.row[1];
        i += 2;
        a.data.len = 2;
        if (a.Atype == Atext)
        {
            if ((len-i)<=a.data.Data)
            {
                for ( int16_t u = 0; u < a.data.Data; u++)
                {
                a.data.text += data[i];
                i++;
                a.data.len ++;
                }
                
            }
            else 
            {
                //problem
            }

        }
        zaloha = ADDdataRow(zaloha,work(a.data));
    } while (i<len);
    return zaloha;
}

#endif