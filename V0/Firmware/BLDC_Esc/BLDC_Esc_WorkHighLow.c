
void Channel0Step_cb_HighLowStarting() {
    Channel0SetValue_hard(0);
    


    
    if (Channel0.isFoward) {
        if (Channel0.step == 6) {
            Channel0.step = 1;

            Channel0.stepTimer.Value -= Channel0.automaticStartInc;
        }
        else
            Channel0.step++;
    }
    else {
        if (Channel0.step <= 1)
            Channel0.step = 6;
        else
            Channel0.step--;
    }

    Channel0SetValue_hard(Channel0.pwmState != 3);
    
    if (Channel0.isOneStep) {
        Channel0.isRunning = 0;
        Channel0.isOneStep = 0;
    }
}


void Channel0Step_cb_hard() {
    Channel0SetValue_hard(0);
    
    if (!Channel0.isRunning)
        return;
    
    if (Channel0.isFoward) {
        if (Channel0.step == 6)
            Channel0.step = 1;
        else
            Channel0.step++;
    }
    else {
        if (Channel0.step <= 1)
            Channel0.step = 6;
        else
            Channel0.step--;
    }

    Channel0SetValue_hard(Channel0.pwmState != 3);
    
    if (Channel0.isOneStep) {
        Channel0.isRunning = 0;
        Channel0.isOneStep = 0;
    }
}

void Channel0PWM_cb_hard() {
    Channel0.pwmState += 1;
    
    switch (Channel0.pwmState) {
        case 1:  // start the pwm high part
            Channel0SetValue_hard(1);
            //Channel0.pwmTimer.Missing = Channel0.pwmOnBeforeAdc;
            Channel0.pwmTimer.Value = Channel0.pwmOnAfterAdc;
            break;
        case 2:  // position to start the aquisition
            //Channel0.pwmTimer.Missing = Channel0.pwmOnAfterAdc;
            Channel0.pwmTimer.Value = Channel0.pwmOff;
            break;
        case 3:  // start the pwm low part
            Channel0SetValue_hard(0);
            //Channel0.pwmTimer.Missing = Channel0.pwmOff;
            Channel0.pwmTimer.Value = Channel0.pwmOnBeforeAdc;
            Channel0.pwmState = 0;
            break;
    }
}

void Channel0SetValue_hard(unsigned char value) {
    switch(Channel0.step) {
        case 1:
            PHASEAH = value;
            PHASEBL = value;
            break;
        case 2:
            PHASEAH = value;
            PHASECL = value;
            break;
        case 3:
            PHASEBH = value;
            PHASECL = value;
            break;
        case 4:
            PHASEBH = value;
            PHASEAL = value;
            break;
        case 5:
            PHASECH = value;
            PHASEAL = value;
            break;
        case 6:
            PHASECH = value;
            PHASEBL = value;
            break;
    }
}