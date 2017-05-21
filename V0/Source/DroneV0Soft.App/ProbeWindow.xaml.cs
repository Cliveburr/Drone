using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
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
using System.Windows.Shapes;
using System.Xml.Serialization;

namespace DroneV0Soft.App
{
    public partial class ProbeWindow : Window
    {
        public string CONFIG_FILE = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "DroneV0Soft_Probe.config.xml");

        private Timer _activeTimer;
        private ProbeChannel[] _channels;
        private DateTime _viewStart;
        private Random rnd = new Random();
        private Device _device;

        public ProbeWindow(Device device)
        {
            _device = device;

            InitializeComponent();

            _channels = new ProbeChannel[6];

            LoadConfig();
            CheckActive();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            var context = DataContext as ProbeContext;

            CheckChannelActive();

            foreach (var view in context.Views)
            {
                CreateGrids(view);
            }
        }

        private void LoadConfig()
        {
            if (System.IO.File.Exists(CONFIG_FILE))
            {
                using (var stream = System.IO.File.OpenRead(CONFIG_FILE))
                {
                    var xml = new XmlSerializer(typeof(ProbeContext));
                    DataContext = xml.Deserialize(stream) as ProbeContext;
                }
            }
            else
            {
                DataContext = new ProbeContext
                {
                    Active = false,
                    AcquistionTimer = 100,
                    ViewField = 10000,
                    MaxValue = 5000,
                    XGrid = 100,
                    YGrid = 100,
                    Views = new ObservableCollection<ProbeViewContext>(),
                    ChannelA = false,
                    ChannelB = false,
                    ChannelC = false,
                    ChannelD = false,
                    ChannelE = false,
                    ChannelF = false
                };
            }
        }

        private void SaveConfig()
        {
            using (var stream = System.IO.File.OpenWrite(CONFIG_FILE))
            {
                var xml = new XmlSerializer(typeof(ProbeContext));
                xml.Serialize(stream, DataContext);
            }
        }

