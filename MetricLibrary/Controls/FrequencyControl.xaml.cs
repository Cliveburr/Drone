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
    public partial class FrequencyControl : UserControl
    {
        public delegate void OnValidationDelegate(Frequency value);
        public event OnValidationDelegate OnValidationEvent;

        private Timer _timer;
        private Frequency _value;

        public FrequencyControl(Frequency value)
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

        public FrequencyControl()
            : this(new Frequency(0, FrequencyUnit.Hertz))
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

            _value.ConvertToUnit((FrequencyUnit)comboBox.SelectedIndex);
            _value.Value = Math.Round(_value.Value, 5);
            ShowText();
        }

        private void ShowText()
        {
            textBox.Text = _value.Value.ToString(CultureInfo.InvariantCulture);
        }

        public void SetFrequency(Frequency value)
        {
            _value = value;
            ShowText();
            comboBox.SelectedIndex = (int)_value.Unit;
        }

        public Frequency GetFrequency()
        {
            return _value;
        }
    }
}
