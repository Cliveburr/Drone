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
            case 1:  // ChannelInfo
            {
                // parse request
                unsigned char index = ReceivedDataBuffer[1];
                
                // generate response
                struct ChannelStruct channel = BLDC_Esc_Channels[index];
                
                ToSendDataBuffer[0] = channel.mode;
                ToSendDataBuffer[1] = channel.state;
                
                ULong32Convertion.value = channel.stepTimer.value;
                ToSendDataBuffer[2] = ULong32Convertion.bytes[0];
                ToSendDataBuffer[3] = ULong32Convertion.bytes[1];
                ToSendDataBuffer[4] = ULong32Convertion.bytes[2];
                ToSendDataBuffer[5] = ULong32Convertion.bytes[3];

                UInt16Convertion.value = channel.pwmOnBeforeAdc;
                ToSendDataBuffer[6] = UInt16Convertion.bytes[0];
                ToSendDataBuffer[7] = UInt16Convertion.bytes[1];

                UInt16Convertion.value = channel.pwmOnAfterAdc;
                ToSendDataBuffer[8] = UInt16Convertion.bytes[0];
                ToSendDataBuffer[9] = UInt16Convertion.bytes[1];

                UInt16Convertion.value = channel.pwmOff;
                ToSendDataBuffer[10] = UInt16Convertion.bytes[0];
                ToSendDataBuffer[11] = UInt16Convertion.bytes[1];
                
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0], 64);
                
                break;
            }
            case 2:  // ChannelChangeMode
            {
                //ToSendDataBuffer[0] = BUTTON;
                //ADCON0bits.GO_DONE = 1;
                //while(ADCON0bits.GO_DONE != 0);
                //ToSendDataBuffer[0] = ADRESL;
                //ToSendDataBuffer[1] = ADRESH;
                //unsigned char i = 0;
                //for (; i < 64; i++) {
                //    ToSendDataBuffer[i] = Channel0.adcValues[i];
                //}
                //memset(Channel0.adcValues, 0, 64);
                //
                //USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0],64);
                
                // parse request
                unsigned char index = ReceivedDataBuffer[1];

                enum ChannelMode mode = ReceivedDataBuffer[2];

                // process the message
                switch (mode) {
                    case CM_Manual: BLDC_Esc_SetManual(index); break;
                    case CM_Automatic: BLDC_Esc_SetAutomatic(index); break;
                }
                
                // generate response
                ToSendDataBuffer[0] = BLDC_Esc_Channels[index].mode;
                ToSendDataBuffer[1] = BLDC_Esc_Channels[index].state;
                
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0], 64);
                
                break;
            }
            case 3:  // ChannelChangeState
            {
                //union ULongConvertion tick;
                //tick.bytes[0] = ReceivedDataBuffer[1];
                //tick.bytes[1] = ReceivedDataBuffer[2];
                //tick.bytes[2] = ReceivedDataBuffer[3];
                //tick.bytes[3] = ReceivedDataBuffer[4];
                //Channel0.stepTimer.Value = tick.value;

                // parse request
                unsigned char index = ReceivedDataBuffer[1];

                enum ChannelState state = ReceivedDataBuffer[2];

                // process the message
                switch (state) {
                    case CS_ManualOff: BLDC_Esc_SetManualOff(index); break;
                    case CS_ManualOn: BLDC_Esc_SetManualOn(index); break;
                    case CS_AutomaticOff: BLDC_Esc_SetAutomaticOff(index); break;
                    case CS_AutomaticOn: BLDC_Esc_SetAutomaticOn(index); break;
                }

                // generate response
                ToSendDataBuffer[0] = BLDC_Esc_Channels[index].mode;
                ToSendDataBuffer[1] = BLDC_Esc_Channels[index].state;
                
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0], 64);

                break;
            }
            case 4:  // ChannelManualConfig
            {
                // parse request
                unsigned char index = ReceivedDataBuffer[1];
                
                unsigned char direction = ReceivedDataBuffer[2];
                unsigned char oneStep = ReceivedDataBuffer[3];
                
                // process the message
                BLDC_Esc_SetManualConfig(index, direction, oneStep);

                break;
            }
            case 5:  // ChannelManualStep
            {
                // parse request
                unsigned char index = ReceivedDataBuffer[1];
                
                ULong32Convertion.bytes[0] = ReceivedDataBuffer[2];
                ULong32Convertion.bytes[1] = ReceivedDataBuffer[3];
                ULong32Convertion.bytes[2] = ReceivedDataBuffer[4];
                ULong32Convertion.bytes[3] = ReceivedDataBuffer[5];
                
                // process the message
                BLDC_Esc_SetManualStep(index, ULong32Convertion.value);
                
                break;
            }
            case 6:  // ChannelPWM
            {
                // parse request
                unsigned char index = ReceivedDataBuffer[1];

                UInt16Convertion.bytes[0] = ReceivedDataBuffer[2];
                UInt16Convertion.bytes[1] = ReceivedDataBuffer[3];
                unsigned int pwmOnBeforeAdc = UInt16Convertion.value;
                
                UInt16Convertion.bytes[0] = ReceivedDataBuffer[4];
                UInt16Convertion.bytes[1] = ReceivedDataBuffer[5];
                unsigned int pwmOnAfterAdc = UInt16Convertion.value;

                UInt16Convertion.bytes[0] = ReceivedDataBuffer[6];
                UInt16Convertion.bytes[1] = ReceivedDataBuffer[7];
                unsigned int pwmOff = UInt16Convertion.value;

                // process the message
                BLDC_Esc_SetPWM(index, pwmOnBeforeAdc, pwmOnAfterAdc, pwmOff);
                
                //union ULongConvertion tick;
                //tick.bytes[0] = ReceivedDataBuffer[1];
                //tick.bytes[1] = ReceivedDataBuffer[2];
                //tick.bytes[2] = ReceivedDataBuffer[3];
                //tick.bytes[3] = ReceivedDataBuffer[4];
                //Channel0.pwmOnBeforeAdc = tick.value;
                //tick.bytes[0] = ReceivedDataBuffer[5];
                //tick.bytes[1] = ReceivedDataBuffer[6];
                //tick.bytes[2] = ReceivedDataBuffer[7];
                //tick.bytes[3] = ReceivedDataBuffer[8];
                //Channel0.pwmOnAfterAdc = tick.value;
                //tick.bytes[0] = ReceivedDataBuffer[9];
                //tick.bytes[1] = ReceivedDataBuffer[10];
                //tick.bytes[2] = ReceivedDataBuffer[11];
                //tick.bytes[3] = ReceivedDataBuffer[12];
                //Channel0.pwmOff = tick.value;
                break;
            }
            case 7:  // ConfigStartStopCurve
            {
                // parse request
                ULong32Convertion.bytes[0] = ReceivedDataBuffer[1];
                ULong32Convertion.bytes[1] = ReceivedDataBuffer[2];
                ULong32Convertion.bytes[2] = ReceivedDataBuffer[3];
                ULong32Convertion.bytes[3] = ReceivedDataBuffer[4];
                unsigned long beginValue = ULong32Convertion.value;

                UInt16Convertion.bytes[0] = ReceivedDataBuffer[5];
                UInt16Convertion.bytes[1] = ReceivedDataBuffer[6];
                unsigned int endValue = UInt16Convertion.value;
                
                unsigned char incValue = ReceivedDataBuffer[7];

                UInt16Convertion.bytes[0] = ReceivedDataBuffer[8];
                UInt16Convertion.bytes[1] = ReceivedDataBuffer[9];
                unsigned int clockValue = UInt16Convertion.value;

                // process the message
                BLDC_Esc_ConfigStartStopCurve(beginValue, endValue, incValue, clockValue);

                break;
            }
            case 8:  // Channel0 one step
            {
                
                BLDC_Esc_SetAltSpeed(0, 10, 0.99);
                
                break;
            }
            case 9: // ChannelStepCounting
            {
                // parse request
                unsigned char index = ReceivedDataBuffer[1];

                // generate response
                UInt16Convertion.value = BLDC_Esc_Channels[index].stepCountingVar;
                ToSendDataBuffer[0] = UInt16Convertion.bytes[0];
                ToSendDataBuffer[1] = UInt16Convertion.bytes[1];
                
                USBInHandle = HIDTxPacket(CUSTOM_DEVICE_HID_EP, (uint8_t*)&ToSendDataBuffer[0], 64);

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