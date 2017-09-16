#include <xc.h>

#include "BLDC_Esc.h"
#include "BLDC_Esc_PWM.h"

void BLDC_Esc_PWM_Rotine(unsigned char tag) {
    struct ChannelStruct* channel = &BLDC_Esc_Channels[tag];
    
    switch (channel->pwmState) {
        case PWMS_Off:
            channel->pwmState = PWMS_BeforeAdc;
            channel->pwmTimer.missing = channel->pwmOnBeforeAdc;
            BLDC_Esc_WHL_Set_Value(tag, channel->step, 1);
            break;
        case PWMS_BeforeAdc:
            channel->pwmState = PWMS_OnAdc;
            channel->pwmTimer.missing = channel->pwmOnAdc;
            break;
        case PWMS_OnAdc:
            channel->pwmState = PWMS_Off;
            channel->pwmTimer.missing = channel->pwmOff;
            BLDC_Esc_WHL_Set_Value(tag, channel->step, 0);
            break;
    }
}