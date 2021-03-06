#include <stdint.h>
#include <xc.h>
#include <htc.h>

#include "BLDC_Esc.h"
#include "BLDC_Esc_WorkHighLow.h"

#include "system.h"

void BLDC_Esc_Initialize() {
    //test1.Value = 46875;
    //test1.Missing = test1.Value = 6000004; //144000;
    //test1.Callback = test1_cb;
    
    //Channel0PWM.Missing = Channel0PWM.Value = 0;
    //Channel0PWM.Callback = Channel0PWM_cb;
    
    CrossZeroDef = 1;
    
    AutomaticStartStepLength = 333333;
    AutomaticStartStepTargetLength = 15384;
    AutomaticStartStopInc = 15;

    StepCountingTimer.callback = BLDC_Esc_StepCounting;
    StepCountingTimer.value = 2400000;
    
    BLDC_Esc_Channels[0].mode = CM_Manual;
    BLDC_Esc_Channels[0].state = CS_ManualOff;
    BLDC_Esc_Channels[0].stepTimer.callback = BLDC_Esc_WHL_Step;
    BLDC_Esc_Channels[0].stepTimer.value = 1000000;
    BLDC_Esc_Channels[0].stepTimer.tag = 0;
    BLDC_Esc_Channels[0].pwmTimer.callback = BLDC_Esc_WHL_PWM;
    BLDC_Esc_Channels[0].pwmOnBeforeAdc = 33;
    BLDC_Esc_Channels[0].pwmOnAdc = 298;
    BLDC_Esc_Channels[0].pwmOff = 331;
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
    
    TimerEventRotine_Tick(&StepCountingTimer);
    
    BLDC_Esc_Tick(0);

}

void BLDC_Esc_Tick(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Manual) {
        if (channel->state == CS_ManualOn) {
            TimerEventCounter_Tick(&channel->stepLengthCounter);

            BLDC_Esc_CrossZeroEvent(channel);
            
            TimerEventRotine_Tick(&channel->stepTimer);
            TimerEventRotine_Tick(&channel->pwmTimer);
        }
    }
    else {
        if (channel->state == CS_AutomaticOn) {
            TimerEventCounter_Tick(&channel->stepLengthCounter);

            if (channel->stepState == CSS_CrossZeroDetect) {
                BLDC_Esc_CrossZeroEvent(channel);
            }

            switch (channel->automaticState) {
                case CAS_Starting:
                case CAS_Stopping:
                {
                    TimerEventRotine_Tick(&channel->stepTimer);
                    break;
                }
                case CAS_Running:
                {
                    if (channel->stepState == CSS_TimeToCommute) {
                        TimerEventRotine_Tick(&channel->stepTimer);
                    }
                    break;
                }
            }
            
            TimerEventRotine_Tick(&channel->pwmTimer);
            TimerEventRotine_Tick(&channel->altSpeedTimer);
        }
    }
}

