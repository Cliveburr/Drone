using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public class ChannelInfoRequest : IMessageRequest
    {
        public int Index { get; set; }

        public byte[] GetBytes()
        {
            using (var mem = new MemoryStream())
            using (var binary = new BinaryWriter(mem))
            {
                binary.Write((byte)MessageIndexEnum.ChannelInfo);

                binary.Write((byte)Index);

                return mem.ToArray();
            }
        }
    }

    public class ChannelInfoResponse : IMessageResponse
    {
        public ChannelModeEnum Mode { get; set; }
        public ChannelStateEnum State { get; set; }
        public uint StepTimerValue { get; set; }
        public ushort PwmOnBeforeAdc { get; set; }
        public ushort PwmOnAfterAdc { get; set; }
        public ushort PwmOff { get; set; }

        public void Parse(byte[] msg)
        {
            using (var mem = new MemoryStream(msg))
            using (var binary = new BinaryReader(mem))
            {
                Mode = (ChannelModeEnum)binary.ReadByte();

                State = (ChannelStateEnum)binary.ReadByte();

                StepTimerValue = binary.ReadUInt32();

                PwmOnBeforeAdc = binary.ReadUInt16();

                PwmOnAfterAdc = binary.ReadUInt16();

                PwmOff = binary.ReadUInt16();
            }
        }
    }
}