#include <stdint.h>
#include <xc.h>
#include <htc.h>

#include "BLDC_Esc.h"

void BLDC_Esc_Initialize() {
    //test1.Value = 46875;
    //test1.Missing = test1.Value = 6000004; //144000;
    //test1.Callback = test1_cb;
    
    //Channel0PWM.Missing = Channel0PWM.Value = 0;
    //Channel0PWM.Callback = Channel0PWM_cb;
    
    CrossZeroDef = 3;

    Channel0.mode = CM_Manual;
    Channel0.state = CS_ManualOff;
    Channel0.stepTimer.Value = 334031;
    Channel0.pwmOnBeforeAdc = 167;
    Channel0.pwmOnAfterAdc = 166;
    Channel0.pwmOff = 333;
    //Channel0.stepTimer.Callback = Channel0Step_cb_hard;
    //Channel0.pwmTimer.Callback = Channel0PWM_cb_hard;
    
    //Channel0.stepTimer.Missing = Channel0.stepTimer.Value = 6000004;
}

void BLDC_Esc_Task() {
            if (Channel0.pwmState == 2 && (Channel0.step == 3 || Channel0.step == 6)) {
            //if (Channel0.pwmState == 2) {
                if (C1OUT)
                    POWERON = 1;
            }    
    
    TimerEvent_Tick();

    BLDC_Esc_Tick(&Channel0);
    
}

void BLDC_Esc_Tick(struct ChannelStruct *channel) {
    if (channel->mode == CM_Manual) {
        if (channel->state == CS_ManualOn) {
            TimerEvent_Check(&channel->stepTimer);
            TimerEvent_Check(&channel->pwmTimer);
        }
    }
    else {
        if (channel->state != CS_Automatic_Off) {
            TimerEvent_Counter(&channel->stepLengthCounter);

            // detect cross zero
            unsigned char crosszero = 1;

            if (crosszero != channel->isCrossZero) {
                channel->crossZeroCount--;
                if (channel->crossZeroCount == 0) {
                    channel->crossZeroCount = CrossZeroDef;
                    channel->isCrossZero = crosszero;
                    
                    CrossZeroEvent(channel);
                }
            }
            else {
                channel->crossZeroCount = CrossZeroDef;
            }

            TimerEvent_Check(&channel->stepTimer);
            TimerEvent_Check(&channel->pwmTimer);
        }
    }
}

void CrossZeroEvent(struct ChannelStruct *channel) {
    unsigned long stepLength = channel->stepLengthCounter.value;
    channel->stepLengthCounter.value = 0;

    if (channel->state == CS_AutomaticStarting) {
        unsigned long variableLength = channel->stepTimer.Value * 0.05;
        if (stepLength > channel->stepTimer.Value - variableLength && stepLength < channel->stepTimer.Value + variableLength) {
            channel->state == CS_AutomaticRunning;
            channel->stepTimer.enabled = 0;
        }
    }
    else if (channel->state == CS_AutomaticStoping) {
    }
    else if (channel->state == CS_AutomaticRunning) {
    }
}

struct ChannelStruct* GetChannelById(unsigned char channelId) {
    switch (channelId) {
        case 0: return &Channel0;
    }
}

void BLDC_Esc_SetManual(unsigned char channelId) {
    struct ChannelStruct* channel = GetChannelById(channelId);

    if (channel->mode == CM_Automatic && channel->state == CS_Automatic_Off) {
        channel->mode = CM_Manual;
        channel->state = CS_ManualOff;
    }
}

void BLDC_Esc_SetManualOn(unsigned char channelId) {
    struct ChannelStruct* channel = GetChannelById(channelId);

    if (channel->mode == CM_Manual && channel->state == CS_ManualOff) {
        channel->stepTimer.Callback = Channel0Step_cb_hard;
        channel->stepTimer.Missing = channel->stepTimer.Value;

        channel->pwmState = 0;
        channel->pwmTimer.Callback = Channel0PWM_cb_hard;
        channel->pwmTimer.Value = channel->pwmOnBeforeAdc;
        channel->pwmTimer.Missing = 0;
        channel->state = CS_ManualOn;
    }
}

void BLDC_Esc_SetManualOff(unsigned char channelId) {
    struct ChannelStruct* channel = GetChannelById(channelId);

    if (channel->mode == CM_Manual && channel->state == CS_ManualOn) {
        // desligar os canais

        channel->state = CS_ManualOff;
    }
}

void BLDC_Esc_SetAutomatic(unsigned char channelId) {
    struct ChannelStruct* channel = GetChannelById(channelId);

    if (channel->mode == CM_Manual && channel->state == CS_ManualOff) {
        channel->mode = CM_Automatic;
        channel->state = CS_Automatic_Off;
    }
}

void BLDC_Esc_SetAutomaticStart(unsigned char channelId) {
    struct ChannelStruct* channel = GetChannelById(channelId);

    if (channel->mode == CM_Automatic && channel->state == CS_Automatic_Off) {
        
        channel->state = CS_AutomaticStarting;
    }
}