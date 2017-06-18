using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public class ChannelManualStep : IMessageRequest
    {
        public int Index { get; set; }
        public uint StepTicks { get; set; }

        public byte[] GetBytes()
        {
            using (var mem = new MemoryStream())
            using (var binary = new BinaryWriter(mem))
            {
                binary.Write((byte)MessageIndexEnum.ChannelManualStep);

                binary.Write((byte)Index);

                binary.Write(StepTicks);

                return mem.ToArray();
            }
        }
    }
}