using DroneV0Soft.App.Motor;
using DroneV0Soft.App.Motor.Transport;
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

namespace DroneV0Soft.App.Windows
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MotorWindow : Window
    {
        private int _index;
        private MotorController _motor;
        private Timer _timer;
        private ProbeWindow _probeWindow;
        private bool _afterIni = false;

        public MotorWindow(int index)
        {
            _index = index;
            Title += _index;

            InitializeComponent();

            //_device = new Device();
            //_device.OnMessageReceive += _device_OnMessageReceive;
            _motor = new MotorController();
            _motor.Transport = new UsbTransport();

            _afterIni = true;

            //GetChannelInfo();
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

        private async void GetChannelInfo()
        {
            try
            {
                var info = await _motor.GetChannelInfo(_index);

                if (info.ChannelMode == ChannelModeEnum.CM_Manual)
                {
                    tabControl.SelectedIndex = 0;
                }
                else
                {
                    tabControl.SelectedIndex = 1;
                }
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }
        }

        private async void tabControlManual_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            try
            {
                await _motor.ChannelChangeMode(_index, ChannelModeEnum.CM_Manual);
            }
            catch (Exception err)
            {
                e.Handled = true;

                Program.ErrorHandler(err);
            }
        }

        private async void tabControlAutomatic_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            try
            {
                await _motor.ChannelChangeMode(_index, ChannelModeEnum.CM_Automatic);
            }
            catch (Exception err)
            {
                e.Handled = true;

                Program.ErrorHandler(err);
            }
        }

        private void tgbOnOff_Click(object sender, RoutedEventArgs e)
        {

        }

        private void UISafe(Func<Task> action)
        {
            Dispatcher.Invoke(() =>
            {
                try
                {
                    action();
                }
                catch (AggregateException aggErr)
                {
                    var err = aggErr.InnerExceptions.First();
                    Program.ErrorHandler(err);
                }
                catch (Exception err)
                {
                    Program.ErrorHandler(err);
                }
            });
        }

        //private void _device_OnMessageReceive(byte[] msg)
        //{
        //    if (msg.Length == 0)
        //        return;

        //    switch (msg[0])
        //    {
        //        case 1: label.Content = "press"; break;
        //        case 2: label.Content = ""; break;
        //    }
        //}

        private void button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                var msg = new byte[] { 1 };
                //_device.SendMessage(msg);
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }

        private const double adc = 0.0048828125;   // 5 / 1023

        //private async void _timer_Elapsed(object sender, ElapsedEventArgs e)
        //{
        //    _timer.Stop();
        //    try
        //    {
        //        var msg = new byte[] { 2 };
        //        var read = await _device.WriteAndRead(msg);

        //        Dispatcher.Invoke(() =>
        //        {

        //            // read[0] = 0000 0011
        //            // read[1] = 1111 1111

        //            //var text = $"low: {read[0]}, high: {read[1]}";
        //            var value = (read[1] << 8) | (read[0]);
        //            var volt = value * adc;

        //            // VADC_in = (VREF +−VREF−)⋅n212−1 + VREF−
        //            //var tt = (5 - 0) * (value / ((2 ^ 10) - 1)) + 5;

        //            label.Content = $"volt: {volt.ToString("#.000000")}V";
        //            //switch (read[0])
        //            //{
        //            //    case 1: label.Content = "press"; break;
        //            //    case 0: label.Content = ""; break;
        //            //}
        //        });
        //    }
        //    catch (Exception err)
        //    {
        //    }
        //    _timer.Start();
        //}

        //private void Probe_Click(object sender, RoutedEventArgs e)
        //{
        //    try
        //    {
        //        if (_probeWindow == null)
        //        {
        //            _probeWindow = new ProbeWindow(_device);
        //            _probeWindow.Closed += (object sender2, EventArgs e2) => { _probeWindow = null; };
        //            _probeWindow.Show();
        //        }
        //        else
        //        {
        //            _probeWindow.Activate();
        //        }
        //    }
        //    catch (Exception err)
        //    {
        //        MessageBox.Show(err.ToString());
        //    }
        //}

        const double timer_value = 0.00000008333333333;

        private void slStepRPM_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!_afterIni)
                return;

            UISafe(async () =>
            {
                SendStepRPMValueChange();
            });
        }

        private void tbSteps_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.Enter)
                return;

            UISafe(async  () =>
            {
                SendStepRPMValueChange();
            });
        }

        private void tbRPMmax_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.Enter)
                return;

            UISafe(async () =>
            {
                var rpmMax = int.Parse(tbRPMmax.Text);
                slStepRPM.Maximum = rpmMax;
            });
        }

        private void SendStepRPMValueChange()
        {
            var steps = int.Parse(tbSteps.Text);
            var value = slStepRPM.Value;

            var hertz = value / 60;    // converte o rpm em hertz
            var period = 1 / hertz;    // converte hertz para periodo
            var tick = period / timer_value;     // calcula o valor do timer para o periodo
            var tick_step = (uint)(tick / steps);    // divide entre os passos do motor

            var tick_step_bytes = BitConverter.GetBytes(tick_step);

            var msg = new byte[] { 3, tick_step_bytes[0], tick_step_bytes[1], tick_step_bytes[2], tick_step_bytes[3] };
            //_device.SendMessage(msg);

            var onestepperiodinus = (period / steps * 1000000);

            lbRPMlabel.Content = $"RPM: {value.ToString("0")}";
            lbRPMdisplay.Content = $"RPM tick: {tick.ToString("#,##0.000")} - Step tick: {tick_step.ToString("#,##0.000")}";
            lbStepWidth.Content = $"{onestepperiodinus.ToString("#,##0.000")}us";
        }

        private void tgbCClock_Click(object sender, RoutedEventArgs e)
        {
            var value = tgbCClock.IsChecked ?? false ? 1 : 0;

            UISafe(async () =>
            {
                var msg = new byte[] { 5, (byte)value };
                //_device.SendMessage(msg);
            });
        }

        public void tglRunning_Click(object sender, RoutedEventArgs e)
        {
            var value = tglRunning.IsChecked ?? false ? 1 : 0;

            UISafe(async () =>
            {
                var msg = new byte[] { 7, (byte)value };
                //_device.SendMessage(msg);
            });
        }

        public void btOneStep_Click(object sender, RoutedEventArgs e)
        {
            UISafe(async () =>
            {
                var msg = new byte[] { 8 };
                //_device.SendMessage(msg);
            });
        }

        private void tbPWMwidth_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.Enter)
                return;

            UISafe(async () =>
            {
                SendPWMValuesChange();
            });

            //    var hertz = double.Parse(tbPWMwidth.Text);
            //var period = 1 / hertz;    // converte hertz para periodo
            //var tick = (uint)(period / timer_value);     // calcula o valor do timer para o periodo

            //var tick_bytes = BitConverter.GetBytes(tick);

            //var msg = new byte[] { 6, tick_bytes[0], tick_bytes[1], tick_bytes[2], tick_bytes[3] };
            //_device.SendMessage(msg);

            //PWM_Slider.Value = tick / 2;
            //PWM_Slider.Maximum = tick;
        }

        private void PWM_Slider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!_afterIni)
                return;

            UISafe(async () =>
            {
                SendPWMValuesChange();
            });

            //var value = PWM_Slider.Value;

            //PWM_Label.Content = $"PWM: {value.ToString("0")}";

            //var pwm_on = (uint)(value);

            //var pwm_on_bytes = BitConverter.GetBytes(pwm_on);

            //var msg = new byte[] { 4, pwm_on_bytes[0], pwm_on_bytes[1], pwm_on_bytes[2], pwm_on_bytes[3] };
            //_device.SendMessage(msg);

            //PWM_Display.Content = $"PWM Maxium tick: {PWM_Slider.Maximum.ToString("#,000")} - On tick: {pwm_on.ToString("#,000")} - Off tick: {(PWM_Slider.Maximum - pwm_on).ToString("#,000")}";
        }

        private void slPWMadcon_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!_afterIni)
                return;

            UISafe(async () =>
            {
                SendPWMValuesChange();
            });
        }

        private void SendPWMValuesChange()
        {
            var pwmhertz = double.Parse(tbPWMwidth.Text);
            var pwmperiod = 1 / pwmhertz;    // converte hertz para periodo
            var pwmtick = (uint)(pwmperiod / timer_value);     // calcula o valor do timer para o periodo

            PWM_Slider.Maximum = pwmtick;

            var onvalue = PWM_Slider.Value;
            var onvaluebeforeadcpercent = slPWMadcon.Value;
            var onvaluebeforeadc = (onvaluebeforeadcpercent * onvalue) / 100;
            var onvalueafteradc = onvalue - onvaluebeforeadc;
            var onvalueoff = pwmtick - onvalue;

            var pwmonbeforeadc_bytes = BitConverter.GetBytes((uint)onvaluebeforeadc);
            var pwmonafteradc_bytes = BitConverter.GetBytes((uint)onvalueafteradc);
            var pwmoff_bytes = BitConverter.GetBytes((uint)onvalueoff);

            var msg = new byte[] { 4,
                pwmonbeforeadc_bytes[0], pwmonbeforeadc_bytes[1], pwmonbeforeadc_bytes[2], pwmonbeforeadc_bytes[3],
                pwmonafteradc_bytes[0], pwmonafteradc_bytes[1], pwmonafteradc_bytes[2], pwmonafteradc_bytes[3],
                pwmoff_bytes[0], pwmoff_bytes[1], pwmoff_bytes[2], pwmoff_bytes[3]
            };
            //_device.SendMessage(msg);

            var pwmperiodus = pwmperiod * 1000000;
            var onvaluebeforeadcus = onvaluebeforeadc * timer_value * 1000000;

            PWM_Label.Content = $"PWM on: {onvalue.ToString("#.000")}";
            lbPWMadcon.Content = $"Adc aquisition: {slPWMadcon.Value.ToString("#.000")}%";
            lbPWMwidth.Content = $"{pwmperiodus.ToString("#.000")}us";
            lbAdcTiming.Content = $"{onvaluebeforeadcus.ToString("#.000")}us";
            PWM_Display.Content = $"PWM Maxium tick: {PWM_Slider.Maximum.ToString("#,000")} - On tick: {onvalue.ToString("#,000")} - Off tick: {(PWM_Slider.Maximum - onvalue).ToString("#,000")}";
        }
    }
}