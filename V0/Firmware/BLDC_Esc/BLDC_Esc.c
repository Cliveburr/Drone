#include <stdint.h>
#include <xc.h>
#include <htc.h>

#include "BLDC_Esc.h"
#include "BLDC_Esc_WorkHighLow.h"

void BLDC_Esc_Initialize() {
    //test1.Value = 46875;
    //test1.Missing = test1.Value = 6000004; //144000;
    //test1.Callback = test1_cb;
    
    //Channel0PWM.Missing = Channel0PWM.Value = 0;
    //Channel0PWM.Callback = Channel0PWM_cb;
    
    CrossZeroDef = 1;
    
    AutomaticStartStepLength = 333333;
    AutomaticStartStepTargetLength = 15384;
    //AutomaticStartStopInc = 20;

    StepCountingTimer.callback = BLDC_Esc_StepCounting;
    StepCountingTimer.value = 2400000;
    
    BLDC_Esc_Channels[0].mode = CM_Manual;
    BLDC_Esc_Channels[0].state = CS_ManualOff;
    BLDC_Esc_Channels[0].stepTimer.callback = BLDC_Esc_WHL_Step;
    BLDC_Esc_Channels[0].stepTimer.value = 334031;
    BLDC_Esc_Channels[0].stepTimer.tag = 0;
    BLDC_Esc_Channels[0].pwmTimer.callback = BLDC_Esc_WHL_PWM;
    BLDC_Esc_Channels[0].pwmOnBeforeAdc = 167;
    BLDC_Esc_Channels[0].pwmOnAfterAdc = 166;
    BLDC_Esc_Channels[0].pwmOff = 333;
    BLDC_Esc_Channels[0].pwmTimer.tag = 0;
    BLDC_Esc_Channels[0].altSpeedTimer.callback = BLDC_Esc_AltSpeed;
    BLDC_Esc_Channels[0].altSpeedTimer.tag = 0;
    BLDC_Esc_Channels[0].altSpeedTimer.value = 12000;
    //Channel0.stepTimer.Callback = Channel0Step_cb_hard;
    //Channel0.pwmTimer.Callback = Channel0PWM_cb_hard;
    
    //Channel0.stepTimer.Missing = Channel0.stepTimer.Value = 6000004;
}

void BLDC_Esc_Task() {
            //if (Channel0.pwmState == 2 && (Channel0.step == 3 || Channel0.step == 6)) {
            ////if (Channel0.pwmState == 2) {
            //    if (C1OUT)
            //        POWERON = 1;
            //}    
    
    TimerEvent_Tick();

    TimerEvent_Check(&StepCountingTimer);
    
    BLDC_Esc_Tick(0);
    
}

void BLDC_Esc_Tick(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Manual) {
        if (channel->state == CS_ManualOn) {
            TimerEvent_Check(&channel->stepTimer);
            TimerEvent_Check(&channel->pwmTimer);
        }
    }
    else {
        if (channel->state == CS_AutomaticOn) {
            TimerEvent_Counter(&channel->stepLengthCounter);

            if (channel->stepState == CSS_Stable) {
                BLDC_Esc_CrossZeroEvent(channel);
            }
            else {
                TimerEvent_Check(&channel->stepTimer);
            }
            
            TimerEvent_Check(&channel->altSpeedTimer);
            TimerEvent_Check(&channel->pwmTimer);
        }
    }
}

void BLDC_Esc_CrossZeroEvent(struct ChannelStruct *channel) {
    unsigned char doPreCommute = 0;
    
    if (channel->pwmState == 2) {
        unsigned char crosszero = BLDC_Esc_CrossZeroDetect(channel->stepTimer.tag, channel->step);

        if (crosszero != channel->isCrossZero) {
            channel->crossZeroCount--;
            if (channel->crossZeroCount == 0) {
                channel->crossZeroCount = CrossZeroDef;
                channel->isCrossZero = crosszero;

                channel->stepLength = channel->stepLengthCounter.value;

                doPreCommute = 1;
            }
        }
        else {
            channel->crossZeroCount = CrossZeroDef;
        }
    }
    
    if (!doPreCommute && channel->stepLengthCounter.value > channel->stepLength) {
        doPreCommute = 1;
    }
    
    if (doPreCommute) {
        channel->stepLengthCounter.value = 0;

        unsigned long stepLength = channel->stepLength / 2;
        channel->stepTimer.missing = channel->stepTimer.value = stepLength;
        channel->stepState = CSS_PreCommute;
    }
}

