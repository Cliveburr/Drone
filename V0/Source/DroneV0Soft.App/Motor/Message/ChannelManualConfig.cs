using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public class ChannelManualConfig : IMessageRequest
    {
        public int Index { get; set; }
        public byte Direction { get; set; }
        public byte OneStep { get; set; }

        public byte[] GetBytes()
        {
            using (var mem = new MemoryStream())
            using (var binary = new BinaryWriter(mem))
            {
                binary.Write((byte)MessageIndexEnum.ChannelManualConfig);

                binary.Write((byte)Index);

                binary.Write(Direction);

                binary.Write(OneStep);

                return mem.ToArray();
            }
        }
    }
}