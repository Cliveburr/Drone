using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public class ChannelChangeStateRequest : IMessageRequest
    {
        public int Index { get; set; }
        public ChannelStateEnum State { get; set; }

        public byte[] GetBytes()
        {
            using (var mem = new MemoryStream())
            using (var binary = new BinaryWriter(mem))
            {
                binary.Write((byte)MessageIndexEnum.ChannelChangeState);

                binary.Write((byte)Index);

                binary.Write((byte)State);

                return mem.ToArray();
            }
        }
    }

    public class ChannelChangeStateResponse : IMessageResponse
    {
        public ChannelStateEnum ChannelState { get; set; }

        public void Parse(byte[] msg)
        {
            using (var mem = new MemoryStream(msg))
            using (var binary = new BinaryReader(mem))
            {
                ChannelState = (ChannelStateEnum)binary.ReadByte();
            }
        }
    }
}