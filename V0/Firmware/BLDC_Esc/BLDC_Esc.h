#ifndef BLDC_ESC_H
#define	BLDC_ESC_H

#include "BLDC_Esc_User.h"

unsigned short TimerLastValue = 0;
unsigned short TimerActualValue = 0;
unsigned short TimerDiffValue = 0;

typedef void (*TimerEventCallback)();

struct TimerEventStruct {
    unsigned long Value;
    unsigned long Missing;
    TimerEventCallback Callback;
};

void TimerEvent_Tick();

void TimerEvent_Check(struct TimerEventStruct *timer);


union ULongConvertion {
   unsigned long value;
   unsigned char bytes[4];
};

enum ChannelMode {
    CM_Manual = 0,
    CM_Automatic = 1,
    CM_StartingAutomatic = 2,
    CM_RunningAutomatic = 3,
    CM_StopingAutomatic = 4
};

struct ChannelStruct {
    unsigned char step;
    unsigned char isFoward;
    unsigned char isRunning;
    unsigned char isOneStep;
    struct TimerEventStruct stepTimer;
    unsigned char pwmState;
    unsigned int pwmOnBeforeAdc;
    unsigned int pwmOnAfterAdc;
    unsigned int pwmOff;
    struct TimerEventStruct pwmTimer;
    unsigned char adcValues[64];
    enum ChannelMode mode;
};

struct ChannelStruct Channel0;

#endif	/* BLDC_ESC */