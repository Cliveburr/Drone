#include "BLDC_Esc_WorkHighLow.h"
#include "BLDC_Esc.h"
#include "BLDC_Esc_User.h"

#include "system.h"

void BLDC_Esc_WHL_Step(unsigned char tag) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[tag];
    
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
        if (channel->state == CS_AutomaticRunning) {
            channel->stepTimer.enabled = 0;
        }

        BLDC_Esc_CrossZeroPortSelect(tag, channel->step);
    }
    
    BLDC_Esc_WHL_Set_Value(tag, channel->step, channel->pwmState != 3);
}

void BLDC_Esc_WHL_PWM(unsigned char tag) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[tag];
    
    channel->pwmState += 1;
    
    switch (channel->pwmState) {
        case 1:  // start the pwm high part
            BLDC_Esc_WHL_Set_Value(tag, channel->step, 1);
            //Channel0.pwmTimer.Missing = Channel0.pwmOnBeforeAdc;
            channel->pwmTimer.value = channel->pwmOnAfterAdc;
            break;
        case 2:  // position to start the aquisition
            //Channel0.pwmTimer.Missing = Channel0.pwmOnAfterAdc;
            channel->pwmTimer.value = channel->pwmOff;
            break;
        case 3:  // start the pwm low part
            BLDC_Esc_WHL_Set_Value(tag, channel->step, 0);
            //Channel0.pwmTimer.Missing = Channel0.pwmOff;
            channel->pwmTimer.value = channel->pwmOnBeforeAdc;
            channel->pwmState = 0;
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