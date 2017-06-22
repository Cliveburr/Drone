using MetricLibrary;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace DroneV0Soft.App.Windows
{
    /// <summary>
    /// Interaction logic for ConfigurationWindow.xaml
    /// </summary>
    public partial class ConfigurationWindow : Window
    {
        public ConfigurationWindow()
        {
            InitializeComponent();

            fcClock.SetFrequency(Program.Motor.ClockFrequency);
            tcSteps.SetText(Program.Motor.Steps.ToString());
        }

        private void CalculateStopStartCurve()
        {
            var clock = Frequency.GetInHertz(Program.Motor.ClockFrequency);
            var cicleclock = clock / 4;
            var cicleperiod = 1 / cicleclock;

            var rpmbegin = float.Parse(tbSTCrpmstart.Text);
            var rpmend = float.Parse(tbSTCrpmtarget.Text);
            var time = Period.GetInSeconds(pcSTCtime.GetPeriod());
            var frequency = Frequency.GetInHertz(fcSTCfrequency.GetFrequency());
            var steps = Program.Motor.Steps;

            var frequencyperiod = 1 / frequency;
            var frquencyticks = frequencyperiod / cicleperiod;
            tbSTCclockvalue.Text = frquencyticks.ToString();

            var totalincs = time / frequencyperiod;

            var rpmtomove = rpmend - rpmbegin;
            var rpmperinc = rpmtomove / totalincs;

            var c0rpm = rpmbegin;
            var c0hertz = c0rpm / 60;
            var c0period = 1 / c0hertz;
            var c0ticks = c0period / cicleperiod;
            var c0tickspersetp = c0ticks / steps;
            tbSTCbeginvalue.Text = c0tickspersetp.ToString();

            var c1rpm = rpmbegin + rpmperinc;
            var c1hertz = c1rpm / 60;
            var c1period = 1 / c1hertz;
            var c1ticks = c1period / cicleperiod;
            var c1tickspersetp = c1ticks / steps;

            var c0invert = 1000000000 / c0tickspersetp;
            var c1invert = 1000000000 / c1tickspersetp;

            var value = c1invert - c0invert;
            tbSTCincvalue.Text = value.ToString();

            var cEhertz = rpmend / 60;
            var cEperiod = 1 / cEhertz;
            var cEticks = cEperiod / cicleperiod;
            var cEtickspersetp = cEticks / steps;
            tbSTCendvalue.Text = cEtickspersetp.ToString();
        }

        private void ClockFrequency_OnValidationEvent(Frequency value)
        {
            Program.Motor.ClockFrequency = value;
        }

        private void tcSteps_OnValidationEvent(string value)
        {
            var steps = int.Parse(value);
            Program.Motor.Steps = steps;
        }

        private void btSTCcalculate_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                CalculateStopStartCurve();
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            Program.ConfigurationWindow = null;
        }

        private async void btSTCsave_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var beginValue = ushort.Parse(tbSTCbeginvalue.Text);
                var endValue = ushort.Parse(tbSTCendvalue.Text);
                var incValue = byte.Parse(tbSTCincvalue.Text);
                var clockValue = ushort.Parse(tbSTCclockvalue.Text);

                await Program.Motor.ConfigStartStopCurve(beginValue, endValue, incValue, clockValue);
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }
        }
    }
}