using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public class ChannelPWM : IMessageRequest
    {
        public int Index { get; set; }
        public ushort PwmOnBeforeAdc { get; set; }
        public ushort PwmOnAfterAdc { get; set; }
        public ushort PwmOff { get; set; }

        public byte[] GetBytes()
        {
            using (var mem = new MemoryStream())
            using (var binary = new BinaryWriter(mem))
            {
                binary.Write((byte)MessageIndexEnum.ChannelPWM);

                binary.Write((byte)Index);

                binary.Write(PwmOnBeforeAdc);

                binary.Write(PwmOnAfterAdc);

                binary.Write(PwmOff);

                return mem.ToArray();
            }
        }
    }
}