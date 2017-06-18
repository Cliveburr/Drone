#include <xc.h>
#include "system.h"
#include <string.h>
//#include "system_config.h"
//#include "usb.h"

//#include <p18f4550.h>

#include "BLDC_Esc.h"

// CONFIG1L
#pragma config PLLDIV = 5       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 2       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = HSPLL_HS  // Oscillator Selection bits (EC oscillator, CLKO function on RA6 (EC))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = ON     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

/*
void Channel0Step_cb();
void Channel0PWM_cb();
void Channel0SetHighValue();

void Channel0Step_cb_hard();
void Channel0PWM_cb_hard();
void Channel0SetValue_hard(unsigned char value);
 */

void SYSTEM_Initialize()
{
    //ANSEL = 0b00000000;   // Set port as digital or analogic, 0 digital, 1 analogic
    
    // bit 0-3  - A/D Port Configuration Control bits   (1110 = Only AN0 is analog)
    // bit 4    - Voltage Reference Configuration bit (1 = VREF+, 0 = VDD)
    // bit 5    - Voltage Reference Configuration bit (1 = VREF-, 0 = VSS)
    // bit 6-7  - Unimplemented
    ADCON1 = 0b00001011;
    
    // bit 7    - A/D Result Format Select bit (1 = Right justified, 0 = Left justified)
    // bit 6    - Unimplemented
    // bit 3-5  - A/D Acquisition Time Select bits (001 = 2 TAD)
    // bit 0-2  - A/D Conversion Clock Select bits (110 =  TOSC * 64)
    ADCON2 = 0b10001110;

    // bit 0    - A/D On bit
    // bit 1    - Go/Done 1 = progress 0 = idle
    // bit 2-5  - Analog Channel Select bits   (0000 = AN0)
    // bit 6-7  - Unimplemented
    ADCON0 = 0b00000000;
    
    
    // bit 0-2  - Comparator Mode bits
    // bit 3    - Comparator Input Switch bit
    // bit 4    - Comparator 1 Output Inversion bit
    // bit 5    - Comparator 2 Output Inversion bit
    // bit 6    - Comparator 1 Output bit (1 = C1 VIN+ > C1 VIN)
    // bit 7    - Comparator 2 Output bit (1 = C2 VIN+ > C2 VIN-)
    CMCON = 0b00110110;
    
    // bit 0-3  - Comparator VREF Value Selection bits
    // bit 4    - Comparator VREF Source Selection bit
    // bit 5    - Comparator VREF Range Selection bit
    // bit 6    - Comparator VREF Output Enable bit
    // bit 7    - Comparator Voltage Reference Enable bit
    CVRCON = 0b10100101;
    
    
    
    INTCON = 0b00000000;
    
    //TRISIO = 0b00000000;  // Set port as input or output, 0 output, 1 input
    //GPIO = 0b00000000;    // Set value for ports
    
    TRISA = 0b00001111;
    TRISB = 0b00000000;
    TRISC = 0b00000110;
    TRISD = 0b00000001;
    
    PORTA = 0b00000001;
    PORTB = 0b00000000;
    PORTC = 0b00000000;
    PORTD = 0b00000000;
    
    // bit 0-2   - Prescaler Select bits (111 = 1:256, 110 = 1:128, 101 = 1:64, 100 = 1:32, 011 = 1:16, 010 = 1:8, 001 = 1:4, 000 = 1:2)
    // bit 3     - Prescaler Assignment bit (1 = TImer0 prescaler is NOT assigned, 0 = Timer0 prescaler is assigned)
    // bit 4     - Source Edge Select bit (1 = Increment on high-to-low, 0 = Increment on low-to-high)
    // bit 5     - Clock Source Select bit (1 = Transition on T0CKI pin, 0 = Internal instruction cycle clock)
    // bit 6     - Timer0 8-Bit/16-Bit Control bit (1 = Timer0 is configured as an 8-bit, 0 = Timer0 is configured as a 16-bit)
    // bit 7     - Timer0 On/Off Control bit (1 = Enabled, 0 = Disabled)
    T0CON = 0b10001011;
    
    BLDC_Esc_Initialize();
}