        private void Active_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                CheckActive();
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }

        private void CheckActive()
        {
            var context = DataContext as ProbeContext;

            if (context.Active)
            {
                if (_activeTimer == null)
                {
                    _activeTimer = new Timer();
                    _activeTimer.Interval = context.AcquistionTimer;
                    _activeTimer.Elapsed += _activeTimer_Elapsed;
                    _viewStart = DateTime.Now;
                }
                _activeTimer.Start();
            }
            else
            {
                if (_activeTimer != null)
                {
                    _activeTimer.Stop();
                }
            }
        }

        private const double adc = 0.0048828125;   // 5 / 1023

        private async void _activeTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            _activeTimer.Stop();

            var now = DateTime.Now;

            var msg = new byte[] { 2 };
            var read = await _device.WriteAndRead(msg);

            var c0value = (read[1] << 8) | (read[0]);
            //var c0volt = c0value * adc;
            var c0point = new ProbePoint(now, (uint)c0value);

            Dispatcher.Invoke(() =>
            {
                var context = DataContext as ProbeContext;

                ProbeAddPointOnChannel(_channels[0], c0point);

                if (false)  // simulation
                {
                    Console.WriteLine("hit" + DateTime.Now.ToString());
                    for (var i = 0; i < 6; i++)
                    {
                        if (_channels[i] != null)
                        {
                            var value = rnd.Next(-30, 30);

                            if (_channels[i].Values.Count > 0)
                            {
                                value = ((int)_channels[i].Values.Last().Value) + value;
                            }

                            if (value < 0)
                                value = 0;

                            if (value > context.MaxValue)
                                value = (int)context.MaxValue;

                            var point = new ProbePoint(now, (uint)value);

                            _channels[i].Values.Add(point);

                            if (_channels[i].View != null)
                            {
                                ProbeAddPointOnChannel(_channels[i], point);
                            }
                        }
                    }
                }

                if (context.Active)
                    _activeTimer?.Start();
            });
        }

        private void AcquistionTimer_KeyDown(object sender, KeyEventArgs e)
        {
            var context = DataContext as ProbeContext;

            if (e.Key == Key.Enter)
            {
                _activeTimer.Interval = context.AcquistionTimer;
            }
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            if (_activeTimer != null)
            {
                _activeTimer.Stop();
                _activeTimer.Dispose();
                _activeTimer = null;
            }
            SaveConfig();
        }

        private void AddView_Click(object sender, RoutedEventArgs e)
        {
            var context = DataContext as ProbeContext;

            try
            {
                if (context.Views == null)
                {
                    context.Views = new ObservableCollection<ProbeViewContext>();
                }

                if (context.Views.Count >= 3)
                {
                    throw new Exception("Maximum view reach!");
                }

                var view =  new ProbeViewContext
                {
                    ChannelA = false,
                    ChannelB = false,
                    ChannelC = false,
                    ChannelD = false,
                    ChannelE = false,
                    ChannelF = false,
                    EnableChannelA = false,
                    EnableChannelB = false,
                    EnableChannelC = false,
                    EnableChannelD = false,
                    EnableChannelE = false,
                    EnableChannelF = false
                };
                context.Views.Add(view);
                context.RaiseNotify("Views");

                CreateGrids(view);

                CheckChannelEnabled();
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }

        private void RemoveView_Click(object sender, RoutedEventArgs e)
        {
            var context = DataContext as ProbeContext;
            var viewContext = ((Button)sender).Tag as ProbeViewContext;

            context.Views.Remove(viewContext);

            CheckChannelEnabled();
        }

        private void ChannelSelect_Change(object sender, RoutedEventArgs e)
        {
            var context = DataContext as ProbeContext;
            var channel = ((CheckBox)sender).Tag as string;

            var idChannel = channel[0] - 'A';

            var usingChannel = false;
            switch (channel)
            {
                case "A": usingChannel = context.ChannelA; break;
                case "B": usingChannel = context.ChannelB; break;
                case "C": usingChannel = context.ChannelC; break;
                case "D": usingChannel = context.ChannelD; break;
                case "E": usingChannel = context.ChannelE; break;
                case "F": usingChannel = context.ChannelF; break;
            }

            if (usingChannel)
            {
                _channels[idChannel] = new ProbeChannel
                {
                    Values = new List<ProbePoint>(),
                    Grid = null,
                    Line = null,
                    View = null
                };
            }
            else
            {
                _channels[idChannel] = null;

                foreach (var view in context.Views)
                {
                    switch (idChannel)
                    {
                        case 0: view.ChannelA = false; view.RaiseNotify("ChannelA"); break;
                        case 1: view.ChannelB = false; view.RaiseNotify("ChannelB"); break;
                        case 2: view.ChannelC = false; view.RaiseNotify("ChannelC"); break;
                        case 3: view.ChannelD = false; view.RaiseNotify("ChannelD"); break;
                        case 4: view.ChannelE = false; view.RaiseNotify("ChannelE"); break;
                        case 5: view.ChannelF = false; view.RaiseNotify("ChannelF"); break;
                    }
                }
            }

            CheckChannelEnabled();
        }

        private void CheckChannelEnabled()
        {
            var context = DataContext as ProbeContext;
            var avaliable = _channels.Select(c => c != null).ToArray();

            foreach (var view in context.Views)
            {
                var channles = GetChannelsViewInArray(view);

                for (var i = 0; i < 6; i++)
                {
                    if (avaliable[i])
                    {
                        if (channles[i])
                        {
                            avaliable[i] = false;
                        }
                    }
                }
            }

            foreach (var view in context.Views)
            {
                var channles = GetChannelsViewInArray(view);
                var enables = GetEnableChannelsViewInArray(view);

                for (var i = 0; i < 6; i++)
                {
                    if (channles[i])
                    {
                        enables[i] = true;
                    }
                    else
                    {
                        enables[i] = avaliable[i];
                    }
                }

                SetEnableChannelsViewInArray(view, enables);
            }
        }

        private bool[] GetChannelsViewInArray(ProbeViewContext view)
        {
            return new bool[] { view.ChannelA, view.ChannelB, view.ChannelC, view.ChannelD, view.ChannelE, view.ChannelF };
        }

        private bool[] GetEnableChannelsViewInArray(ProbeViewContext view)
        {
            return new bool[] { view.EnableChannelA, view.EnableChannelB, view.EnableChannelC, view.EnableChannelD, view.EnableChannelE, view.EnableChannelF };
        }

        private void SetEnableChannelsViewInArray(ProbeViewContext view, bool[] values)
        {
            view.EnableChannelA = values[0];
            view.EnableChannelB = values[1];
            view.EnableChannelC = values[2];
            view.EnableChannelD = values[3];
            view.EnableChannelE = values[4];
            view.EnableChannelF = values[5];

            view.RaiseNotify("EnableChannelA");
            view.RaiseNotify("EnableChannelB");
            view.RaiseNotify("EnableChannelC");
            view.RaiseNotify("EnableChannelD");
            view.RaiseNotify("EnableChannelE");
            view.RaiseNotify("EnableChannelF");
        }

        private void CheckChannelActive()
        {
            var context = DataContext as ProbeContext;

            foreach (var view in context.Views)
            {
                var channels = GetChannelsViewInArray(view);

                for (var i = 0; i < 6; i++)
                {
                    if (channels[i])
                    {
                        SetChannelActive(i, view);
                    }
                }
            }
        }

        public void ViewChannelSelect_Change(object sender, RoutedEventArgs e)
        {
            var context = DataContext as ProbeContext;
            var checkBox = sender as CheckBox;
            var viewContext = checkBox.Tag as ProbeViewContext;

            var idChannel = checkBox.Name[0] - 'A';

            if (GetChannelsViewInArray(viewContext)[idChannel])
            {
                SetChannelActive(idChannel, viewContext);
            }
            else
            {
                if (_channels[idChannel] != null)
                {
                    _channels[idChannel].Grid?.Children.Remove(_channels[idChannel].Line);

                    _channels[idChannel].View = null;
                    _channels[idChannel].Line = null;
                    _channels[idChannel].Grid = null;
                }
            }

            CheckChannelEnabled();
        }

        private void SetChannelActive(int idChannel, ProbeViewContext viewContext)
        {
            var line = new Polyline();
            switch (idChannel)
            {
                case 0: line.Stroke = Brushes.Blue; break;
                case 1: line.Stroke = Brushes.Green; break;
                case 2: line.Stroke = Brushes.Red; break;
                case 3: line.Stroke = Brushes.Brown; break;
                case 4: line.Stroke = Brushes.Cyan; break;
                case 5: line.Stroke = Brushes.Magenta; break;
            }
            line.StrokeThickness = 1;

            var container = itemsControl.ItemContainerGenerator.ContainerFromItem(viewContext) as FrameworkElement;
            var grid = FindChild<Grid>(container, (g) => g.Name == "MyGrid");
            grid.Children.Add(line);

            _channels[idChannel].View = viewContext;
            _channels[idChannel].Line = line;
            _channels[idChannel].Grid = grid;

            foreach (var value in _channels[idChannel].Values)
            {
                ProbeAddPointOnChannel(_channels[idChannel], value);
            }
        }

        public static T FindChild<T>(DependencyObject depObj, Func<T, bool> predicate) where T : DependencyObject
        {
            if (depObj != null)
            {
                for (int i = 0; i < VisualTreeHelper.GetChildrenCount(depObj); i++)
                {
                    DependencyObject child = VisualTreeHelper.GetChild(depObj, i);
                    if (child != null && child is T)
                    {
                        if (predicate((T)child))
                            return (T)child;
                    }

                    T childItem = FindChild<T>(child, predicate);
                    if (childItem != null)
                        return childItem;
                }
            }
            return null;
        }

        private void ProbeAddPointOnChannel(ProbeChannel channel, ProbePoint point)
        {
            var context = DataContext as ProbeContext;

            if (!context.Active)
                return;

            var posTimer = point.DateTime - _viewStart;

            if (posTimer.TotalMilliseconds > context.ViewField)
            {
                if (context.OneShot)
                {
                    context.Active = false;
                    context.OneShot = false;
                    context.RaiseNotify("Active");
                    context.RaiseNotify("OneShot");
                    return;
                }
                else
                {
                    foreach (var cnh in _channels)
                    {
                        cnh?.Line?.Points.Clear();
                    }
                    _viewStart = point.DateTime;

                    posTimer = new TimeSpan();
                }
            }

            var yfield = channel.Grid.ActualHeight - 0;
            var ypos = (yfield / context.MaxValue) * point.Value;

            var xfield = channel.Grid.ActualWidth - 0;
            var xpos = (xfield / context.ViewField) * posTimer.TotalMilliseconds;

            channel.Line.Points.Add(new Point(xpos, yfield - ypos));
        }

        private void CreateGrids(ProbeViewContext view)
        {
            var context = DataContext as ProbeContext;

            var container = itemsControl.ItemContainerGenerator.ContainerFromItem(view) as FrameworkElement;
            var grid = FindChild<Grid>(container, (g) => g.Name == "MyGrid");
            if (grid == null)
            {
                Task.Run(() =>
                {
                    System.Threading.Thread.Sleep(10);
                    Dispatcher.Invoke(() => CreateGrids(view));
                });
                return;
            }

            var isNew = view.GridLine == null;
            var gridLines = view.GridLine ?? new Grid();
            gridLines.Children.Clear();

            var xfield = grid.ActualWidth - 0;
            var propXuni = (xfield / context.ViewField) * context.XGrid;

            var yfield = grid.ActualHeight - 0;
            var propYuni = (yfield / context.MaxValue) * context.YGrid;

            for (var x = propXuni; x < xfield; x += propXuni)
            {
                var linY = new Line();
                linY.StrokeThickness = 1;
                linY.Stroke = Brushes.Gray;
                linY.X1 = x;
                linY.Y1 = 0;
                linY.X2 = x;
                linY.Y2 = yfield;

                gridLines.Children.Add(linY);
            }

            for (var y = propYuni; y < yfield; y += propYuni)
            {
                var linX = new Line();
                linX.StrokeThickness = 1;
                linX.Stroke = Brushes.Gray;
                linX.X1 = 0;
                linX.Y1 = y;
                linX.X2 = xfield;
                linX.Y2 = y;

                gridLines.Children.Add(linX);
            }

            if (isNew)
            {
                view.GridLine = gridLines;
                grid.Children.Add(gridLines);
            }
        }

        private void YGrid_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                if (e.Key != Key.Enter)
                    return;

                var context = DataContext as ProbeContext;

                foreach (var view in context.Views)
                {
                    CreateGrids(view);
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }

        private void XGrid_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                if (e.Key != Key.Enter)
                    return;

                var context = DataContext as ProbeContext;

                foreach (var view in context.Views)
                {
                    CreateGrids(view);
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }

        private void MyGrid_MouseMove(object sender, MouseEventArgs e)
        {
            var context = DataContext as ProbeContext;
            var grid = sender as Grid;
            var view = grid.Tag as ProbeViewContext;

            var pos = e.GetPosition(grid);

            var xfield = grid.ActualWidth - 0;
            var posX = pos.X / (xfield / context.ViewField);

            var yfield = grid.ActualHeight - 0;
            var posY = (grid.ActualHeight - pos.Y) / (yfield / context.MaxValue);

            context.MousePosition = $"{{ {posX.ToString("#0.00")} - {posY.ToString("#0.00")} }}";
            context.RaiseNotify("MousePosition");
        }

        private void MyGrid_MouseLeave(object sender, MouseEventArgs e)
        {
            var context = DataContext as ProbeContext;
            context.MousePosition = "";
            context.RaiseNotify("MousePosition");
        }

        private void Window_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            try
            {
                var context = DataContext as ProbeContext;

                foreach (var view in context.Views)
                {
                    CreateGrids(view);
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }

        private void ViewField_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                if (e.Key != Key.Enter)
                    return;

                var context = DataContext as ProbeContext;

                foreach (var view in context.Views)
                {
                    CreateGrids(view);
                }

                foreach (var channel in _channels.Where(c => c != null))
                {
                    if (channel.Line != null)
                    {
                        channel.Line.Points.Clear();
                        foreach (var point in channel.Values)
                        {
                            ProbeAddPointOnChannel(channel, point);
                        }
                    }
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }

        private void MaxValue_KeyDown(object sender, KeyEventArgs e)
        {
            try
            {
                if (e.Key != Key.Enter)
                    return;

                var context = DataContext as ProbeContext;

                foreach (var view in context.Views)
                {
                    CreateGrids(view);
                }

                foreach (var channel in _channels.Where(c => c != null))
                {
                    if (channel.Line != null)
                    {
                        channel.Line.Points.Clear();
                        foreach (var point in channel.Values)
                        {
                            ProbeAddPointOnChannel(channel, point);
                        }
                    }
                }
            }
            catch (Exception err)
            {
                MessageBox.Show(err.ToString());
            }
        }
    }

    public class ProbeContext : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public bool Active { get; set; }
        public uint AcquistionTimer { get; set; }
        public uint ViewField { get; set; }
        public uint MaxValue { get; set; }
        public uint YGrid { get; set; }
        public uint XGrid { get; set; }
        public ObservableCollection<ProbeViewContext> Views { get; set; }
        public bool ChannelA { get; set; }
        public bool ChannelB { get; set; }
        public bool ChannelC { get; set; }
        public bool ChannelD { get; set; }
        public bool ChannelE { get; set; }
        public bool ChannelF { get; set; }
        [XmlIgnore]
        public string MousePosition { get; set; }
        [XmlIgnore]
        public bool OneShot { get; set; }

        public void RaiseNotify(string property)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
        }
    }

    public class ProbeViewContext : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public bool ChannelA { get; set; }
        public bool ChannelB { get; set; }
        public bool ChannelC { get; set; }
        public bool ChannelD { get; set; }
        public bool ChannelE { get; set; }
        public bool ChannelF { get; set; }
        [XmlIgnore]
        public bool EnableChannelA { get; set; }
        [XmlIgnore]
        public bool EnableChannelB { get; set; }
        [XmlIgnore]
        public bool EnableChannelC { get; set; }
        [XmlIgnore]
        public bool EnableChannelD { get; set; }
        [XmlIgnore]
        public bool EnableChannelE { get; set; }
        [XmlIgnore]
        public bool EnableChannelF { get; set; }
        [XmlIgnore]
        public Grid GridLine;

        public void RaiseNotify(string property)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
        }
    }

    public class ProbeChannel
    {
        public ProbeViewContext View { get; set; }
        public Grid Grid { get; set; }
        public Polyline Line { get; set; }
        public List<ProbePoint> Values { get; set; }
    }

    public class ProbePoint
    {
        public DateTime DateTime { get; set; }
        public uint Value { get; set; }

        public ProbePoint(DateTime dateTime, uint value)
        {
            DateTime = dateTime;
            Value = value;
        }
    }
}