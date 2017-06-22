using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MetricLibrary
{
    public class Period
    {
        public double Value { get; set; }
        public PeriodUnit Unit { get; private set; }

        public Period(double value = 0, PeriodUnit unit = PeriodUnit.Seconds)
        {
            Value = value;
            Unit = unit;
        }

        public void ConvertToUnit(PeriodUnit unit)
        {
            var inhertz = GetInSeconds(this);
            Value = GetSecondsInUnit(inhertz, unit);
            Unit = unit;
        }

        public static double GetInSeconds(Period period)
        {
            switch (period.Unit)
            {
                case PeriodUnit.PicoSeconds: return period.Value * Math.Pow(10, -12);
                case PeriodUnit.NanoSeconds: return period.Value * Math.Pow(10, -9);
                case PeriodUnit.MicroSeconds: return period.Value * Math.Pow(10, -6);
                case PeriodUnit.MiliSeconds: return period.Value * Math.Pow(10, -3);
                case PeriodUnit.Seconds: return period.Value;
                default: throw new Exception("Wrong Period unit!");
            }
        }

        public static double GetSecondsInUnit(double value, PeriodUnit unit)
        {
            switch (unit)
            {
                case PeriodUnit.PicoSeconds: return value * Math.Pow(10, 12);
                case PeriodUnit.NanoSeconds: return value * Math.Pow(10, 9);
                case PeriodUnit.MicroSeconds: return value * Math.Pow(10, 6);
                case PeriodUnit.MiliSeconds: return value * Math.Pow(10, 3);
                case PeriodUnit.Seconds: return value;
                default: throw new Exception("Wrong Period unit!");
            }
        }

        public override string ToString()
        {
            switch (Unit)
            {
                case PeriodUnit.PicoSeconds: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "ps";
                case PeriodUnit.NanoSeconds: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "ns";
                case PeriodUnit.MicroSeconds: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "us";
                case PeriodUnit.MiliSeconds: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "ms";
                case PeriodUnit.Seconds: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "s";
                default: throw new Exception("Wrong Period unit!");
            }
        }
    }

    public enum PeriodUnit
    {
        PicoSeconds = 0,
        NanoSeconds = 1,
        MicroSeconds = 2,
        MiliSeconds = 3,
        Seconds = 4
    }
}