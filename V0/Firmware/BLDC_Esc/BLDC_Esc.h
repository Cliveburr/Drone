#ifndef BLDC_ESC_H
#define	BLDC_ESC_H

#include "Timer_Events.h"

union UInt16ConvertionUnion {
   unsigned int value;
   unsigned char bytes[2];
} UInt16Convertion;

union ULong32ConvertionUnion {
   unsigned long value;
   unsigned char bytes[4];
} ULong32Convertion;

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
    //unsigned char adcValues[64];
    enum ChannelMode mode;
    enum ChannelState state;
    unsigned char isCrossZero;
    unsigned char crossZeroCount;
    struct TimerEventCounter stepLengthCounter;
    struct TimerEventStruct automaticStartStopTimer;
};

unsigned char CrossZeroDef;

unsigned long AutomaticStartStopValue;
unsigned char AutomaticStartStopInc;

struct ChannelStruct BLDC_Esc_Channels[4];

void BLDC_Esc_Initialize();

void BLDC_Esc_Task();

void BLDC_Esc_Tick(struct ChannelStruct *channel);

void CrossZeroEvent(struct ChannelStruct *channel);

void BLDC_Esc_SetManual(unsigned char index);

void BLDC_Esc_SetManualOn(unsigned char index);

void BLDC_Esc_SetManualOff(unsigned char index);

void BLDC_Esc_SetManualConfig(unsigned char index, unsigned char direction, unsigned char oneStep);

void BLDC_Esc_SetManualStep(unsigned char index, unsigned long stepTicks);

void BLDC_Esc_SetManualPWM(unsigned char index, unsigned int pwmOnBeforeAdc, unsigned int pwmOnAfterAdc, unsigned int pwmOff);

void BLDC_Esc_SetAutomatic(unsigned char index);

void BLDC_Esc_SetAutomaticOn(unsigned char index);

void BLDC_Esc_SetAutomaticOff(unsigned char index);

void BLDC_Esc_SetAutomaticStartStop(unsigned char tag);

#endif	/* BLDC_ESC */