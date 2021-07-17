using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TrueStepTerminal
{
    /// <summary>
    /// Base class for all serial protocols
    /// </summary>
    public abstract class SerialProtocolBase
    {
        public delegate void PacketReceived_EventHandler(object sender, byte[] packet);
        public delegate void PacketError_EventHandler(object sender, string errMessage);
        public event PacketReceived_EventHandler PacketReceived;
        public event PacketError_EventHandler PacketError;

        public abstract byte[] GeneratePacket(byte[] data);

        public abstract void Parse(byte receivedByte);

        public void NotifyNewPacketReceived(byte[] data)
        {
            PacketReceived_EventHandler handler = PacketReceived;
                if (handler != null)
                    handler(this, data);
        }
        public void NotifyNewPacketError(string errMessage)
        {
            PacketError_EventHandler handler = PacketError;
            if (handler != null)
                handler(this, errMessage);
        }
    }
}
