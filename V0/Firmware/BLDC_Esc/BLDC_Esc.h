#ifndef BLDC_ESC_H
#define	BLDC_ESC_H

#include "BLDC_Esc_User.h"
#include "Timer_Events.h"

union UInt32Convertion {
   unsigned int value;
   unsigned char bytes[4];
};

enum ChannelMode {
    CM_Manual = 0,
    CM_Automatic = 1
};

enum ChannelState {
    CS_ManualOff = 0,
    CS_ManualOn = 1,
    CS_Automatic_Off = 2,
    CS_AutomaticStarting = 3,
    CS_AutomaticRunning = 4,
    CS_AutomaticStoping = 5
};

struct ChannelStruct {
    unsigned char step;
    unsigned char isFoward;
    unsigned char isOneStep;
    struct TimerEventStruct stepTimer;
    unsigned char pwmState;
    unsigned int pwmOnBeforeAdc;
    unsigned int pwmOnAfterAdc;
    unsigned int pwmOff;
    struct TimerEventStruct pwmTimer;
    unsigned char adcValues[64];
    enum ChannelMode mode;
    enum ChannelState state;
    unsigned char isCrossZero;
    unsigned char crossZeroCount;
    struct TimerEventCounter stepLengthCounter;
};

unsigned char CrossZeroDef;
unsigned int AutomaticStartValue;
unsigned char AutomaticStartInc;

struct ChannelStruct Channel0;

void BLDC_Esc_Initialize();

void BLDC_Esc_Task();

void BLDC_Esc_Tick(struct ChannelStruct *channel);

#endif	/* BLDC_ESC */