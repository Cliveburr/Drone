using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UsbHidLibrary;

namespace DroneV0Soft.App
{
    public class Device : IDisposable
    {
        public delegate void OnMessageReceiveDelegate(byte[] msg);
        public event OnMessageReceiveDelegate OnMessageReceive;
        public bool IsConnected { get; private set; }

        private const int _vendorId = 0x1781;
        private const int _productId = 0x07D0;
        private HidDevice _device;
        private UsbMonitor _monitor;

        public Device()
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
                IsConnected = true;
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
                IsConnected = false;
            }
        }


        public void Dispose()
        {
            if (_device != null)
                _device.Dispose();
        }

        public bool SendMessage(byte[] msg)
        {
            if (!IsConnected)
            {
                throw new Exception("Device not found!");
            }

            return _device.Write(msg) > 0;
        }

        public async Task<byte[]> WriteAndRead(byte[] msg)
        {
            if (!IsConnected)
            {
                throw new Exception("Device not found!");
            }

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
    }
}