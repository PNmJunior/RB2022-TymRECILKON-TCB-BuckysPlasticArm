#ifndef kn_motor
#define kn_motor

#include <Arduino.h>
#include <binary.h>
#include <Arduino_JSON.h>
#include <vector>
#include "esp32-hal.h"
#include "soc/soc_caps.h"
#include "driver/ledc.h"
//#include <komunBasic.h>
typedef byte resDuty;



#define LEDC_mTIMER              LEDC_TIMER_3
#define LEDC_mFrec              5000

#define pohonMaxHod 255

struct pohonVol
{
    resDuty dutOld;
    resDuty duty;
    ledc_mode_t speed_mode;
    ledc_channel_t channel;
    uint32_t hpoint;
};


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


#define mVpred 1
#define mVzad -1
#define mStopLow 0
#define mStopHigh 2
#define mStop mStopLow
#define mBrzda 3
#define rychModLow 0
#define rychModHigh 1

class motor
{
private:
    volatile byte vyst = 0;
    pohonSet m[8];
    volatile pohonVol v[8];
    byte set = 0;
    ledc_timer_config_t defTim;
    bool setDefTimer = 0;


    bool inputGo(byte mot, char smer ,int spead ,int maxSpead ,int minSpead);
    
public:
    byte vystup();
    void beginStart();
    bool beginTimer(uint32_t frek ,ledc_timer_t _tim  ,ledc_mode_t _speadmode );
    bool begin(byte mot, int _pin, ledc_channel_t channel,bool _inverz,bool _neg, byte _max, byte _min, ledc_timer_config_t *ledc_timer );
    bool beginEnd();
    void updatePWM(int i);
    void updatePWM();
    byte numToPosun(byte i);
    byte posunToNum(byte i);
    
    
    bool inputProc(byte mot, char proc);
    bool input(byte mot, char smer ,int spead,int maxSpead ,int minSpead , int cas );
    long TStop(byte mot);
    //motor();
    //~motor();
};

byte motor::vystup()
{
    return vyst;
}


byte motor::numToPosun(byte i)
{
    return B10000000 >> i;
}


byte motor::posunToNum(byte i)
{
    switch (i)
    {
    case B10000000:
        return 0;
        break;
    case B1000000:
        return 1;
        break;
    case B100000:
        return 2;
        break;
    case B10000:
        return 3;
        break;
    case B1000:
        return 4;
        break;
    case B100:
        return 5;
        break;
    case B10:
        return 6;
        break;
    case B1:
        return 7;
        break;
            
    default:
        break;
    }
    return 0;
}

long motor::TStop(byte mot)
{
    return m[mot].timeStop;
}


/*
//https://github.com/espressif/esp-idf/tree/4b6d9c8ad3/examples/peripherals/ledc/ledc_basic
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (5) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4095) // Set duty to 50%. ((2 ** 13) - 1) * 50% = 4095
#define LEDC_FREQUENCY          (5000) // Frequency in Hertz. Set frequency at 5 kHz

static void example_ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}
*/


void motor::beginStart()
{
    vyst = 0;
    set = 0;
    setDefTimer = false;
}