void SYSTEM_Task() {

    BLDC_Esc_Task();

}

/*
void Channel0Step_cb() {
    //POWERON ^= 1;
    
    if (!Channel0.isRunning)
        return;
    
    if (Channel0.isFoward) {
        if (Channel0.step == 6)
            Channel0.step = 1;
        else
            Channel0.step++;
        
        switch(Channel0.step) {
            case 1:
                PHASECH = 0;
                Channel0SetHighValue();
                break;
            case 2:
                PHASEBL = 0;
                PHASECL = 1;
                break;
            case 3:
                PHASEAH = 0;
                Channel0SetHighValue();
                break;
            case 4:
                PHASECL = 0;
                PHASEAL = 1;
                break;
            case 5:
                PHASEBH = 0;
                Channel0SetHighValue();
                break;
            case 6:
                PHASEAL = 0;
                PHASEBL = 1;
                break;
        }
    }
    else {
        if (Channel0.step <= 1)
            Channel0.step = 6;
        else
            Channel0.step--;
        
        switch(Channel0.step) {
            case 1:
                PHASECL = 0;
                PHASEBL = 1;
                break;
            case 2:
                PHASEBH = 0;
                Channel0SetHighValue();
                break;
            case 3:
                PHASEAL = 0;
                PHASECL = 1;
                break;
            case 4:
                PHASECH = 0;
                Channel0SetHighValue();
                break;
            case 5:
                PHASEBL = 0;
                PHASEAL = 1;
                break;
            case 6:
                PHASEAH = 0;
                Channel0SetHighValue();
                break;
        }
    }
    
    if (Channel0.isOneStep) {
        Channel0.isRunning = 0;
        Channel0.isOneStep = 0;
    }
}

void Channel0PWM_cb() {
    Channel0.pwmState += 1;
    
    switch (Channel0.pwmState) {
        case 1:
            Channel0SetHighValue();
            Channel0.pwmTimer.Missing = Channel0.pwmOnBeforeAdc;
            //ADCON0bits.GO_DONE = 1;
            switch (Channel0.step) {
                case 3:
                case 6:
                    //ADCON0 = 0b00000001;   // channel A - set the AN0 port and start the capacitor
                    break;
                case 2:
                case 5:
                    //ADCON0 = 0b00000001;   // channel B - set the AN0 port and start the capacitor
                    break;
                case 1:
                case 4:
                    //ADCON0 = 0b00000001;   // channel C - set the AN0 port and start the capacitor
                    break;
            }
            break;
        case 2:
            Channel0.pwmTimer.Missing = Channel0.pwmOnAfterAdc;
            
            //if (Channel0.step == 1 || Channel0.step == 3) {
            //ADCON0bits.GO_DONE = 1;
            //while (ADCON0bits.GO_DONE);
            
            //ADCON0 = 0b00000000;
            
            //unsigned char count = Channel0.adcValues[0];
            
            //if (count == 31) {
                //memset(Channel0.adcValues, 0, 64);
                //Channel0.adcValues[1] = ADRESL;
                //Channel0.adcValues[2] = ADRESH;
                //Channel0.adcValues[0] = 1;
            //}
            //else {
                //Channel0.adcValues[(count * 2) + 1] = ADRESL;
                //Channel0.adcValues[(count * 2) + 2] = ADRESH;
                //Channel0.adcValues[0] = ++count;
            //}
            //}
            break;
        case 3:
            Channel0SetHighValue();
            Channel0.pwmTimer.Missing = Channel0.pwmOff;
            Channel0.pwmState = 0;
            break;
    }
}

void Channel0SetHighValue() {
    switch(Channel0.step) {
        case 1:
        case 2:
            PHASEAH = Channel0.pwmState != 3;
            break;
        case 3:
        case 4:
            PHASEBH = Channel0.pwmState != 3;
            break;
        case 5:
        case 6:
            PHASECH = Channel0.pwmState != 3;
            break;
    }
}


*/


