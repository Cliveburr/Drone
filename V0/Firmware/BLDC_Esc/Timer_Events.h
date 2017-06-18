#ifndef TIMER_EVENTS_H
#define	TIMER_EVENTS_H

unsigned short TimerLastValue = 0;
unsigned short TimerActualValue = 0;
unsigned short TimerDiffValue = 0;

typedef void (*TimerEventCallback)(unsigned char tag);

struct TimerEventStruct {
    unsigned long value;
    unsigned long missing;
    TimerEventCallback callback;
    unsigned char enabled;
    unsigned char tag;
};

void TimerEvent_Tick();

void TimerEvent_Check(struct TimerEventStruct *timer);

struct TimerEventCounter {
    unsigned long value;
};

void TimerEvent_Counter(struct TimerEventCounter *counter);

#endif	/* TIMER_EVENTS */