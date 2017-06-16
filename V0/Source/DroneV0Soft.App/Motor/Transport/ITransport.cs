using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DroneV0Soft.App.Motor.Message;

namespace DroneV0Soft.App.Motor.Transport
{
    public interface ITransport
    {
        Task<T> SendMessageWithResonse<T>(IMessageRequest request) where T: IMessageResponse;
    }
}