#ifndef _spolecneStruktury_
#define _spolecneStruktury_

#include <Arduino.h>
#include "esp32-hal.h"
#include "soc/soc_caps.h"
#include "driver/ledc.h"
#include "enkoder.h"
#include "disp.h"
#include "writeTextSegDisp.h"

writeTextSegDisp editText;

struct textik
{
    long nextTime;
    long timeTrvani;
    char textNow[4];
    byte Aline[4];
    byte vystup[4] = {0,0,0,0};
};


//Motor.h
typedef byte resDuty;


//Motor.h
struct pohonVol
{
    resDuty dutOld;
    resDuty duty;
    ledc_mode_t speed_mode;
    ledc_channel_t channel;
    uint32_t hpoint;
};

//Motor.h
struct pohonSet
{
    char neg;//oto4ena svorkovnice
    char inverz;//ote4eni motoru
    resDuty max;
    resDuty min;
    byte index;
    char smer;
    long timeStop;
   ledc_timer_config_t *ledc_timer;
   ledc_channel_config_t ledc_channel;
};

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


#endif