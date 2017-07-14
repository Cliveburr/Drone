using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DroneV0Soft.App
{
    public class TimerEventTester
    {
        public void TestCounter()
        {
            var counter = new TimerEventCounter();
            TimerEventCounter_Clear(counter);
            Console.WriteLine($"counter.value = {counter.value.ToString()}");

            Thread.Sleep(100);
            TimerEventCounter_Tick(counter);
            Console.WriteLine($"counter.value = {counter.value.ToString()}");

            Thread.Sleep(200);
            TimerEventCounter_Tick(counter);
            Console.WriteLine($"counter.value = {counter.value.ToString()}");

            Thread.Sleep(300);
            TimerEventCounter_Tick(counter);
            Console.WriteLine($"counter.value = {counter.value.ToString()}");

            TimerEventCounter_Clear(counter);
            Console.WriteLine($"counter.value = {counter.value.ToString()}");

            Thread.Sleep(100);
            TimerEventCounter_Tick(counter);
            Console.WriteLine($"counter.value = {counter.value.ToString()}");

            Thread.Sleep(200);
            TimerEventCounter_Tick(counter);
            Console.WriteLine($"counter.value = {counter.value.ToString()}");

            Thread.Sleep(300);
            TimerEventCounter_Tick(counter);
            Console.WriteLine($"counter.value = {counter.value.ToString()}");
        }

        public void TestRotine()
        {
            var hits = 0;

            var rotine = new TimerEventRotine
            {
                callback = (tag) =>
                {
                    Console.WriteLine($"in callback rotine {DateTime.Now.Millisecond.ToString()}");
                    hits++;
                }
            };
            TimerEventRotine_Set(rotine, 3000000);

            var task = Task.Run(() =>
            {
                do
                {
                    TimerEventRotine_Tick(rotine);

                    if (hits == 4)
                    {
                        TimerEventRotine_Set(rotine, 5000000);
                        hits++;
                    }

                } while (hits < 11);
            });

            task.Wait();
        }

        public void TimerEventCounter_Clear(TimerEventCounter counter)
        {
            counter.control.lastValue = GetActualTimer();
            counter.value = 0;
        }

        public void TimerEventCounter_Tick(TimerEventCounter counter)
        {
            var diffValue = ReadDiffValue(counter.control);

            if (diffValue > uint.MaxValue - counter.value)
            {
                counter.value = uint.MaxValue;
            }
            else
            {
                counter.value += diffValue;
            }
        }

        private uint GetActualTimer()
        {
            return (uint)DateTime.Now.Ticks;
        }

        private uint ReadDiffValue(TimerEventControl control)
        {
            uint diffValue = 0;
            var actualValue = GetActualTimer();

            if (actualValue >= control.lastValue)
            {
                diffValue = actualValue - control.lastValue;
            }
            else
            {
                diffValue = (uint.MaxValue - control.lastValue) + actualValue;
            }

            control.lastValue = actualValue;
            return diffValue;
        }

        public void TimerEventRotine_Tick(TimerEventRotine rotine)
        {
            var diffValue = ReadDiffValue(rotine.control);

            if (diffValue >= rotine.missing)
            {
                rotine.missing = rotine.value - (diffValue - rotine.missing);
                rotine.callback(rotine.tag);
            }
            else
            {
                rotine.missing -= diffValue;
            }
        }

        public void TimerEventRotine_Set(TimerEventRotine rotine, uint value)
        {
            rotine.control.lastValue = GetActualTimer();
            rotine.value = value;
            rotine.missing = value;
        }
    }

    public class TimerEventControl
    {
        public uint lastValue { get; set; }
    }

    public class TimerEventCounter
    {
        public uint value { get; set; }
        public TimerEventControl control { get; set; } = new TimerEventControl();
    }

    public class TimerEventRotine
    {
        public delegate void TimerEventDelegate(int tag);

        public uint value { get; set; }
        public uint missing { get; set; }
        public TimerEventDelegate callback { get; set; }
        public int tag { get; set; }
        public TimerEventControl control { get; set; } = new TimerEventControl();
    }
}