void BLDC_Esc_CrossZeroEvent(struct ChannelStruct *channel) {
    //unsigned char doPreCommute = 0;
    
    if (channel->pwmState == CPWMS_OnAdc) { // && channel->automaticState == CAS_Running) {
        unsigned char crosszero = BLDC_Esc_CrossZeroDetect(channel->stepTimer.tag, channel->step);

        if (crosszero != channel->isCrossZero) {
            //POWERON ^= 1;
            
            //channel->crossZeroCount--;
            //if (channel->crossZeroCount == 0) {
                //channel->crossZeroCount = CrossZeroDef;
                channel->isCrossZero = crosszero;

                //channel->stepLength = channel->stepLengthCounter.value * 2;

                //doPreCommute = 1;
                if (channel->automaticState == CAS_Running) {
                    //unsigned long stepLength = channel->stepLength / 2;
                    //TimerEventRotine_Set(&channel->stepTimer, stepLength);
                    TimerEventRotine_Set(&channel->stepTimer, channel->stepLengthCounter.value);
                    channel->stepState = CSS_TimeToCommute;
                }
                findCross = 1;
                
            //TimerEventCounter_Tick(&channel->stepLengthCounter);
            //unsigned long bycross = channel->stepLengthCounter.value * 2;
            //unsigned long valueminor = channel->stepTimer.value * 0.80;
            //unsigned long valuemajor = channel->stepTimer.value * 1.20;
            //if (!(valueminor < bycross && bycross < valuemajor)) {
            //    POWERON = 1;
           // }

            //}
        }
        //else {
        //    channel->crossZeroCount = CrossZeroDef;
        //}
    }
    
    //if (!doPreCommute && channel->stepLengthCounter.value > channel->stepLength) {
    
    if (channel->stepLengthCounter.value > channel->stepTimer.value * 2) {
        //doPreCommute = 1;
        channel->stepTimer.missing = 0;
        channel->stepState = CSS_TimeToCommute;
    }
    
    //if (doPreCommute) {
        //channel->stepLengthCounter.value = 0;

    //    unsigned long stepLength = channel->stepLength / 2;
    //    channel->stepTimer.missing = channel->stepTimer.value = stepLength;
    //    channel->stepState = CSS_PreCommute;
    //}
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
        TimerEventRotine_Reset(&channel->stepTimer);

        channel->pwmState = CPWMS_Off;
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
            
            channel->pwmState = CPWMS_Off;
            BLDC_Esc_WHL_PWM(0);
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
        channel->pwmOnAdc = pwmOnAfterAdc;
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
        channel->stepState = CSS_CrossZeroDetect; //CSS_Stable;
        //channel->stepLength = AutomaticStartStepLength;
        channel->automaticState = CAS_Starting;
        //channel->altSpeedState = CSpeed_Inc;
        //channel->altSpeedCount = AutomaticStartSpeedCount;
        //channel->altSpeedValue = AutomaticStartSpeedValue;
        //channel->stepTimer.missing = channel->stepTimer.value = AutomaticStartStopBegin;
        TimerEventRotine_Set(&channel->stepTimer, AutomaticStartStepLength);
        TimerEventRotine_Reset(&channel->altSpeedTimer);

        channel->pwmState = CPWMS_Off;
        channel->pwmTimer.missing = 0;
        
        channel->state = CS_AutomaticOn;
    }
}

void BLDC_Esc_SetAutomaticOff(unsigned char index) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[index];

    if (channel->mode == CM_Automatic && channel->state == CS_AutomaticOn) {
        
        //channel->automaticStartStopTimer.enabled = 1;
        //channel->stepTimer.enabled = 1;
        //channel->state = CS_AutomaticOff;
        channel->automaticState = CAS_Stopping;
        channel->stepState = CSS_CrossZeroDetect; //CSS_Stable;
    }
}

const unsigned long easy_value = 1000000000;
//     const float easy_value = 10000000000;
const float freq = 0.0000000833333333333333;

float AutomaticStartfreq = 1;

void BLDC_Esc_AltSpeed(unsigned char tag) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[tag];
    float tocalc;
    
    switch (channel->automaticState) {
        case CAS_Starting:
        {
            //AutomaticStartfreq += 0.17;
            //tocalc = 1 / AutomaticStartfreq;
            //tocalc /= freq;
            //channel->stepLength = tocalc / 36;
            
            //=ticks-(((RPMtarget-RPMatual)/RPMtarget)*ticks)
            //unsigned int RPMtarget = AutomaticStartRPM + AutomaticStartStopInc;
            //channel->stepLength -= (((RPMtarget - AutomaticStartRPM) / RPMtarget) * channel->stepLength);
            //AutomaticStartRPM = RPMtarget;
            
            tocalc = (float)(easy_value / channel->stepTimer.value);
            tocalc += AutomaticStartStopInc;
            tocalc = (float)(easy_value / tocalc);

            channel->stepTimer.value = (unsigned long)tocalc;
            //channel->stepTimer.value = channel->stepLength = (unsigned long)tocalc;
            
            break;
        }
        case CAS_Running:
        {
            if (channel->altSpeedCount && 0) {
                channel->pwmOnBeforeAdc *= channel->altSpeedValue;
                channel->pwmOnAdc *= channel->altSpeedValue;
                channel->pwmOff *= channel->altSpeedValue;

                channel->altSpeedCount--;
            }

            break;
        }
        case CAS_Stopping:
        {
            tocalc = easy_value / channel->stepTimer.value;
            tocalc -= AutomaticStartStopInc;
            tocalc = easy_value / tocalc;

            channel->stepTimer.value = (unsigned long)tocalc;
            //channel->stepTimer.value = channel->stepLength = (unsigned long)tocalc;
            
            break;
        }
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
    BLDC_Esc_Channels[0].stepCountingVar = BLDC_Esc_Channels[0].stepCounting;
    BLDC_Esc_Channels[0].stepCounting = 0;
    POWERON = 0;
}