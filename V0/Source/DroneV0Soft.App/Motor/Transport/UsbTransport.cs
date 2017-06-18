using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DroneV0Soft.App.Motor.Message;
using UsbHidLibrary;

namespace DroneV0Soft.App.Motor.Transport
{
    public class UsbTransport : IDisposable, ITransport
    {
        public delegate void OnMessageReceiveDelegate(byte[] msg);
        public event OnMessageReceiveDelegate OnMessageReceive;
        public delegate void OnRemovedDelegate();
        public event OnRemovedDelegate OnRemoved;

        private const int _vendorId = 0x1781;
        private const int _productId = 0x07D0;
        private HidDevice _device;
        private UsbMonitor _monitor;

        public UsbTransport()
        {
            _monitor = new UsbMonitor();
            _monitor.UsbChangeEventArrival += new UsbMonitor.UsbChangeEventHandler(monitor_Arrival);
            _monitor.UsbChangeEventRemoved += new UsbMonitor.UsbChangeEventHandler(monitor_Removed);

            monitor_Arrival();
        }

        private void monitor_Arrival()
        {
            if (_device == null)
            {
                var devices = HidDevice.Enumerate()
                    .Select(de => new HidDevice(de));

                var connected = devices
                    .Where(d => d.Attributes != null)
                    .FirstOrDefault(d => d.Attributes.VendorId == _vendorId && d.Attributes.ProductId == _productId);

                if (connected == null)
                    return;

                 _device = connected;
            }
        }

        private void monitor_Removed()
        {
            if (_device != null)
            {
                var devices = HidDevice.Enumerate()
                    .Select(de => new HidDevice(de));

                var connected = devices
                    .Where(d => d.Attributes != null)
                    .FirstOrDefault(d => d.Attributes.VendorId == _vendorId && d.Attributes.ProductId == _productId);

                if (connected != null)
                    return;

                _device.Dispose();
                _device = null;

                OnRemoved?.Invoke();
            }
        }


        public void Dispose()
        {
            if (_device != null)
                _device.Dispose();
        }

        private void CheckConnected()
        {
            if (_device == null)
            {
                monitor_Arrival();
            }

            if (_device == null)
            {
                throw new Exception("Device not found!");
            }
        }

        private void Write(byte[] msg)
        {
            CheckConnected();

            _device.Write(msg);
        }

        private async Task<byte[]> WriteAndRead(byte[] msg)
        {
            CheckConnected();

            var writed = _device.Write(msg);
            if (writed == 0)
            {
                throw new Exception("Error on write!");
            }

            var readed = await _device.Read();

            if (readed.Success)
            {
                return readed.Data;
            }
            else
            {
                throw readed.Error;
            }
        }

        public Task SendMessage(IMessageRequest request)
        {
            var bytesRequest = request.GetBytes();

            return Task.Run(() =>
            {
                Write(bytesRequest);
            });
        }

        public async Task<T> SendMessageWithResonse<T>(IMessageRequest request) where T : IMessageResponse
        {
            var bytesRequest = request.GetBytes();

            var bytesResponse = await WriteAndRead(bytesRequest);

            var response = Activator.CreateInstance<T>();

            response.Parse(bytesResponse);

            return response;
        }
    }
}