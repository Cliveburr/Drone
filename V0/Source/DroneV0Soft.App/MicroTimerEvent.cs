using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DroneV0Soft.App
{
    public class MicroTimerEvent<T>
    {
        private long _ticks;
        private long _started;
        private Thread _thread;
        private bool _isRunning;

        public T Data { get; set; }
        public delegate void ElapsedDelegate(T data);
        public ElapsedDelegate Elapsed;

        public MicroTimerEvent(T data, long interval, ElapsedDelegate elapsed)
        {
            Data = data;
            Interval = interval;
            Elapsed = elapsed;
        }

        public long Interval
        {
            get
            {
                return _ticks * 100;
            }
            set
            {
                _ticks = value / 100;
            }
        }

        public void Start()
        {
            MicroTimerEventKernel.GetSystemTimePreciseAsFileTime(out _started);

            _isRunning = true;

            _thread = new Thread(ThreadLoop);
            _thread.Priority = ThreadPriority.Highest;
            _thread.Start();
        }

        private void ThreadLoop()
        {
            long thisTime = 0;
            while (_isRunning)
            {
                MicroTimerEventKernel.GetSystemTimePreciseAsFileTime(out thisTime);

                if (thisTime - _started > _ticks)
                {
                    _started = thisTime;
                    Elapsed?.Invoke(Data);
                }
            }
            _thread = null;
        }

        public void Stop()
        {
            _isRunning = false;
        }
    }

    static class MicroTimerEventKernel
    {
        [DllImport("Kernel32.dll", CallingConvention = CallingConvention.Winapi)]
        public static extern void GetSystemTimePreciseAsFileTime(out long filetime);
    }
}