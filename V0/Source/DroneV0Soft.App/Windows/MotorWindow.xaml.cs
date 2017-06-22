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
        const double timer_value = 0.00000008333333333;

        private int _index;
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

            GetChannelInfo(true);

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

        private async void GetChannelInfo(bool closeIfError = false)
        {
            try
            {
                var info = await Program.Motor.GetChannelInfo(_index);

                if (info.Mode == ChannelModeEnum.CM_Manual)
                {
                    tabControl.SelectedIndex = 0;

                    tgbManualOnOff.IsChecked = info.State == ChannelStateEnum.CS_ManualOn;


                    var steps = int.Parse(tbManualSteps.Text);
                    var tick = info.StepTimerValue * steps;
                    var period = tick * timer_value;
                    var value = period * 60;

                    slManualStepRPM.Value = value;

                    lbManualRPMlabel.Content = $"RPM: {value.ToString("0")}";
                    lbManualRPMdisplay.Content = $"RPM tick: {tick.ToString("#,##0.000")} - Step tick: {info.StepTimerValue.ToString("#,##0.000")}";
                }
                else
                {
                    tabControl.SelectedIndex = 1;

                    tgbAutoOnOff.IsChecked = info.State != ChannelStateEnum.CS_Automatic_Off;
                }
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);

                if (closeIfError)
                {
                    IsEnabled = false;
                    Close();
                }
            }
            SetManualGroups();
            _afterIni = true;
        }

        private async void tabControlManual_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            try
            {
                await Program.Motor.ChannelChangeMode(_index, ChannelModeEnum.CM_Manual);
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
                await Program.Motor.ChannelChangeMode(_index, ChannelModeEnum.CM_Automatic);
            }
            catch (Exception err)
            {
                e.Handled = true;

                Program.ErrorHandler(err);
            }
        }

        private async void tgbManualOnOff_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            try
            {
                var state = tgbManualOnOff.IsChecked ?? false ?
                    ChannelStateEnum.CS_ManualOff :
                    ChannelStateEnum.CS_ManualOn;

                await Program.Motor.ChannelChangeState(_index, state);
            }
            catch (Exception err)
            {
                e.Handled = true;

                Program.ErrorHandler(err);
            }
            SetManualGroups(true);
        }

        private void SetManualGroups(bool inverse = false)
        {
            grbManualStep.IsEnabled = tgbManualOnOff.IsChecked ?? false;
            grbManualPwm.IsEnabled = tgbManualOnOff.IsChecked ?? false;

            if (inverse)
            {
                grbManualStep.IsEnabled ^= true;
                grbManualPwm.IsEnabled ^= true;
            }
        }

        #region ChannelManualConfig
        private void tgbManualDirection_Click(object sender, RoutedEventArgs e)
        {
            SendManualConfig(false);
        }

        public void btManualOneStep_Click(object sender, RoutedEventArgs e)
        {
            SendManualConfig(true);
        }

        private async void SendManualConfig(bool oneStep)
        {
            try
            {
                var direction = tgbManualDirection.IsChecked ?? false ? 1 : 0;
                var oneStepByte = oneStep ? 1: 0;

                await Program.Motor.ChannelManualConfig(_index, (byte)direction, (byte)oneStepByte);
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }
        }
        #endregion

        #region ChannelManualStep
        private async void slManualStepRPM_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!_afterIni)
                return;

            try
            {
                await SendManualStep();
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }
        }

        private async void tbManualSteps_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.Enter)
                return;

            try
            {
                await SendManualStep();
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }
        }

        private void tbManualRPMmax_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.Enter)
                return;

            try
            {
                var rpmMax = int.Parse(tbManualRPMmax.Text);
                slManualStepRPM.Maximum = rpmMax;
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }
        }

        private async Task SendManualStep()
        {
            var steps = int.Parse(tbManualSteps.Text);
            var value = slManualStepRPM.Value;

            var hertz = value / 60;    // converte o rpm em hertz
            var period = 1 / hertz;    // converte hertz para periodo
            var tick = period / timer_value;     // calcula o valor do timer para o periodo
            var tick_step = (uint)(tick / steps);    // divide entre os passos do motor

            await Program.Motor.ChannelManualStep(_index, tick_step);

            var onestepperiodinus = (period / steps * 1000000);

            lbManualRPMlabel.Content = $"RPM: {value.ToString("0")}";
            lbManualRPMdisplay.Content = $"RPM tick: {tick.ToString("#,##0.000")} - Step tick: {tick_step.ToString("#,##0.000")}";
            lbStepWidth.Content = $"{onestepperiodinus.ToString("#,##0.000")}us";
        }
        #endregion

        #region old
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

        //private void button_Click(object sender, RoutedEventArgs e)
        //{
        //    try
        //    {
        //        var msg = new byte[] { 1 };
        //        //_device.SendMessage(msg);
        //    }
        //    catch (Exception err)
        //    {
        //        MessageBox.Show(err.ToString());
        //    }
        //}

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
        #endregion

        #region ChannelManualPWM
        private async void tbManualPWMwidth_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key != Key.Enter)
                return;

            try
            {
                await SendPWMValuesChange();
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }

            //    var hertz = double.Parse(tbPWMwidth.Text);
            //var period = 1 / hertz;    // converte hertz para periodo
            //var tick = (uint)(period / timer_value);     // calcula o valor do timer para o periodo

            //var tick_bytes = BitConverter.GetBytes(tick);

            //var msg = new byte[] { 6, tick_bytes[0], tick_bytes[1], tick_bytes[2], tick_bytes[3] };
            //_device.SendMessage(msg);

            //PWM_Slider.Value = tick / 2;
            //PWM_Slider.Maximum = tick;
        }

        private async void slManualPWM_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!_afterIni)
                return;

            try
            {
                await SendPWMValuesChange();
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }

            //var value = PWM_Slider.Value;

            //PWM_Label.Content = $"PWM: {value.ToString("0")}";

            //var pwm_on = (uint)(value);

            //var pwm_on_bytes = BitConverter.GetBytes(pwm_on);

            //var msg = new byte[] { 4, pwm_on_bytes[0], pwm_on_bytes[1], pwm_on_bytes[2], pwm_on_bytes[3] };
            //_device.SendMessage(msg);

            //PWM_Display.Content = $"PWM Maxium tick: {PWM_Slider.Maximum.ToString("#,000")} - On tick: {pwm_on.ToString("#,000")} - Off tick: {(PWM_Slider.Maximum - pwm_on).ToString("#,000")}";
        }

        private async void slManualPWMadcon_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            if (!_afterIni)
                return;

            try
            {
                await SendPWMValuesChange();
            }
            catch (Exception err)
            {
                Program.ErrorHandler(err);
            }
        }

        private async Task SendPWMValuesChange()
        {
            var pwmhertz = double.Parse(tbManualPWMwidth.Text);
            var pwmperiod = 1 / pwmhertz;    // converte hertz para periodo
            var pwmtick = (uint)(pwmperiod / timer_value);     // calcula o valor do timer para o periodo

            slManualPWM.Maximum = pwmtick;

            var onvalue = slManualPWM.Value;
            var onvaluebeforeadcpercent = slManualPWMadcon.Value;
            var onvaluebeforeadc = (onvaluebeforeadcpercent * onvalue) / 100;
            var onvalueafteradc = onvalue - onvaluebeforeadc;
            var onvalueoff = pwmtick - onvalue;

            //var pwmonbeforeadc_bytes = BitConverter.GetBytes((uint)onvaluebeforeadc);
            //var pwmonafteradc_bytes = BitConverter.GetBytes((uint)onvalueafteradc);
            //var pwmoff_bytes = BitConverter.GetBytes((uint)onvalueoff);

            //var msg = new byte[] { 4,
            //    pwmonbeforeadc_bytes[0], pwmonbeforeadc_bytes[1], pwmonbeforeadc_bytes[2], pwmonbeforeadc_bytes[3],
            //    pwmonafteradc_bytes[0], pwmonafteradc_bytes[1], pwmonafteradc_bytes[2], pwmonafteradc_bytes[3],
            //    pwmoff_bytes[0], pwmoff_bytes[1], pwmoff_bytes[2], pwmoff_bytes[3]
            //};
            //_device.SendMessage(msg);
            await Program.Motor.ChannelManualPWM(_index, (ushort)onvaluebeforeadc, (ushort)onvalueafteradc, (ushort)onvalueoff);

            var pwmperiodus = pwmperiod * 1000000;
            var onvaluebeforeadcus = onvaluebeforeadc * timer_value * 1000000;

            lbManualPWM.Content = $"PWM on: {onvalue.ToString("#.000")}";
            lbManualPWMadcon.Content = $"Adc aquisition: {slManualPWMadcon.Value.ToString("#.000")}%";
            lbPWMwidth.Content = $"{pwmperiodus.ToString("#.000")}us";
            lbAdcTiming.Content = $"{onvaluebeforeadcus.ToString("#.000")}us";
            lbDispalyPWM.Content = $"PWM Maxium tick: {slManualPWM.Maximum.ToString("#,000")} - On tick: {onvalue.ToString("#,000")} - Off tick: {(slManualPWM.Maximum - onvalue).ToString("#,000")}";
        }
        #endregion

        private async void tgbAutoOnOff_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            try
            {
                var state = tgbAutoOnOff.IsChecked ?? false ?
                    ChannelStateEnum.CS_Automatic_Off :
                    ChannelStateEnum.CS_AutomaticStarting;

                await Program.Motor.ChannelChangeState(_index, state);
            }
            catch (Exception err)
            {
                e.Handled = true;

                Program.ErrorHandler(err);
            }
            //SetManualGroups(true);
        }
    }
}