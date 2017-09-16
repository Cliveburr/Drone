#ifndef BLDC_ESC_WORKHIGHLOW_H
#define	BLDC_ESC_WORKHIGHLOW_H

void BLDC_Esc_WHL_Step(unsigned char tag);

void BLDC_Esc_WHL_PWM(unsigned char tag);

void BLDC_Esc_WHL_Set_Value(unsigned char index, unsigned char step, unsigned char value);

void BLDC_Esc_CrossZeroPortSelect(unsigned char index, unsigned char step);

unsigned char BLDC_Esc_CrossZeroDetect(unsigned char index, unsigned char step);

#endif	/* BLDC_ESC_WORKHIGHLOW_H */