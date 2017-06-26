using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public class ChannelStepCountingRequest : IMessageRequest
    {
        public int Index { get; set; }

        public byte[] GetBytes()
        {
            using (var mem = new MemoryStream())
            using (var binary = new BinaryWriter(mem))
            {
                binary.Write((byte)MessageIndexEnum.ChannelStepCounting);

                binary.Write((byte)Index);

                return mem.ToArray();
            }
        }
    }

    public class ChannelStepCountingResponse : IMessageResponse
    {
        public ushort StepCounting { get; set; }

        public void Parse(byte[] msg)
        {
            using (var mem = new MemoryStream(msg))
            using (var binary = new BinaryReader(mem))
            {
                StepCounting = binary.ReadUInt16();
            }
        }
    }
}