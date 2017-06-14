using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App
{
    public class VirtualMotor
    {
        public float Position { get; private set; }
        public int Steps { get; private set; }
        public int Step { get; private set; }

        public VirtualMotor(int steps)
        {
            Steps = steps;
            Step = 0;
        }

        public void Comutate()
        {
            Step++;
            if (Step >= Steps)
                Step = 0;


        }
    }
}