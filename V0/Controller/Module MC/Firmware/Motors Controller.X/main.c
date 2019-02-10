#include "mcc_generated_files/mcc.h"


void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    

    while (1)
    {
        // Add your application code
        
        if (EUSART_DataReady)
        {
            uint8_t data = EUSART_Read();
            if (data == 65)
            {
                data = EUSART_Read();
                if (data == 66)
                {
                    EUSART_Write(67);
                }
            }
            else if (data == 66)
            {
                EUSART_Write(65);
            }
        }
        
    }
}
/**
 End of File
*/