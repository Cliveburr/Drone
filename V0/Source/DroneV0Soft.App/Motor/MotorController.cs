using DroneV0Soft.App.Motor.Message;
using DroneV0Soft.App.Motor.Transport;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DroneV0Soft.App.Motor
{
    public class MotorController
    {
        public ITransport Transport { get; set; }

        public async Task<ChannelInfoResponse> GetChannelInfo(int index)
        {
            var request = new ChannelInfoRequest
            {
                Index = index
            };

            var response = await Transport.SendMessageWithResonse<ChannelInfoResponse>(request);

            return response;
        }

        public async Task ChannelChangeMode(int index, ChannelModeEnum mode)
        {
            var request = new ChannelChangeModeRequest
            {
                Index = index,
                Mode = mode
            };

            var response = await Transport.SendMessageWithResonse<ChannelChangeModeResponse>(request);

            if (response.ChannelMode != mode)
                throw new Exception("ChannelChangeMode fail!");
        }

        public async Task ChannelChangState(int index, ChannelStateEnum state)
        {
            var request = new ChannelChangeStateRequest
            {
                Index = index,
                State = state
            };

            var response = await Transport.SendMessageWithResonse<ChannelChangeStateResponse>(request);

            if (response.ChannelState != state)
                throw new Exception("ChannelChangeState fail!");
        }
    }
}