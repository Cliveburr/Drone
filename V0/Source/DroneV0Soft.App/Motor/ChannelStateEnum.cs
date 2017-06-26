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
        CS_AutomaticOff = 2,
        CS_AutomaticOn = 3
    }
}