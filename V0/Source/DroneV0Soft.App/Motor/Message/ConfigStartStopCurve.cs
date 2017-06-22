using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public class ConfigStartStopCurve : IMessageRequest
    {
        public ushort BeginValue { get; set; }
        public ushort EndValue { get; set; }
        public byte IncValue { get; set; }
        public ushort ClockValue { get; set; }

        public byte[] GetBytes()
        {
            using (var mem = new MemoryStream())
            using (var binary = new BinaryWriter(mem))
            {
                binary.Write((byte)MessageIndexEnum.ConfigStartStopCurve);

                binary.Write(BeginValue);

                binary.Write(EndValue);

                binary.Write(IncValue);

                binary.Write(ClockValue);

                return mem.ToArray();
            }
        }
    }
}