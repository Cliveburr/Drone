using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MetricLibrary
{
    public class ErrorHandler
    {
        public delegate void ErrorHandleDelegate(Exception err);
        public static ErrorHandleDelegate ErrorEvent;
    }
}