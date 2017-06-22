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
    
    CrossZeroDef = 3;
    
    AutomaticStartStopBegin = 333333;
    AutomaticStartStopEnd = 15384;
    AutomaticStartStopInc = 20;

    BLDC_Esc_Channels[0].mode = CM_Manual;
    BLDC_Esc_Channels[0].state = CS_ManualOff;
    BLDC_Esc_Channels[0].stepTimer.value = 334031;
    BLDC_Esc_Channels[0].stepTimer.tag = 0;
    BLDC_Esc_Channels[0].pwmOnBeforeAdc = 167;
    BLDC_Esc_Channels[0].pwmOnAfterAdc = 166;
    BLDC_Esc_Channels[0].pwmOff = 333;
    BLDC_Esc_Channels[0].pwmTimer.tag = 0;
    BLDC_Esc_Channels[0].automaticStartStopTimer.callback = BLDC_Esc_SetAutomaticStartStop;
    BLDC_Esc_Channels[0].automaticStartStopTimer.tag = 0;
    BLDC_Esc_Channels[0].automaticStartStopTimer.value = 12000;
    BLDC_Esc_Channels[0].automaticStartStopTimer.enabled = 0;    
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
        if (channel->state != CS_Automatic_Off) {
            TimerEvent_Counter(&channel->stepLengthCounter);

            if (channel->state == CS_AutomaticRunning) {
                if (channel->pwmState == 2) {
                    BLDC_Esc_CrossZeroEvent(channel);
                }
            }
            else {
            //if (channel->state == CS_AutomaticStarting || channel->state == CS_AutomaticStoping) {
                TimerEvent_Check(&channel->automaticStartStopTimer);
            }
            
            TimerEvent_Check(&channel->stepTimer);
            TimerEvent_Check(&channel->pwmTimer);
        }
    }
}

void BLDC_Esc_CrossZeroEvent(struct ChannelStruct *channel) {
    // if (channel->state == CS_AutomaticStarting) {
    //     unsigned long variableLength = channel->stepTimer.value * 0.05;
    //     if (stepLength > channel->stepTimer.value - variableLength && stepLength < channel->stepTimer.value + variableLength) {
    //         channel->state == CS_AutomaticRunning;
    //         channel->stepTimer.enabled = 0;
    //     }
    // }
    // else if (channel->state == CS_AutomaticStoping) {
    // }

    unsigned char crosszero = BLDC_Esc_CrossZeroDetect(index, channel->step);

    if (crosszero != channel->isCrossZero) {
        channel->crossZeroCount--;
        if (channel->crossZeroCount == 0) {
            channel->crossZeroCount = CrossZeroDef;
            channel->isCrossZero = crosszero;
            
            //BLDC_Esc_CrossZeroEvent(channel);
            unsigned long stepLength = channel->stepLengthCounter.value;
            channel->stepLengthCounter.value = 0;

            stepLength = stepLength / 2;
            channel->stepTimer.missing = channel->stepTimer.value = stepLength;
            channel->stepTimer.enabled = 1;
        }
    }
    else {
        channel->crossZeroCount = CrossZeroDef;
    }
}

void BLDC_Esc_SetManual(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Automatic && channel->state == CS_Automatic_Off) {
        channel->mode = CM_Manual;
        channel->state = CS_ManualOff;
    }
}

void BLDC_Esc_SetManualOn(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Manual && channel->state == CS_ManualOff) {
        channel->stepTimer.callback = BLDC_Esc_WHL_Step;
        channel->stepTimer.missing = channel->stepTimer.value;
        channel->stepTimer.enabled = 1;

        channel->pwmState = 0;
        channel->pwmTimer.callback = BLDC_Esc_WHL_PWM;
        channel->pwmTimer.value = channel->pwmOnBeforeAdc;
        channel->pwmTimer.missing = 0;
        channel->pwmTimer.enabled = 1;
        
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

void BLDC_Esc_SetManualPWM(unsigned char index, unsigned int pwmOnBeforeAdc, unsigned int pwmOnAfterAdc, unsigned int pwmOff) {
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
        channel->state = CS_Automatic_Off;
    }
}

void BLDC_Esc_SetAutomaticOn(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Automatic && channel->state == CS_Automatic_Off) {
        channel->stepTimer.callback = BLDC_Esc_WHL_Step;
        channel->stepTimer.missing = channel->stepTimer.value = AutomaticStartStopBegin;
        channel->stepTimer.enabled = 1;

        channel->pwmState = 0;
        channel->pwmTimer.callback = BLDC_Esc_WHL_PWM;
        channel->pwmTimer.value = channel->pwmOnBeforeAdc;
        channel->pwmTimer.missing = 0;
        channel->pwmTimer.enabled = 1;
        
        channel->automaticStartStopTimer.enabled = 1;
        
        channel->state = CS_AutomaticStarting;
    }
}


void BLDC_Esc_SetAutomaticOff(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Automatic && (channel->state == CS_AutomaticStarting || channel->state == CS_AutomaticRunning)) {
        
        channel->automaticStartStopTimer.enabled = 1;
        channel->stepTimer.enabled = 1;
        channel->state = CS_AutomaticStoping;
    }
}

const unsigned long easy_value = 1000000000;

void BLDC_Esc_SetAutomaticStartStop(unsigned char tag) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[tag];
    
    unsigned float tocalc = 0;

    if (channel->state == CS_AutomaticStarting) {
        tocalc = easy_value / channel->stepTimer.value;
        tocalc -= AutomaticStartStopInc;
        tocalc = easy_value / tocalc;

        channel->stepTimer.value = tocalc;

        if (channel->stepTimer.value <= AutomaticStartStopEnd) {
            channel->automaticStartStopTimer.enabled = 0;
            channel->state = CS_AutomaticRunning;
        }
    }
    else if (channel->state == CS_AutomaticStoping) {
        tocalc = easy_value / channel->stepTimer.value;
        tocalc += AutomaticStartStopInc;
        tocalc = easy_value / tocalc;

        channel->stepTimer.value = tocalc;

        if (channel->stepTimer.value >= AutomaticStartStopBegin) {
            channel->automaticStartStopTimer.enabled = 0;
            channel->state = CS_Automatic_Off;
        }
    }
}

void BLDC_Esc_ConfigStartStopCurve(unsigned int beginValue, unsigned int endValue, unsigned char incValue, unsigned int clockValue) {
    AutomaticStartStopBegin = beginValue;
    AutomaticStartStopEnd = endValue;
    AutomaticStartStopInc = incValue;
    BLDC_Esc_Channels[0].automaticStartStopTimer.value = clockValue;
}