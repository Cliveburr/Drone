#ifndef BLDC_ESC_WORKHIGHLOW_H
#define	BLDC_ESC_WORKHIGHLOW_H

void BLDC_Esc_WHL_Step(unsigned char tag);

void BLDC_Esc_WHL_PWM(unsigned char tag);

void BLDC_Esc_WHL_Set_Value(unsigned char index, unsigned char step, unsigned char value);

#endif	/* BLDC_ESC_WORKHIGHLOW_H */