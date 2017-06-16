using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public class ChannelChangeModeRequest : IMessageRequest
    {
        public int Index { get; set; }
        public ChannelModeEnum Mode { get; set; }

        public byte[] GetBytes()
        {
            using (var mem = new MemoryStream())
            using (var binary = new BinaryWriter(mem))
            {
                binary.Write((byte)MessageIndexEnum.ChannelChangeMode);

                binary.Write((byte)Index);

                binary.Write((byte)Mode);

                return mem.ToArray();
            }
        }
    }

    public class ChannelChangeModeResponse : IMessageResponse
    {
        public ChannelModeEnum ChannelMode { get; set; }

        public void Parse(byte[] msg)
        {
            using (var mem = new MemoryStream(msg))
            using (var binary = new BinaryReader(mem))
            {
                ChannelMode = (ChannelModeEnum)binary.ReadByte();
            }
        }
    }
}