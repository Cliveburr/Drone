﻿using System;
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
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Motor0_Click(object sender, RoutedEventArgs e)
        {
            Program.ShowMotorWindow(0);
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            Program.Close();
        }

        private void Configuration_Click(object sender, RoutedEventArgs e)
        {
            Program.ShowConfigurationWindow();
        }
    }
}