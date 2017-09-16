#include <stdint.h>
#include <xc.h>
#include <htc.h>
#include <limits.h>

#include "Timer_Events.h"

unsigned int GetActualTimer() {
    unsigned int actualValue = TMR0L;
    actualValue = (TMR0H << 8) | actualValue;
    return actualValue;
}

unsigned int ReadDiffValue(struct TimerEventControl *ctr) {
    unsigned int diffValue = 0;
    unsigned int actualValue = GetActualTimer();

    if (actualValue >= ctr->lastValue) {
        diffValue = actualValue - ctr->lastValue;
    }
    else {
        diffValue = (USHRT_MAX - ctr->lastValue) + actualValue;
    }

    ctr->lastValue = actualValue;
    return diffValue;
}

void TimerEventCounter_Tick(struct TimerEventCounter *counter) {
    unsigned int diffValue = ReadDiffValue(&counter->ctr);

    if (diffValue > ULONG_MAX - counter->value) {
        counter->value = ULONG_MAX;
    }
    else {
        counter->value += diffValue;
    }
}

void TimerEventCounter_Clear(struct TimerEventCounter *counter) {
    counter->ctr.lastValue = GetActualTimer();
    counter->value = 0;
}

void TimerEventRotine_Tick(struct TimerEventRotine *rotine) {
    unsigned int diffValue = ReadDiffValue(&rotine->ctr);

    if (diffValue >= rotine->missing) {
        rotine->missing = rotine->value - (diffValue - rotine->missing);
        rotine->callback(rotine->tag);
    }
    else {
        rotine->missing -= diffValue;
    }
}

void TimerEventRotine_Reset(struct TimerEventRotine *rotine) {
    rotine->ctr.lastValue = GetActualTimer();
    rotine->missing = rotine->value;
}

void TimerEventRotine_Set(struct TimerEventRotine *rotine, unsigned long value) {
    rotine->ctr.lastValue = GetActualTimer();
    rotine->value = value;
    rotine->missing = value;
}