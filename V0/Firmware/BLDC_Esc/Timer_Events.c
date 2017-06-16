#include <stdint.h>
#include <xc.h>
#include <htc.h>
#include <limits.h>

#include "Timer_Events.h"

void TimerEvent_Tick() {
    TimerActualValue = TMR0L;
    TimerActualValue = (TMR0H << 8) | TimerActualValue;
    
    if (TimerActualValue >= TimerLastValue) {
        TimerDiffValue = TimerActualValue - TimerLastValue;
        //INTCONbits.TMR0IF = 0;
    }
    else {
        TimerDiffValue = (USHRT_MAX - TimerLastValue) + TimerActualValue;
    }
    
    TimerLastValue = TimerActualValue;
}

void TimerEvent_Check(struct TimerEventStruct *timer) {
    if (timer->enabled) {
        if (TimerDiffValue >= timer->missing) {
            timer->missing = timer->value - (TimerDiffValue - timer->missing);
            timer->callback();
        }
        else {
            timer->missing -= TimerDiffValue;
        }
    }
}

void TimerEvent_Counter(struct TimerEventCounter *counter) {
    if (TimerDiffValue > ULONG_MAX - counter->value) {
        counter->value = ULONG_MAX;
    }
    else {
        counter->value += TimerDiffValue;
    }
}