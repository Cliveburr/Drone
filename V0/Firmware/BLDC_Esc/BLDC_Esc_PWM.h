#ifndef BLDC_ESC_PWM_H
#define	BLDC_ESC_PWM_H

#include "Timer_Events.h"

enum PWM_State {
    PWMS_Off = 0,
    PWMS_BeforeAdc = 1,
    PWMS_OnAdc = 2
};

struct PWM_Struct {
    enum PWM_State state;
    unsigned int onBeforeAdc;
    unsigned int onAdc;
    unsigned int off;
    struct TimerEventRotine rotine;
};

#endif	/* BLDC_ESC_PWM_H */