void BLDC_Esc_SetManual(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Automatic && channel->state == CS_AutomaticOff) {
        channel->mode = CM_Manual;
        channel->state = CS_ManualOff;
    }
}

void BLDC_Esc_SetManualOn(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Manual && channel->state == CS_ManualOff) {
        channel->stepTimer.missing = channel->stepTimer.value;

        channel->pwmState = 0;
        channel->pwmTimer.value = channel->pwmOnBeforeAdc;
        channel->pwmTimer.missing = 0;
        
        channel->state = CS_ManualOn;
    }
}

void BLDC_Esc_SetManualOff(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Manual && channel->state == CS_ManualOn) {
        BLDC_Esc_WHL_Set_Value(index, channel->step, 0);

        channel->state = CS_ManualOff;
    }
}

void BLDC_Esc_SetManualConfig(unsigned char index, unsigned char direction, unsigned char oneStep) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Manual) {
        channel->isFoward = direction;
        
        if (oneStep) {
            channel->isOneStep = oneStep;
            channel->state = CS_ManualOn;
        }
    }
}

void BLDC_Esc_SetManualStep(unsigned char index, unsigned long stepTicks) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];
    
    if (channel->mode == CM_Manual && channel->state == CS_ManualOn) {
        channel->stepTimer.value = stepTicks;
    }
}

void BLDC_Esc_SetPWM(unsigned char index, unsigned int pwmOnBeforeAdc, unsigned int pwmOnAfterAdc, unsigned int pwmOff) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];
    
    if (channel->mode == CM_Manual && channel->state == CS_ManualOn) {
        channel->pwmOnBeforeAdc = pwmOnBeforeAdc;
        channel->pwmOnAfterAdc = pwmOnAfterAdc;
        channel->pwmOff = pwmOff;
    }    
}

void BLDC_Esc_SetAutomatic(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Manual && channel->state == CS_ManualOff) {
        channel->mode = CM_Automatic;
        channel->state = CS_AutomaticOff;
    }
}

void BLDC_Esc_SetAutomaticOn(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Automatic && channel->state == CS_AutomaticOff) {
        channel->stepState = CSS_Stable;
        channel->stepLength = AutomaticStartStepLength;
        //channel->altSpeedState = CSpeed_Inc;
        //channel->altSpeedCount = AutomaticStartSpeedCount;
        //channel->altSpeedValue = AutomaticStartSpeedValue;
        //channel->stepTimer.missing = channel->stepTimer.value = AutomaticStartStopBegin;

        channel->pwmState = 0;
        channel->pwmTimer.value = channel->pwmOnBeforeAdc;
        channel->pwmTimer.missing = 0;
        
        channel->state = CS_AutomaticOn;
    }
}


void BLDC_Esc_SetAutomaticOff(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Automatic && channel->state == CS_AutomaticOn) {
        
        //channel->automaticStartStopTimer.enabled = 1;
        //channel->stepTimer.enabled = 1;
        channel->state = CS_AutomaticOff;
    }
}

const unsigned long easy_value = 1000000000;

void BLDC_Esc_AltSpeed(unsigned char tag) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[tag];
    
    if (channel->stepLength > AutomaticStartStepTargetLength) {
        float tocalc = easy_value / channel->stepLength;
        tocalc -= AutomaticStartStopInc;
        tocalc = easy_value / tocalc;

        channel->stepLength = (unsigned long)tocalc;
    }
    
    if (channel->altSpeedCount) {
        channel->pwmOnBeforeAdc *= channel->altSpeedValue;
        channel->pwmOnAfterAdc *= channel->altSpeedValue;
        channel->pwmOff *= channel->altSpeedValue;

        channel->altSpeedCount--;
    }
}

void BLDC_Esc_ConfigStartStopCurve(unsigned long beginValue, unsigned int endValue, unsigned char incValue, unsigned int clockValue) {
    AutomaticStartStepLength = beginValue;
    AutomaticStartStepTargetLength = endValue;
    AutomaticStartStopInc = incValue;
    BLDC_Esc_Channels[0].altSpeedTimer.value = clockValue;
}

void BLDC_Esc_SetAltSpeed(unsigned char index, unsigned int speedCount, float speedValue) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];
    
    channel->altSpeedCount = speedCount;
    channel->altSpeedValue = speedValue;
}

void BLDC_Esc_StepCounting() {
    BLDC_Esc_Channels[0].stepCounting = 0;
}