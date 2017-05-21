#include <stdint.h>
#include <xc.h>
#include <htc.h>

#include "BLDC_Esc.h"

void TimerEvent_Tick() {
    TimerActualValue = TMR0L;
    TimerActualValue = (TMR0H << 8) | TimerActualValue;
    
    if (TimerActualValue >= TimerLastValue) {
        TimerDiffValue = TimerActualValue - TimerLastValue;
        //INTCONbits.TMR0IF = 0;
    }
    else {
        TimerDiffValue = (0xFFFF - TimerLastValue) + TimerActualValue;
    }
    
    TimerLastValue = TimerActualValue;
}

void TimerEvent_Check(struct TimerEventStruct *timer) {
    if (TimerDiffValue > timer->Missing) {
        timer->Missing = timer->Value - (TimerDiffValue - timer->Missing);
        timer->Callback();
    }
    else {
        timer->Missing -= TimerDiffValue;
    }
}