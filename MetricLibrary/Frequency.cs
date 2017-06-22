using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MetricLibrary
{
    public class Frequency
    {
        public double Value { get; set; }
        public FrequencyUnit Unit { get; private set; }

        public Frequency(double value = 0, FrequencyUnit unit = FrequencyUnit.Hertz)
        {
            Value = value;
            Unit = unit;
        }

        public void ConvertToUnit(FrequencyUnit unit)
        {
            var inhertz = GetInHertz(this);
            Value = GetHertzInUnit(inhertz, unit);
            Unit = unit;
        }

        public static double GetInHertz(Frequency frequency)
        {
            switch (frequency.Unit)
            {
                case FrequencyUnit.Hertz: return frequency.Value;
                case FrequencyUnit.QuiloHertz: return frequency.Value * Math.Pow(10, 3);
                case FrequencyUnit.MegaHertz: return frequency.Value * Math.Pow(10, 6);
                case FrequencyUnit.GigaHertz: return frequency.Value * Math.Pow(10, 9);
                default: throw new Exception("Wrong Frequency unit!");
            }
        }

        public static double GetHertzInUnit(double value, FrequencyUnit unit)
        {
            switch (unit)
            {
                case FrequencyUnit.Hertz: return value;
                case FrequencyUnit.QuiloHertz: return value * Math.Pow(10, -3);
                case FrequencyUnit.MegaHertz: return value * Math.Pow(10, -6);
                case FrequencyUnit.GigaHertz: return value * Math.Pow(10, -9);
                default: throw new Exception("Wrong Frequency unit!");
            }
        }

        public override string ToString()
        {
            switch (Unit)
            {
                case FrequencyUnit.Hertz: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "Hz";
                case FrequencyUnit.QuiloHertz: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "KHz";
                case FrequencyUnit.MegaHertz: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "MHz";
                case FrequencyUnit.GigaHertz: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "GHz";
                default: throw new Exception("Wrong Frequency unit!");
            }
        }
    }

    public enum FrequencyUnit
    {
        Hertz = 0,
        QuiloHertz = 1,
        MegaHertz = 2,
        GigaHertz = 3
    }
}