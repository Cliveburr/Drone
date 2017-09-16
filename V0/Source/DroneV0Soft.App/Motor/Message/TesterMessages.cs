using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor.Message
{
    public class TesterMessages
    {
        public class CrossZeroPortSelectRequest : IMessageRequest
        {
            public char Step { get; set; }

            public byte[] GetBytes()
            {
                using (var mem = new MemoryStream())
                using (var binary = new BinaryWriter(mem))
                {
                    binary.Write((byte)10);

                    binary.Write((byte)Step);

                    return mem.ToArray();
                }
            }
        }

        public class CrossZeroDetectRequest : IMessageRequest
        {
            public char Step { get; set; }

            public byte[] GetBytes()
            {
                using (var mem = new MemoryStream())
                using (var binary = new BinaryWriter(mem))
                {
                    binary.Write((byte)11);

                    binary.Write((byte)Step);

                    return mem.ToArray();
                }
            }
        }

        public class CrossZeroDetectResponse : IMessageResponse
        {
            public char CrossZero { get; set; }

            public void Parse(byte[] msg)
            {
                using (var mem = new MemoryStream(msg))
                using (var binary = new BinaryReader(mem))
                {
                    CrossZero = binary.ReadChar();
                }
            }
        }
    }
}