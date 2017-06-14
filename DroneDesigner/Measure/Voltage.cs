using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneDesigner.Measure
{
    public class Voltage
    {
        public double Value { get; set; }
        public VoltageUnit Unit { get; private set; }

        public Voltage(double value = 0, VoltageUnit unit = VoltageUnit.Volts)
        {
            Value = value;
            Unit = unit;
        }

        public void ConvertToUnit(VoltageUnit unit)
        {
            var involtz = ConvertToVolts(this);
            Value = ConvertVoltsToUnit(involtz, unit);
            Unit = unit;
        }

        public static double ConvertToVolts(Voltage voltage)
        {
            switch (voltage.Unit)
            {
                case VoltageUnit.Volts: return voltage.Value;
                case VoltageUnit.MiliVolts: return voltage.Value * Math.Pow(10, -3);
                case VoltageUnit.MicroVolts: return voltage.Value * Math.Pow(10, -6);
                case VoltageUnit.NanoVolts: return voltage.Value * Math.Pow(10, -9);
                default: throw new Exception("Wrong Voltz unit!");
            }
        }

        public double ConvertVoltsToUnit(double value, VoltageUnit unit)
        {
            switch (unit)
            {
                case VoltageUnit.Volts: return value;
                case VoltageUnit.MiliVolts: return value * Math.Pow(10, 3);
                case VoltageUnit.MicroVolts: return value * Math.Pow(10, 6);
                case VoltageUnit.NanoVolts: return value * Math.Pow(10, 9);
                default: throw new Exception("Wrong Voltz unit!");
            }
        }

        public override string ToString()
        {
            switch (Unit)
            {
                case VoltageUnit.Volts: return Value.ToString("#,##0.###") + "V";
                case VoltageUnit.MiliVolts: return Value.ToString("#,##0.###") + "mV";
                case VoltageUnit.MicroVolts: return Value.ToString("#,##0.###") + "uV";
                case VoltageUnit.NanoVolts: return Value.ToString("#,##0.###") + "nV";
                default: throw new Exception("Wrong Voltz unit!");
            }
        }
    }

    public enum VoltageUnit
    {
        Volts = 0,
        MiliVolts = 1,
        MicroVolts = 2,
        NanoVolts = 3
    }
}