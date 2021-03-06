#include "system.h"
#include <system_config.h>

#include "usb.h"
#include "usb_device_hid.h"

#include "usb_app.h"

void t_delay_10ms(int n);

void main(void) {
    SYSTEM_Initialize();

    POWERON = 1;
    t_delay_10ms(100);
    POWERON = 0;

    USBDeviceInit();

    //while(1)
    //{
        //LATBbits.LATB2 = PORTBbits.RB2 ^ 1;
        //POWERON ^= 1;
    //    t_delay_10ms(10);
    //    POWERON = C1OUT;
    //}
    
    while(1)
    {
        if (USBGETSIGNAL)
        {
            USBSTATE = 1;
            USBDeviceAttach();
            
            while (USBGETSIGNAL)
            {
                //char tt = USBGetDeviceState();
                
                if (USBGetDeviceState() >= CONFIGURED_STATE && !USBIsDeviceSuspended())
                {
                    APP_DeviceTasks();
                }
                
                SYSTEM_Task();
            }
            
            USBDeviceDetach();
            USBSTATE = 0;
        }
        
        SYSTEM_Task();
    }   
}

void t_delay_10ms(int n)
{
    while(n--)
    {
        __delaywdt_ms(10);
    }
}