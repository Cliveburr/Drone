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

namespace MetricLibrary.Controls
{
    public partial class TextControl : UserControl
    {
        public delegate void OnValidationDelegate(string value);
        public event OnValidationDelegate OnValidationEvent;

        private Timer _timer;
        private string _value;

        public TextControl(string value)
        {
            InitializeComponent();

            _value = value;

            _timer = new Timer();
            _timer.Interval = 400;
            _timer.Elapsed += _timer_Elapsed;
            _timer.Stop();

            ShowText();
        }

        public TextControl()
            :this("")
        {
        }

        private void textBox_PreviewKeyDown(object sender, KeyEventArgs e)
        {
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
                    _value = textBox.Text;
                    OnValidationEvent?.Invoke(_value);
                });
            }
            catch (Exception err)
            {
                ErrorHandler.ErrorEvent?.Invoke(err);
            }
        }

        private void ShowText()
        {
            textBox.Text = _value;
        }

        public void SetText(string value)
        {
            _value = value;
            ShowText();
        }
    }
}
