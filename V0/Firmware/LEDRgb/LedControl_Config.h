/* 
 * File:   LedControl_Config.h
 * Author: Clive
 *
 * Created on 20 de Fevereiro de 2016, 18:38
 */

#ifndef LEDCONTROL_CONFIG_H
#define	LEDCONTROL_CONFIG_H

#define LedControl0
uint16_t RED0on    = 100;
uint16_t RED0off   = 0;
#define RED0port     PORTDbits.RD7
uint16_t GREEN0on  = 100;
uint16_t GREEN0off = 100;
#define GREEN0port   PORTDbits.RD6
uint16_t BLUE0on   = 100;
uint16_t BLUE0off  = 100;
#define BLUE0port    PORTDbits.RD5

#endif	/* LEDCONTROL_CONFIG_H */

