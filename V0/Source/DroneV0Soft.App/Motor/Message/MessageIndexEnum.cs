using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public enum MessageIndexEnum : byte
    {
        ChannelInfo = 1,
        ChannelChangeMode = 2,
        ChannelChangeState = 3,
        ChannelManualConfig = 4,
        ChannelManualStep = 5,
        ChannelManualPWM = 6,
        ConfigStartStopCurve = 7
    }
}