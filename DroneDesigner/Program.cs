using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace DroneDesigner
{
    public static class Program
    {
        public static Application App { get; private set; }

        [STAThread]
        public static void Main()
        {

            //Config = UserConfig.Open();

            //Notify = new NotifyControler();
            //Notify.Set();

            //if (!Config.Data.DontShowAboutAnymore)
            //{
            //    ShowWindow(typeof(WPF.About.AboutWindow));
            //}

            //HotKeysController.Instance.SetHotKeys();

            //var capacity = new Measure.AmperePerHour(2200, Measure.AmperePerHourUnit.MiliAmperePerHour);
            //var ampare = new Measure.Ampere(2.2);
            //var max = ampare * 40;
            //var time = capacity / ampare;
            

            var main = new MainWindow();
            main.Show();

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
            MessageBox.Show(err.ToString());
        }
    }
}