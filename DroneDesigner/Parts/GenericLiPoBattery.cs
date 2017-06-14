using DroneDesigner.Measure;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneDesigner.Parts
{
    public class GenericLiPoBattery
    {
        public string Id { get; private set; }
        public string Name { get; set; }
        public AmperePerHour Capacity { get; set; }
        public ushort CellCount { get; set; }
        public ushort NominalDischargeRate { get; set; }

        public GenericLiPoBattery(string name)
        {
            Id = "GenericLiPoBattery";
            Name = name;
        }

        public Voltage Voltage
        {
            get
            {
                return new Voltage(CellCount * 3.7);
            }
        }

        public Ampere MaxMiliAmpere
        {
            get
            {
                return Capacity.GetAmpere() * NominalDischargeRate;
            }
        }

        public TimeSpan FullDischargeAt(Ampere ampere)
        {
            return Capacity / ampere;
        }
    }
}