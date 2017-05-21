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

struct TimerEventStruct test1;

unsigned long Channel0PWM_Width = 1200;
unsigned int Channel0PWN_On = 600;
unsigned int Channel0PWN_Off = 600;

unsigned char foward = 1;


#endif	/* BLDC_ESC */