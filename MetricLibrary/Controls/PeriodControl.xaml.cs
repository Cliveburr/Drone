using System;
using System.Collections.Generic;
using System.Globalization;
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

namespace MetricLibrary.Controls
{
    public partial class PeriodControl : UserControl
    {
        public delegate void OnValidationDelegate(Period value);
        public event OnValidationDelegate OnValidationEvent;

        private Timer _timer;
        private Period _value;

        public PeriodControl(Period value)
        {
            InitializeComponent();

            _value = value;

            _timer = new Timer();
            _timer.Interval = 400;
            _timer.Elapsed += _timer_Elapsed;
            _timer.Stop();

            ShowText();
            comboBox.SelectedIndex = (int)_value.Unit;
        }

        public PeriodControl()
            : this(new Period(0, PeriodUnit.Seconds))
        {
        }

        private void textBox_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            _timer.Stop();

            if (e.Key == Key.Enter)
            {
                OnValidation();
            }
            else
            {
                _timer.Start();
            }
        }

        private void _timer_Elapsed(object sender, ElapsedEventArgs e)
        {
            _timer.Stop();

            OnValidation();
        }

        private void OnValidation()
        {
            try
            {
                Dispatcher.Invoke(() =>
                {
                    _value.Value = double.Parse(textBox.Text, CultureInfo.InvariantCulture);
                    OnValidationEvent?.Invoke(_value);
                });
            }
            catch (Exception err)
            {
                ErrorHandler.ErrorEvent?.Invoke(err);
            }
        }

        private void comboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (_value == null)
                return;

            _value.ConvertToUnit((PeriodUnit)comboBox.SelectedIndex);
            _value.Value = Math.Round(_value.Value, 5);
            ShowText();
        }

        private void ShowText()
        {
            textBox.Text = _value.Value.ToString(CultureInfo.InvariantCulture);
        }

        public void SetPeriod(Period value)
        {
            _value = value;
            ShowText();
            comboBox.SelectedIndex = (int)_value.Unit;
        }

        public Period GetPeriod()
        {
            return _value;
        }
    }
}