//LEDC_LOW_SPEED_MODE
bool motor::beginTimer(uint32_t frek = LEDC_mFrec,ledc_timer_t _tim =LEDC_mTIMER ,ledc_mode_t _speadmode = LEDC_LOW_SPEED_MODE)
{
        if (_speadmode != LEDC_LOW_SPEED_MODE || _speadmode != LEDC_HIGH_SPEED_MODE)
    {
        return true;
    }
        ledc_timer_config_t ledc_timer = {
        .speed_mode       = _speadmode,
        .duty_resolution  = LEDC_TIMER_8_BIT,
        .timer_num        = _tim,
        .freq_hz          = frek,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    
    esp_err_t a = ledc_timer_config(&ledc_timer);
    if (a != ESP_OK)
    {
        
return false;
    }
    defTim = ledc_timer;
        setDefTimer = true;
    return true;
}


bool motor::begin(byte mot, int _pin, ledc_channel_t channel,bool _inverz,bool _neg, byte _max, byte _min, ledc_timer_config_t *ledc_timer = null )
{
    if (mot>=8)
    {
        return false;
    }
    if (_pin>=GPIO_NUM_MAX)
    {
        return false;
    }
    if (channel >= LEDC_CHANNEL_MAX)
    {
        return false;
    }
    
    
    if (ledc_timer == null)
    {
        if (setDefTimer)
        {
            ledc_timer = &defTim;
        }
        else
        {
            return false;
        }
        
    }


    if (_max <= _min)
    {
        return false;
    }
    
    for (int i = 0; i < 8; i++)
    {
        if (set &numToPosun(i) != 0)
        {
            if (m[i].ledc_channel.gpio_num == _pin || m[i].ledc_channel.channel == channel)
            {
                return false;
            }
            
        }
        
    }
    pinMode(_pin, OUTPUT);
    
    
    m[mot].ledc_timer = ledc_timer;
    if(_inverz){
        m[mot].inverz = 1;
    }
    else
    {
        m[mot].inverz = -1;
    }
    if (_neg)
    {
        m[mot].neg = 1;
    }
    else
    {
        m[mot].neg = -1;
    }
    m[mot].max = _max;
    m[mot].min = _min;
    v[mot].duty = 0;
    v[mot].dutOld = v[mot].duty;
    m[mot].index = numToPosun(mot);
    m[mot].smer = mStop;
    m[mot].timeStop = 0;

        ledc_channel_config_t ledc_channel = {
        .gpio_num       = _pin,
        .speed_mode     = ledc_timer->speed_mode,
        .channel        = channel,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = ledc_timer->timer_num,
        .duty           = v[mot].duty,
        .hpoint         = 0
    };
    esp_err_t a = ledc_channel_config(&ledc_channel);
    if(a != ESP_OK)
    {
        return false;
    }

    m[mot].ledc_channel =ledc_channel;
    v[mot].speed_mode = m[mot].ledc_channel.speed_mode;
    v[mot].channel = m[mot].ledc_channel.channel;
    v[mot].hpoint = m[mot].ledc_channel.hpoint;
    set |= m[mot].index;
    return true;
}

bool motor::beginEnd()
{
    if (set != B11111111)
    {
        return false;
    }
    return true;
    
}


bool motor::input(byte mot, char smer = mStop,int spead = 0,int maxSpead = 255,int minSpead  =0, int cas = 0)
{
    
    if (mot>=8)
    {
        return false;
    }
    m[mot].timeStop = 0;
    if (smer==mBrzda)
    {
        if (vyst & m[mot].index !=0)
        {
            vyst &= ~m[mot].index;
        }
        else 
        {
            vyst |= m[mot].index;       
        }
        v[mot].duty = map(spead,minSpead,maxSpead,m[mot].min, m[mot].max); 
        m[mot].timeStop = millis() + cas;
    }
    else if (smer==mStopHigh)
    {
        vyst |= m[mot].index;
        v[mot].duty = pohonMaxHod;
    }
    else if (smer == mStopLow)
    {
        vyst &= ~m[mot].index;
        v[mot].duty = 0;
    }
    else
    {
        resDuty a = map(spead,minSpead,maxSpead,m[mot].min, m[mot].max);
        int b = smer * m[mot].neg * m[mot].inverz;
        if (b == mVpred)
        {
            v[mot].duty = a;
            vyst &= ~m[mot].index;
        }
        else if (b == mVzad)
        {
            vyst |= m[mot].index;
            v[mot].duty = ~a;
        }
        else
        {
            return false;
        }
    }
    
    return true;
}



bool motor::inputProc(byte mot, char proc)
{
    if (proc == 0)
    {
        return input(mot,mStop);
    }
    else if (proc > 0 && proc <=100)
    {
        return input(mot,mVpred,proc,100,0);
    }
    else if(proc < 0 && proc >= -100 )
    {
        return input(mot,mVzad,proc * -1,100,0);
    }
    else
    {
        return false;
    }
    return false;
    
}


void motor::updatePWM(int i)
{
    if (v[i].duty != v[i].dutOld)
    {
        ledc_set_duty_and_update(v[i].speed_mode,v[i].channel,v[i].duty, v[i].hpoint);
        v[i].dutOld = v[i].duty;
    }
}

void motor::updatePWM()
{
    for (int i = 0; i < 8; i++)
    {
        updatePWM(i);
    }
}

/*
motor::motor()
{
    
}
*/
/*
motor::~motor()
{
}
*/

#endif 