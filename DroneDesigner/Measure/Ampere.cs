using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneDesigner.Measure
{
    public class Ampere
    {
        public double Value { get; set; }
        public AmpereUnit Unit { get; private set; }

        public Ampere(double value = 0, AmpereUnit unit = AmpereUnit.Ampere)
        {
            Value = value;
            Unit = unit;
        }

        public void ConvertToUnit(AmpereUnit unit)
        {
            var inampere = ConvertToAmpere(this);
            Value = ConvertAmpereToUnit(inampere, unit);
            Unit = unit;
        }

        public static double ConvertToAmpere(Ampere ampere)
        {
            switch (ampere.Unit)
            {
                case AmpereUnit.Ampere: return ampere.Value;
                case AmpereUnit.MiliAmpere: return ampere.Value * Math.Pow(10, -3);
                case AmpereUnit.MicroAmpere: return ampere.Value * Math.Pow(10, -6);
                case AmpereUnit.NanoAmpere: return ampere.Value * Math.Pow(10, -9);
                default: throw new Exception("Wrong Ampere unit!");
            }
        }

        public double ConvertAmpereToUnit(double value, AmpereUnit unit)
        {
            switch (unit)
            {
                case AmpereUnit.Ampere: return value;
                case AmpereUnit.MiliAmpere: return value * Math.Pow(10, 3);
                case AmpereUnit.MicroAmpere: return value * Math.Pow(10, 6);
                case AmpereUnit.NanoAmpere: return value * Math.Pow(10, 9);
                default: throw new Exception("Wrong Ampere unit!");
            }
        }

        public override string ToString()
        {
            switch (Unit)
            {
                case AmpereUnit.Ampere: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "A";
                case AmpereUnit.MiliAmpere: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "mA";
                case AmpereUnit.MicroAmpere: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "uA";
                case AmpereUnit.NanoAmpere: return Value.ToString("#,##0.###", CultureInfo.InvariantCulture) + "nA";
                default: throw new Exception("Wrong Ampere unit!");
            }
        }

        public static Ampere operator*(Ampere ampere, double value)
        {
            return new Ampere(ampere.Value * value, ampere.Unit);
        }
    }

    public enum AmpereUnit
    {
        Ampere = 0,
        MiliAmpere = 1,
        MicroAmpere = 2,
        NanoAmpere = 3
    }
}