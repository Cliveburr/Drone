#ifndef BLDC_ESC_H
#define	BLDC_ESC_H

#include "BLDC_Esc_User.h"

unsigned short TimerLastValue = 0;
unsigned short TimerActualValue = 0;
unsigned short TimerDiffValue = 0;

typedef void (*TimerEventCallback)();

struct TimerEventStruct {
    unsigned short Value;
    unsigned short Missing;
    TimerEventCallback Callback;
};

void TimerEvent_Tick();

void TimerEvent_Check(struct TimerEventStruct *timer);

#endif	/* BLDC_ESC */