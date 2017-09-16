#ifndef TIMER_EVENTS_H
#define	TIMER_EVENTS_H

typedef void (*TimerEventCallback)(unsigned char tag);

struct TimerEventControl {
    unsigned int lastValue;
};

struct TimerEventRotine {
    unsigned long value;
    unsigned long missing;
    TimerEventCallback callback;
    unsigned char tag;
    struct TimerEventControl ctr;
};

struct TimerEventCounter {
    unsigned long value;
    struct TimerEventControl ctr;
};


void TimerEventCounter_Tick(struct TimerEventCounter *counter);

void TimerEventCounter_Clear(struct TimerEventCounter *counter);

void TimerEventRotine_Tick(struct TimerEventRotine *rotine);

void TimerEventRotine_Reset(struct TimerEventRotine *rotine);

void TimerEventRotine_Set(struct TimerEventRotine *rotine, unsigned long value);

#endif	/* TIMER_EVENTS */