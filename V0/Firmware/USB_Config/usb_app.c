#include "usb.h"
#include "usb_device_hid.h"

#include "system.h"
#include "BLDC_Esc.h"

unsigned char ReceivedDataBuffer[64] @ HID_CUSTOM_OUT_DATA_BUFFER_ADDRESS;
unsigned char ToSendDataBuffer[64] @ HID_CUSTOM_IN_DATA_BUFFER_ADDRESS;

volatile USB_HANDLE USBOutHandle;    
volatile USB_HANDLE USBInHandle;

void APP_DeviceInitialize()
{
    //initialize the variable holding the handle for the last
    // transmission
    USBInHandle = 0;

    //enable the HID endpoint
    USBEnableEndpoint(CUSTOM_DEVICE_HID_EP, USB_IN_ENABLED|USB_OUT_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);

    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = (volatile USB_HANDLE)HIDRxPacket(CUSTOM_DEVICE_HID_EP,(uint8_t*)&ReceivedDataBuffer,64);
}

void APP_DeviceTasks()
{   
    //Check if we have received an OUT data packet from the host
    if(HIDRxHandleBusy(USBOutHandle) == false)
    {   
        //uwrite_array(0, (char*)&ReceivedDataBuffer);
        
        //We just received a packet of data from the USB host.
        //Check the first uint8_t of the packet to see what command the host
        //application software wants us to fulfill.
        switch(ReceivedDataBuffer[0])				//Look at the data the host sent, to see what kind of application specific command it sent.
        {
            case 1:
            {
                agoravai ^= 1;
                POWERON = agoravai;
                break;
            }
            case 2:
            {
                //ToSendDataBuffer[0] = BUTTON;
                //ADCON0bits.GO_DONE = 1;
                //while(ADCON0bits.GO_DONE != 0);
                //ToSendDataBuffer[0] = ADRESL;
                //ToSendDataBuffer[1] = ADRESH;
                unsigned char i = 0;
                for (; i < 64; i++) {
                    ToSendDataBuffer[i] = Channel0.adcValues[i];
                }
                memset(Channel0.adcValues, 0, 64);
                
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                break;
            }
            case 3:  // Channel0 step value change
            {
                union ULongConvertion tick;
                tick.bytes[0] = ReceivedDataBuffer[1];
                tick.bytes[1] = ReceivedDataBuffer[2];
                tick.bytes[2] = ReceivedDataBuffer[3];
                tick.bytes[3] = ReceivedDataBuffer[4];
                Channel0.stepTimer.Value = tick.value;
                break;
            }
            case 4:  // Channel0 pwm values change
            {
                union ULongConvertion tick;
                tick.bytes[0] = ReceivedDataBuffer[1];
                tick.bytes[1] = ReceivedDataBuffer[2];
                tick.bytes[2] = ReceivedDataBuffer[3];
                tick.bytes[3] = ReceivedDataBuffer[4];
                Channel0.pwmOnBeforeAdc = tick.value;
                tick.bytes[0] = ReceivedDataBuffer[5];
                tick.bytes[1] = ReceivedDataBuffer[6];
                tick.bytes[2] = ReceivedDataBuffer[7];
                tick.bytes[3] = ReceivedDataBuffer[8];
                Channel0.pwmOnAfterAdc = tick.value;
                tick.bytes[0] = ReceivedDataBuffer[9];
                tick.bytes[1] = ReceivedDataBuffer[10];
                tick.bytes[2] = ReceivedDataBuffer[11];
                tick.bytes[3] = ReceivedDataBuffer[12];
                Channel0.pwmOff = tick.value;
                break;
            }
            case 5:  // Channel0 isfoward change
            {
                Channel0.isFoward = ReceivedDataBuffer[1];
                break;
            }
            case 6:
            {
                break;
            }
            case 7:  // Channel0 isrunning change
            {
                Channel0.isRunning = ReceivedDataBuffer[1];
                if (Channel0.isRunning) {
                    Channel0.stepTimer.Missing = Channel0.stepTimer.Value;
                    Channel0.pwmTimer.Missing = Channel0.pwmTimer.Value;
                }
                break;
            }
            case 8:  // Channel0 one step
            {
                Channel0.isOneStep = Channel0.isRunning = 1;
                break;
            }
            case 10:   // Change the LedControl channel 0
            {
                //RED0on = ReceivedDataBuffer[1];
                //RED0on = ReceivedDataBuffer[2] << 8;
                //RED0off = ReceivedDataBuffer[3];
                //RED0off = ReceivedDataBuffer[4] << 8;
                //GREEN0on = ReceivedDataBuffer[5];
                //GREEN0on = ReceivedDataBuffer[6] << 8;
                //GREEN0off = ReceivedDataBuffer[7];
                //GREEN0off = ReceivedDataBuffer[8] << 8;
                //BLUE0on = ReceivedDataBuffer[9];
                //BLUE0on = ReceivedDataBuffer[10] << 8;
                //BLUE0off = ReceivedDataBuffer[11];
                //BLUE0off = ReceivedDataBuffer[12] << 8;
                //LedControl_Init();
                break;
            }
            //case COMMAND_TOGGLE_LED:  //Toggle LEDs command
                //LED_Toggle(LED_USB_DEVICE_HID_CUSTOM);
            //    break;
            //case COMMAND_GET_BUTTON_STATUS:  //Get push button state
                //Check to make sure the endpoint/buffer is free before we modify the contents
            //    if(!HIDTxHandleBusy(USBInHandle))
            //    {
            //        ToSendDataBuffer[0] = 0x81;				//Echo back to the host PC the command we are fulfilling in the first uint8_t.  In this case, the Get Pushbutton State command.
                    //if(BUTTON_IsPressed(BUTTON_USB_DEVICE_HID_CUSTOM) == false)	//pushbutton not pressed, pull up resistor on circuit board is pulling the PORT pin high
                    //{
                    //        ToSendDataBuffer[1] = 0x01;
                    //}
                    //else									//sw3 must be == 0, pushbutton is pressed and overpowering the pull up resistor
                    //{
                    //        ToSendDataBuffer[1] = 0x00;
                    //}
                    //Prepare the USB module to send the data packet to the host
            //        USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
            //    }
            //    break;

            //case COMMAND_READ_POTENTIOMETER:	//Read POT command.  Uses ADC to measure an analog voltage on one of the ANxx I/O pins, and returns the result to the host
            //    {
            //        uint16_t pot;

                    //Check to make sure the endpoint/buffer is free before we modify the contents
            //        if(!HIDTxHandleBusy(USBInHandle))
            //        {
                        //Use ADC to read the I/O pin voltage.  See the relevant HardwareProfile - xxxxx.h file for the I/O pin that it will measure.
                        //Some demo boards, like the PIC18F87J50 FS USB Plug-In Module board, do not have a potentiometer (when used stand alone).
                        //This function call will still measure the analog voltage on the I/O pin however.  To make the demo more interesting, it
                        //is suggested that an external adjustable analog voltage should be applied to this pin.

                        //pot = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);

            //            ToSendDataBuffer[0] = 0x37;  	//Echo back to the host the command we are fulfilling in the first uint8_t.  In this case, the Read POT (analog voltage) command.
            //            ToSendDataBuffer[1] = (uint8_t)pot; //LSB
            //            ToSendDataBuffer[2] = pot >> 8;     //MSB


                        //Prepare the USB module to send the data packet to the host
            //            USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
            //        }
            //    }
            //    break;
        }
        //Re-arm the OUT endpoint, so we can receive the next OUT data packet 
        //that the host may try to send us.
        USBOutHandle = HIDRxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ReceivedDataBuffer, 64);
    }
}

