#include <xc.h>
#include "system.h"
//#include "system_config.h"
//#include "usb.h"

//#include <p18f4550.h>

#include "BLDC_Esc.h"

// CONFIG1L
#pragma config PLLDIV = 5       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC2_PLL3// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
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
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
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

struct TimerEventStruct Channel0PWM;
void test1_cb();
void Channel0PWM_cb();

void SYSTEM_Initialize()
{
    //ANSEL = 0b00000000;   // Set port as digital or analogic, 0 digital, 1 analogic
    
    // bit 0-3  - A/D Port Configuration Control bits   (1110 = Only AN0 is analog)
    // bit 4    - Voltage Reference Configuration bit (1 = VREF+, 0 = VDD)
    // bit 5    - Voltage Reference Configuration bit (1 = VREF-, 0 = VSS)
    // bit 6-7  - Unimplemented
    ADCON1 = 0b00001110;
    
    // bit 7    - A/D Result Format Select bit (1 = Right justified, 0 = Left justified)
    // bit 6    - Unimplemented
    // bit 3-5  - A/D Acquisition Time Select bits (001 = 2 TAD)
    // bit 0-2  - A/D Conversion Clock Select bits (000 =  FOSC/2)
    ADCON2 = 0b10001000;

    // bit 0    - A/D On bit
    // bit 1    - Go/Done 1 = progress 0 = idle
    // bit 2-5  - Analog Channel Select bits   (0000 = AN0)
    // bit 6-7  - Unimplemented
    ADCON0 = 0b00000001;
    
    INTCON = 0b00000000;
    //TRISIO = 0b00000000;  // Set port as input or output, 0 output, 1 input
    //GPIO = 0b00000000;    // Set value for ports
    
    TRISA = 0b0000001;
    TRISB = 0b0000000;
    TRISC = 0b0000110;
    TRISD = 0b0000001;
    
    PORTA = 0b0000001;
    PORTB = 0b0000000;
    PORTC = 0b0000000;
    PORTD = 0b0000000;
    
    // bit 0-2   - Prescaler Select bits (111 = 1:256, 110 = 1:128, 101 = 1:64, 100 = 1:32, 011 = 1:16, 010 = 1:8, 001 = 1:4, 000 = 1:2)
    // bit 3     - Prescaler Assignment bit (1 = TImer0 prescaler is NOT assigned, 0 = Timer0 prescaler is assigned)
    // bit 4     - Source Edge Select bit (1 = Increment on high-to-low, 0 = Increment on low-to-high)
    // bit 5     - Clock Source Select bit (1 = Transition on T0CKI pin, 0 = Internal instruction cycle clock)
    // bit 6     - Timer0 8-Bit/16-Bit Control bit (1 = Timer0 is configured as an 8-bit, 0 = Timer0 is configured as a 16-bit)
    // bit 7     - Timer0 On/Off Control bit (1 = Enabled, 0 = Disabled)
    T0CON = 0b10001011;
    
    //test1.Value = 46875;
    test1.Missing = test1.Value = 6000004; //144000;
    test1.Callback = test1_cb;
    
    Channel0PWM.Missing = Channel0PWM.Value = 0;
    Channel0PWM.Callback = Channel0PWM_cb;
}

// 65,535
// 48Mhz = 48.000.000hz / 4 = 12.000.000hz = 83,33ns
// Preescale 256 * 83,33ns = 21.333,33ns = 21,33us
// Preescale 128 * 83,33ns = 10.666,66ns = 10,66us
// Preescale 16 * 83,33ns = 1.333,33ns = 1,333us
// Timer0 8-bit = 256 * 21,33us = 35.460,48us = 35,46ms
// Timer0 16-bit - Preescale 128 - Maximum idle time = 65535 * 10,66us = 698.603,10us = 698,60ms

// 128 preescale 1s = 1.000.000us / 10,66us = 93750
// 16 preescale 1s = 1.000.000us / 1,33us = 750000
// 0 preescale 12000004

// 500prm / 60 =  8,333hz = 0,12s = 120ms
// 120ms = 120.000us / 0,08333us = 144000

// 10khz = 0,0001s = 0,1ms = 100us
// 16pre = 100us / 1,33us = 75
// 0pre = 100us / 0,08333us = 1200
void Channel0SetHighValue();

char step = 0;

void SYSTEM_Task() {
    
    TimerEvent_Tick();
    
    TimerEvent_Check(&test1);
    
    TimerEvent_Check(&Channel0PWM);
}


void test1_cb() {
    POWERON ^= 1;
    
    if (foward) {
        if (step == 6)
            step = 1;
        else
            step++;
    }
    else {
        if (step <= 1)
            step = 6;
        else
            step--;
    }
    
    
    switch(step) {
        case 1:
        {
            //PORTD = 0b00100100;
            //PORTD = 0b00100100; // test 2
            
            PHASECH = 0;
            Channel0SetHighValue();  //PHASEAH = 1;
            //*PHASEBL = 1;
            break;
        }
        case 2:
        {
            //PORTD = 0b00011000;
            //PORTD = 0b10000100; // test 2
            
            PHASEBL = 0;
            //*PHASEAH = 1;
            PHASECL = 1;
            break;
        }
        case 3:
        {
            //PORTD = 0b10010000;
            //PORTD = 0b10010000; // test 2
            
            PHASEAH = 0;
            Channel0SetHighValue();  //PHASEBH = 1;
            //*PHASECL = 1;
            break;
        }
        case 4:
        {
            //PORTD = 0b01100000;
            //PORTD = 0b00011000; // test 2
            
            PHASECL = 0;
            //*PHASEBH = 1;
            PHASEAL = 1;
            break;
        }
        case 5:
        {
            //PORTD = 0b01001000;
            //PORTD = 0b01001000; // test 2
            
            PHASEBH = 0;
            Channel0SetHighValue();  //PHASECH = 1;
            //*PHASEAL = 1;
            break;
        }
        case 6:
        {
            //PORTD = 0b10000100;
            //PORTD = 0b01100000; // test 2
            
            PHASEAL = 0;
            //*PHASECH = 1;
            PHASEBL = 1;
            break;
        }
    }
}


unsigned char Channel0PWNState = 0;

void Channel0PWM_cb() {
    if (Channel0PWNState) {
        Channel0PWM.Missing = Channel0PWN_Off;
    }
    else {
        Channel0PWM.Missing = Channel0PWN_On;
    }
    Channel0PWNState ^= 1;
    Channel0SetHighValue();
}

void Channel0SetHighValue() {
    switch(step) {
        case 1:
        case 2:
            PHASEAH = Channel0PWNState;
            break;
        case 3:
        case 4:
            PHASEBH = Channel0PWNState;
            break;
        case 5:
        case 6:
            PHASECH = Channel0PWNState;
            break;
    }
}