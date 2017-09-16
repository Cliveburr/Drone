using DroneV0Soft.App.Motor;
using DroneV0Soft.App.Motor.Transport;
using DroneV0Soft.App.Windows;
using MetricLibrary;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using static DroneV0Soft.App.Motor.Message.TesterMessages;

namespace DroneV0Soft.App
{
    public static class Program
    {
        public static Application App { get; private set; }
        public static MainWindow MainWindow { get; private set; }
        public static MotorWindow[] MotorWindow { get; private set; }
        public static ConfigurationWindow ConfigurationWindow { get; set; }
        public static MotorController Motor { get; private set; }

        [STAThread]
        public static void Main()
        {
            //var test = new TimerEventTester();
            //test.TestRotine();

            MotorWindow = new MotorWindow[4];

            var usbTransport = new UsbTransport();
            //usbTransport.OnRemoved += () => Dispatcher.Invoke(() => Close());

            Motor = new MotorController();
            Motor.Transport = usbTransport;

            //var request = new CrossZeroPortSelectRequest
            //{
            //    Step = (char)1
            //};
            //Program.Motor.Transport.SendMessage(request);

            LoadConfigurations();

            MainWindow = new MainWindow();
            MainWindow.Show();

            App = new Application();
            App.ShutdownMode = ShutdownMode.OnExplicitShutdown;
            App.Run();
        }

        private static void LoadConfigurations()
        {
            Motor.ClockFrequency = new Frequency(48000000);
            Motor.Steps = 36;
        }

        public static void Close()
        {
            App.Shutdown();
        }

        public static void ErrorHandler(Exception err)
        {
            if (err is AggregateException)
            {
                err = ((AggregateException)err).InnerExceptions.First();
            }

            MainWindow.Dispatcher.Invoke(() =>
            {
                MessageBox.Show(err.ToString());
            });
        }

        public static void ShowMotorWindow(int index)
        {
            var motorWindow = MotorWindow[index];

            if (motorWindow == null)
            {
                motorWindow = new MotorWindow(index);
                if (motorWindow.IsEnabled)
                {
                    MotorWindow[index] = motorWindow;
                    motorWindow.Closed += (object sender, EventArgs e) =>
                    {
                        MotorWindow[index] = null;
                    };

                    motorWindow.Show();
                }
            }
            else
            {
                motorWindow.Activate();
            }
        }

        public static void ShowConfigurationWindow()
        {
            if (ConfigurationWindow == null)
            {
                ConfigurationWindow = new ConfigurationWindow();
                ConfigurationWindow.Show();
            }
            else
            {
                ConfigurationWindow.Activate();
            }
        }
    }
}