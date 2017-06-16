using DroneV0Soft.App.Windows;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace DroneV0Soft.App
{
    public static class Program
    {
        public static Application App { get; private set; }
        public static MainWindow MainWindow { get; private set; }
        public static MotorWindow[] MotorWindow { get; private set; }

        [STAThread]
        public static void Main()
        {
            MotorWindow = new MotorWindow[4];

            MainWindow = new MainWindow();
            MainWindow.Show();

            App = new Application();
            App.ShutdownMode = ShutdownMode.OnExplicitShutdown;
            App.Run();
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
                MotorWindow[index] = motorWindow;
                motorWindow.Show();
            }
            else
            {
                motorWindow.Activate();
            }
        }
    }
}