bool USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, uint16_t size)
{
    switch((int)event)
    {
        case EVENT_TRANSFER:
            break;

        case EVENT_SOF:
            /* We are using the SOF as a timer to time the LED indicator.  Call
             * the LED update function here. */
            //APP_LEDUpdateUSBStatus();
            break;

        case EVENT_SUSPEND:
            /* Update the LED status for the suspend event. */
            //APP_LEDUpdateUSBStatus();

            //Call the hardware platform specific handler for suspend events for
            //possible further action (like optionally going reconfiguring the application
            //for lower power states and going to sleep during the suspend event).  This
            //would normally be done in USB compliant bus powered applications, although
            //no further processing is needed for purely self powered applications that
            //don't consume power from the host.
            //SYSTEM_Initialize(SYSTEM_STATE_USB_SUSPEND);
            break;

        case EVENT_RESUME:
            /* Update the LED status for the resume event. */
            //APP_LEDUpdateUSBStatus();

            //Call the hardware platform specific resume from suspend handler (ex: to
            //restore I/O pins to higher power states if they were changed during the 
            //preceding SYSTEM_Initialize(SYSTEM_STATE_USB_SUSPEND) call at the start
            //of the suspend condition.
            //SYSTEM_Initialize(SYSTEM_STATE_USB_RESUME);
            break;

        case EVENT_CONFIGURED:
            /* When the device is configured, we can (re)initialize the demo
             * code. */
            APP_DeviceInitialize();
            break;

        case EVENT_SET_DESCRIPTOR:
            break;

        case EVENT_EP0_REQUEST:
            /* We have received a non-standard USB request.  The HID driver
             * needs to check to see if the request was for it. */
            USBCheckHIDRequest();
            break;

        case EVENT_BUS_ERROR:
            break;

        case EVENT_TRANSFER_TERMINATED:
            break;

        default:
            break;
    }
    return true;
}

void interrupt SYS_InterruptHigh(void)
{
    #if defined(USB_INTERRUPT)
        USBDeviceTasks();
    #endif
}