#include "BLDC_Esc_WorkHighLow.h"
#include "BLDC_Esc.h"
#include "BLDC_Esc_User.h"

#include "system.h"

void BLDC_Esc_WHL_Step(unsigned char tag) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[tag];
    
    //if (channel->mode == CM_Automatic && channel->stepState == CSS_PosCommute) {
    //    channel->stepState = CSS_Stable;
    //    return;
    //}
    
    BLDC_Esc_WHL_Set_Value(tag, channel->step, 0);
    
    if (channel->isFoward) {
        if (channel->step == 6)
            channel->step = 1;
        else
            channel->step++;
    }
    else {
        if (channel->step <= 1)
            channel->step = 6;
        else
            channel->step--;
    }

    if (channel->mode == CM_Manual) {
        if (channel->isOneStep) {
            channel->state = CS_ManualOff;
            channel->isOneStep = 0;
        }
    }
    else {
        switch (channel->automaticState) {
            case CAS_Starting:
            {
                //if (channel->stepLength < AutomaticStartStepTargetLength) {
                if (channel->stepTimer.value < AutomaticStartStepTargetLength) {
                    channel->automaticState = CAS_Running;
                    channel->stepState = CSS_CrossZeroDetect; // CSS_PosCommute;
                    //channel->stepTimer.value = channel->stepTimer.missing = channel->stepLength * 0.05;
                    channel->stepTimer.value /= 2;
                }
                break;
            }
            case CAS_Running:
            {
                channel->stepState = CSS_CrossZeroDetect; // CSS_PosCommute;
                //channel->stepTimer.value = channel->stepTimer.missing = channel->stepLength * 0.05;
                //channel->stepLength = channel->stepLengthCounter.value;
                break;
            }
            case CAS_Stopping:
            {
                //if (channel->stepLength > AutomaticStartStepLength) {
                if (channel->stepTimer.value > AutomaticStartStepLength) {
                    channel->state = CS_AutomaticOff;
                }
                break;
            }
        }
            
    }
    
    if (findCross) {
        //POWERON = 1;
    }
    findCross = 0;

    //90 < 91 < 110
    TimerEventCounter_Tick(&channel->stepLengthCounter);
    float bycross = channel->stepLengthCounter.value;
    float valueminor = channel->stepTimer.value * 0.80;
    float valuemajor = channel->stepTimer.value * 1.20;
    if (!(valueminor < bycross && bycross < valuemajor)) {
        POWERON = 1;
    }
    //if (!((channel->stepTimer.value * 0.99) < channel->stepLengthCounter.value < (channel->stepTimer.value * 1.01))) {
    //    POWERON = 1;
    //}
    TimerEventCounter_Clear(&channel->stepLengthCounter);

    BLDC_Esc_CrossZeroPortSelect(tag, channel->step);
    
    channel->stepCounting++;
    
    //channel->pwmState == CPWMS_Off;
    BLDC_Esc_WHL_Set_Value(tag, channel->step, channel->pwmState != CPWMS_Off);
}

void BLDC_Esc_WHL_PWM(unsigned char tag) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[tag];
    
    switch (channel->pwmState) {
        case CPWMS_Off:
            channel->pwmState = CPWMS_BeforeAdc;
            channel->pwmTimer.missing = channel->pwmOnBeforeAdc;
            //channel->pwmTimer.value = channel->pwmOnAfterAdc;
            BLDC_Esc_WHL_Set_Value(tag, channel->step, 1);
            break;
        case CPWMS_BeforeAdc:
            channel->pwmState = CPWMS_OnAdc;
            channel->pwmTimer.missing = channel->pwmOnAdc;
            //channel->pwmTimer.value = channel->pwmOff;
            break;
        case CPWMS_OnAdc:
            channel->pwmState = CPWMS_Off;
            channel->pwmTimer.missing = channel->pwmOff;
            //channel->pwmTimer.value = channel->pwmOnBeforeAdc;
            BLDC_Esc_WHL_Set_Value(tag, channel->step, 0);
            break;
    }
}

void BLDC_Esc_WHL_Set_Value(unsigned char index, unsigned char step, unsigned char value) {
    switch (index) {
        case 0:
        {
            switch(step) {
                case 1:
                    PHASEAH = value;
                    PHASEBL = value;
                    break;
                case 2:
                    PHASEAH = value;
                    PHASECL = value;
                    break;
                case 3:
                    PHASEBH = value;
                    PHASECL = value;
                    break;
                case 4:
                    PHASEBH = value;
                    PHASEAL = value;
                    break;
                case 5:
                    PHASECH = value;
                    PHASEAL = value;
                    break;
                case 6:
                    PHASECH = value;
                    PHASEBL = value;
                    break;
            }
            break;
        }
    }
}

// Cis = 0 .. RA0 = C1OUT .. RA1 = C2OUT
// Cis = 1 .. RA3 = C1OUT .. RA2 = C2OUT

void BLDC_Esc_CrossZeroPortSelect(unsigned char index, unsigned char step) {
    switch (index) {
        case 0:
        {
            switch(step) {
                case 1:
                case 4: // channel C free - RA2
                {
                    CMCON = 0b00111110;
                    break;
                }
                case 2:
                case 5: // channel B free - RA1
                {
                    CMCON = 0b00110110;
                    break;
                }
                case 3:
                case 6: // channel A free - RA0
                {
                    CMCON = 0b00110110;
                    break;
                }
            }
            break;
        }
    }
}

unsigned char BLDC_Esc_CrossZeroDetect(unsigned char index, unsigned char step) {
    switch (index) {
        case 0:
        {
            switch(step) {
                case 1:
                case 4: // channel C free
                    return C2OUT;
                case 2:
                case 5: // channel B free
                    return C2OUT;
                case 3:
                case 6: // channel A free
                    return C1OUT;
            }
            break;
        }
    }
}