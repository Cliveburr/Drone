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

    BLDC_Esc_Channels[0].mode = CM_Manual;
    BLDC_Esc_Channels[0].state = CS_ManualOff;
    BLDC_Esc_Channels[0].stepTimer.value = 334031;
    BLDC_Esc_Channels[0].pwmOnBeforeAdc = 167;
    BLDC_Esc_Channels[0].pwmOnAfterAdc = 166;
    BLDC_Esc_Channels[0].pwmOff = 333;
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

    BLDC_Esc_Tick(&BLDC_Esc_Channels[0]);
    
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
        unsigned long variableLength = channel->stepTimer.value * 0.05;
        if (stepLength > channel->stepTimer.value - variableLength && stepLength < channel->stepTimer.value + variableLength) {
            channel->state == CS_AutomaticRunning;
            channel->stepTimer.enabled = 0;
        }
    }
    else if (channel->state == CS_AutomaticStoping) {
    }
    else if (channel->state == CS_AutomaticRunning) {
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
        //channel->stepTimer.callback = Channel0Step_cb_hard;
        channel->stepTimer.missing = channel->stepTimer.value;

        channel->pwmState = 0;
        //channel->pwmTimer.callback = Channel0PWM_cb_hard;
        channel->pwmTimer.value = channel->pwmOnBeforeAdc;
        channel->pwmTimer.missing = 0;
        channel->state = CS_ManualOn;
    }
}

void BLDC_Esc_SetManualOff(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Manual && channel->state == CS_ManualOn) {
        // desligar os canais

        channel->state = CS_ManualOff;
    }
}

void BLDC_Esc_SetAutomatic(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Manual && channel->state == CS_ManualOff) {
        channel->mode = CM_Automatic;
        channel->state = CS_Automatic_Off;
    }
}

void BLDC_Esc_SetAutomaticStart(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Automatic && channel->state == CS_Automatic_Off) {
        
        channel->state = CS_AutomaticStarting;
    }
}