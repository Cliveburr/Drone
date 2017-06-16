using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor
{
    public enum ChannelStateEnum : byte
    {
        CS_ManualOff = 0,
        CS_ManualOn = 1,
        CS_Automatic_Off = 2,
        CS_AutomaticStarting = 3,
        CS_AutomaticRunning = 4,
        CS_AutomaticStoping = 5
    }
}