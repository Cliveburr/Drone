using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneDesigner.Measure
{
    public class AmperePerHour
    {
        public double Value { get; set; }
        public AmperePerHourUnit Unit { get; private set; }

        public AmperePerHour(double value = 0, AmperePerHourUnit unit = AmperePerHourUnit.AmperePerHour)
        {
            Value = value;
            Unit = unit;
        }

        public void ConvertToUnit(AmperePerHourUnit unit)
        {
            var inampereperhour = ConvertToAmperePerHour(this);
            Value = ConvertAmperePerHourToUnit(inampereperhour, unit);
            Unit = unit;
        }

        public Ampere GetAmpere()
        {
            var inampereperhour = ConvertToAmperePerHour(this);
            return new Ampere(inampereperhour);
        }

        public static double ConvertToAmperePerHour(AmperePerHour ampereperhour)
        {
            switch (ampereperhour.Unit)
            {
                case AmperePerHourUnit.AmperePerHour: return ampereperhour.Value;
                case AmperePerHourUnit.MiliAmperePerHour: return ampereperhour.Value * Math.Pow(10, -3);
                case AmperePerHourUnit.MicroAmperePerHour: return ampereperhour.Value * Math.Pow(10, -6);
                case AmperePerHourUnit.NanoAmperePerHour: return ampereperhour.Value * Math.Pow(10, -9);
                default: throw new Exception("Wrong AmperePerHour unit!");
            }
        }

        public double ConvertAmperePerHourToUnit(double value, AmperePerHourUnit unit)
        {
            switch (unit)
            {
                case AmperePerHourUnit.AmperePerHour: return value;
                case AmperePerHourUnit.MiliAmperePerHour: return value * Math.Pow(10, 3);
                case AmperePerHourUnit.MicroAmperePerHour: return value * Math.Pow(10, 6);
                case AmperePerHourUnit.NanoAmperePerHour: return value * Math.Pow(10, 9);
                default: throw new Exception("Wrong AmperePerHour unit!");
            }
        }

        public override string ToString()
        {
            switch (Unit)
            {
                case AmperePerHourUnit.AmperePerHour: return Value.ToString("#,##0.###") + "Ah";
                case AmperePerHourUnit.MiliAmperePerHour: return Value.ToString("#,##0.###") + "mAh";
                case AmperePerHourUnit.MicroAmperePerHour: return Value.ToString("#,##0.###") + "uAh";
                case AmperePerHourUnit.NanoAmperePerHour: return Value.ToString("#,##0.###") + "nAh";
                default: throw new Exception("Wrong Ampere unit!");
            }
        }

        public static TimeSpan operator/(AmperePerHour amperePerHour, Ampere ampere)
        {
            var inampereperhour = ConvertToAmperePerHour(amperePerHour);
            var inampere = Ampere.ConvertToAmpere(ampere);
            var hour = inampereperhour / inampere;
            return TimeSpan.FromHours(hour);
        }
    }

    public enum AmperePerHourUnit
    {
        AmperePerHour = 0,
        MiliAmperePerHour = 1,
        MicroAmperePerHour = 2,
        NanoAmperePerHour = 3
    }
}