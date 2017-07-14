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
    CS_AutomaticOff = 2,
    CS_AutomaticOn = 3
};

enum ChannelStepState {
    CSS_PosCommute = 0,
    CSS_Stable = 1,
    CSS_PreCommute = 2
};

enum ChannelAutomaticState {
    CAS_Starting = 0,
    CAS_Running = 1,
    CAS_Stopping = 2
};

enum ChannelPWMState {
    CPWMS_Off = 0,
    CPWMS_BeforeAdc = 1,
    CPWMS_OnAdc = 2
};

struct ChannelStruct {
    unsigned char step;
    enum ChannelStepState stepState;
    enum ChannelAutomaticState automaticState;
    unsigned long stepLength;
    unsigned char isFoward;
    unsigned char isOneStep;
    struct TimerEventRotine stepTimer;
    enum ChannelPWMState pwmState;
    unsigned int pwmOnBeforeAdc;
    unsigned int pwmOnAdc;
    unsigned int pwmOff;
    struct TimerEventRotine pwmTimer;
    //unsigned char adcValues[64];
    enum ChannelMode mode;
    enum ChannelState state;
    unsigned char isCrossZero;
    unsigned char crossZeroCount;
    struct TimerEventCounter stepLengthCounter;
    struct TimerEventRotine altSpeedTimer;
    unsigned int altSpeedCount;
    float altSpeedValue;
    unsigned int stepCounting;
    unsigned int stepCountingVar;
};

unsigned char CrossZeroDef;

unsigned long AutomaticStartStepLength;
unsigned int AutomaticStartStepTargetLength;
unsigned char AutomaticStartStopInc;

struct ChannelStruct BLDC_Esc_Channels[4];

struct TimerEventRotine StepCountingTimer;

void BLDC_Esc_Initialize();

void BLDC_Esc_Task();

void BLDC_Esc_Tick(unsigned char index);

void BLDC_Esc_CrossZeroEvent(struct ChannelStruct *channel);

void BLDC_Esc_SetManual(unsigned char index);

void BLDC_Esc_SetManualOn(unsigned char index);

void BLDC_Esc_SetManualOff(unsigned char index);

void BLDC_Esc_SetManualConfig(unsigned char index, unsigned char direction, unsigned char oneStep);

void BLDC_Esc_SetManualStep(unsigned char index, unsigned long stepTicks);

void BLDC_Esc_SetPWM(unsigned char index, unsigned int pwmOnBeforeAdc, unsigned int pwmOnAfterAdc, unsigned int pwmOff);

void BLDC_Esc_SetAutomatic(unsigned char index);

void BLDC_Esc_SetAutomaticOn(unsigned char index);

void BLDC_Esc_SetAutomaticOff(unsigned char index);

void BLDC_Esc_AltSpeed(unsigned char tag);

void BLDC_Esc_ConfigStartStopCurve(unsigned long beginValue, unsigned int endValue, unsigned char incValue, unsigned int clockValue);

void BLDC_Esc_SetAltSpeed(unsigned char index, unsigned int speedCount, float speedValue);

void BLDC_Esc_StepCounting();

#endif	/* BLDC_ESC */