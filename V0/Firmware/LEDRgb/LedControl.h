/* 
 * File:   LedControl.h
 * Author: Clive
 *
 * Created on 2 de Fevereiro de 2016, 21:27
 */

#ifndef LEDCONTROL_H
#define	LEDCONTROL_H

#include <xc.h>
#include <stdint.h>

#include "LedControl_Config.h"

#ifdef LedControl0
    uint16_t RED0count;
    uint16_t GREEN0count;
    uint16_t BLUE0count;
#endif

void LedControl_Tick();

void LedControl_Init();

#endif	/* LEDCONTROL_H */

