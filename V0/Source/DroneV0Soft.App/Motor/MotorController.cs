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

            if (response.Mode != mode)
                throw new Exception("ChannelChangeMode fail!");
        }

        public async Task ChannelChangeState(int index, ChannelStateEnum state)
        {
            var request = new ChannelChangeStateRequest
            {
                Index = index,
                State = state
            };

            var response = await Transport.SendMessageWithResonse<ChannelChangeStateResponse>(request);

            if (state == ChannelStateEnum.CS_Automatic_Off || state == ChannelStateEnum.CS_AutomaticStoping)
            {
                if (!(response.State == ChannelStateEnum.CS_Automatic_Off || response.State == ChannelStateEnum.CS_AutomaticStoping))
                    throw new Exception("ChannelChangeState fail!");
            }
            else
            {
                if (response.State != state)
                    throw new Exception("ChannelChangeState fail!");
            }
        }

        public async Task ChannelManualConfig(int index, byte direction, byte oneStep)
        {
            var request = new ChannelManualConfig
            {
                Index = index,
                Direction = direction,
                OneStep = oneStep
            };

            await Transport.SendMessage(request);
        }

        public async Task ChannelManualStep(int index, uint stepTicks)
        {
            var request = new ChannelManualStep
            {
                Index = index,
                StepTicks = stepTicks
            };

            await Transport.SendMessage(request);
        }

        public async Task ChannelManualPWM(int index, ushort pwmOnBeforeAdc, ushort pwmOnAfterAdc, ushort pwmOff)
        {
            var request = new ChannelManualPWM
            {
                Index = index,
                PwmOnBeforeAdc = pwmOnBeforeAdc,
                PwmOnAfterAdc = pwmOnAfterAdc,
                PwmOff = pwmOff
            };

            await Transport.SendMessage(request);
        }
    }
}