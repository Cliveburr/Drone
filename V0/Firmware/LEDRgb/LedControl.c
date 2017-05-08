
#include <stdint.h>

#include "LedControl.h"

void LedControl_Tick()
{
#ifdef LedControl0
    //  Red
    if (RED0port && (RED0off > 0))
    {
        if (RED0count > RED0on)
        {
            RED0port = 0;
            RED0count = 0;
        }
        else
        {
            RED0count++;
        }
    }
    else if (!RED0port && RED0on > 0)
    {
        if (RED0count > RED0off)
        {
            RED0port = 1;
            RED0count = 0;
        }
        else
        {
            RED0count++;
        }
    }

    // Green
    if (GREEN0port && GREEN0off > 0)
    {
        if (GREEN0count > GREEN0on)
        {
            GREEN0port = 0;
            GREEN0count = 0;
        }
        else
        {
            GREEN0count++;
        }
    }
    else if (!GREEN0port && GREEN0on > 0)
    {
        if (GREEN0count > GREEN0off)
        {
            GREEN0port = 1;
            GREEN0count = 0;
        }
        else
        {
            GREEN0count++;
        }
    }
    
    // Blue
    if (BLUE0port && BLUE0off > 0)
    {
        if (BLUE0count > BLUE0on)
        {
            BLUE0port = 0;
            BLUE0count = 0;
        }
        else
        {
            BLUE0count++;
        }
    }
    else if (!BLUE0port && BLUE0on > 0)
    {
        if (BLUE0count > BLUE0off)
        {
            BLUE0port = 1;
            BLUE0count = 0;
        }
        else
        {
            BLUE0count++;
        }
    }
#endif
}

void LedControl_Init()
{
#ifdef LedControl0
    RED0port = RED0on != 0 ? 1: 0;
    RED0count = 0;
    GREEN0port = GREEN0on != 0 ? 1: 0;
    GREEN0count = 0;
    BLUE0port = BLUE0on != 0 ? 1: 0;
    BLUE0count = 0;
#endif
}