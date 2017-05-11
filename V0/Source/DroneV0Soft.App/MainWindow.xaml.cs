using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Xml.Serialization;

namespace DroneV0Soft.App
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Device _device;
        private Timer _timer;

        private ProbeWindow _probeWindow;

        public MainWindow()
        {
            InitializeComponent();

            //_device = new Device();
            //_device.OnMessageReceive += _device_OnMessageReceive;

            //_timer = new Timer();
            //_timer.Elapsed += _timer_Elapsed;
            //_timer.Interval = 100;
            //_timer.Start();


            //_probe = new ProbeProps
            //{
            //    ViewField = 10000
            //};
            //_probe.ChannelA = ProbeInitializeChannel(groupBox);

            //itemsControl.ItemsSource = new List<string> { "testando", "alog" }
           //     .Select(p => new { Value = p });
        }

        private void _device_OnMessageReceive(byte[] msg)
        {
            if (msg.Length == 0)
                return;

            switch (msg[0])
            {
                case 1: label.Content = "press"; break;
                case 2: label.Content = ""; break;
            }
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var msg = new byte[] { 1 };
                _device.SendMessage(msg);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }

        private const double adc = 0.0048828125;   // 5 / 1023

        private async void _timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            _timer.Stop();
            try
            {
                var msg = new byte[] { 2 };
                var read = await _device.WriteAndRead(msg);

                Dispatcher.Invoke(() =>
                {

                    // read[0] = 0000 0011
                    // read[1] = 1111 1111

                    //var text = $"low: {read[0]}, high: {read[1]}";
                    var value = (read[1] << 8) | (read[0]);
                    var volt = value * adc;

                    // VADC_in = (VREF +−VREF−)⋅n212−1 + VREF−
                    //var tt = (5 - 0) * (value / ((2 ^ 10) - 1)) + 5;

                    label.Content = $"volt: {volt.ToString("#.000000")}V";
                    //switch (read[0])
                    //{
                    //    case 1: label.Content = "press"; break;
                    //    case 0: label.Content = ""; break;
                    //}
                });
            }
            catch (Exception err)
            {
            }
            _timer.Start();
        }

        private void Probe_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_probeWindow == null)
                {
                    _probeWindow = new ProbeWindow();
                    _probeWindow.Closed += (object sender2, EventArgs e2) => { _probeWindow = null; };
                    _probeWindow.Show();
                }
                else
                {
                    _probeWindow.Activate();
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }
    }
}