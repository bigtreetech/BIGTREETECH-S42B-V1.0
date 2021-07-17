using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TrueStepTerminal
{
    /// <summary>
    /// Serial protocol implementation for the original BTT protocol
    /// </summary>
    public class SerialBTT : SerialProtocolBase
    {

        string receivedString;

        public override byte[] GeneratePacket(byte[] data)
        {
            byte[] arg = new byte[2];
            byte functionCode = ConvertFunctionCode(data, arg);

            byte[] newPacket = new byte[8];

            newPacket[0] = 0xFE;                            // Frame header
            newPacket[1] = 0xFE;                            // Frame header
            newPacket[2] = 0x05;                            // Length of the rest of the message
            newPacket[3] = functionCode;                    // Function code
            newPacket[4] = arg[0];                          // Data
            newPacket[5] = arg[1];                          // Data
            newPacket[6] = CalculateCheckSum(newPacket);    // Checksum
            newPacket[7] = 0x16;                            // Frame tail

            return newPacket;
        }


        public override void Parse(byte receivedByte)
        {
         // Could probably be optimized...

            receivedString += (char)receivedByte;

            if (receivedString.Contains("\r\n"))
            {
                int endOfLine = receivedString.IndexOf("\r\n");
                string subString = receivedString.Substring(0, endOfLine);
                receivedString = receivedString.Remove(0, endOfLine + 2);

                if (subString.Contains("kp ="))
                {
                    int valIndex = subString.IndexOf("kp =") + 4;
                    int val;

                    if (int.TryParse(subString.Substring(valIndex), out val))
                    {
                        byte[] valBytes = BitConverter.GetBytes((Int16)val);
                        NotifyNewPacketReceived(new byte[] { 2, 10, valBytes[0], valBytes[1] });
                    }
                }

                if (subString.Contains("ki ="))
                {
                    int valIndex = subString.IndexOf("ki =") + 4;
                    int val;

                    if (int.TryParse(subString.Substring(valIndex), out val))
                    {
                        byte[] valBytes = BitConverter.GetBytes((Int16)val);
                        NotifyNewPacketReceived(new byte[] { 2, 11, valBytes[0], valBytes[1] });
                    }
                }

                if (subString.Contains("kd ="))
                {
                    int valIndex = subString.IndexOf("kd =") + 4;
                    int val;

                    if (int.TryParse(subString.Substring(valIndex), out val))
                    {
                        byte[] valBytes = BitConverter.GetBytes((Int16)val);
                        NotifyNewPacketReceived(new byte[] { 2, 12, valBytes[0], valBytes[1] });
                    }
                }

                if (subString.Contains("Currents ="))
                {
                    int valIndex = subString.IndexOf("Currents =") + 10;
                    int val;

                    subString = subString.Replace("mA", "").Trim();
                    if (int.TryParse(subString.Substring(valIndex), out val))
                    {
                        byte[] valBytes = BitConverter.GetBytes((Int16)(val / 6.5));
                        NotifyNewPacketReceived(new byte[] { 2, 13, valBytes[0], valBytes[1] });
                    }
                }

                if (subString.Contains("Microstep ="))
                {
                    int valIndex = subString.IndexOf("Microstep =") + 11;
                    int val;

                    subString = subString.Remove(valIndex + 2);
                    if (int.TryParse(subString.Substring(valIndex), out val))
                    {
                        byte[] valBytes = BitConverter.GetBytes((Int16)val);
                        NotifyNewPacketReceived(new byte[] { 2, 14, valBytes[0], valBytes[1] });
                    }
                }

                if (subString.Contains("!Enable ="))
                {
                    int valIndex = subString.IndexOf("!Enable =") + 9;
                    int val;

                    if (int.TryParse(subString.Substring(valIndex), out val))
                    {
                        byte[] valBytes = BitConverter.GetBytes((Int16)val);
                        NotifyNewPacketReceived(new byte[] { 2, 15, valBytes[0], valBytes[1] });
                    }
                }

                if (subString.Contains("Dir ="))
                {
                    int valIndex = subString.IndexOf("Dir =") + 5;
                    int val;

                    subString = subString.Remove(valIndex + 1); //.Replace(", CW", "").Trim();
                    if (int.TryParse(subString.Substring(valIndex), out val))
                    {
                        byte[] valBytes = BitConverter.GetBytes((Int16)val);
                        NotifyNewPacketReceived(new byte[] { 2, 16, valBytes[0], valBytes[1] });
                    }
                }

                if (subString.Contains("Frame Err!"))
                {
                    // Notify of error
                    NotifyNewPacketError("Frame Err!");
                }
            }

            Console.Write((char)receivedByte);
        }



        /// <summary>
        /// Convert TrueStep native message to BTT function code
        /// </summary>
        /// <param name="messageID"></param>
        /// <returns></returns>
        private byte ConvertFunctionCode(byte[] data, byte[] arg)
        {
            // Convert get parameter messages
            if (data[0] == (byte)Messages.MESSAGE_IDS.GETPARAM)
            {
                if ((data[1] == (byte)Messages.PARAMETER_TYPES.KP) ||
                   (data[1] == (byte)Messages.PARAMETER_TYPES.KI) ||
                   (data[1] == (byte)Messages.PARAMETER_TYPES.KD))
                {
                    arg[0] = arg[1] = 0xAA;
                    return 0xB0;
                }

                if (data[1] == (byte)Messages.PARAMETER_TYPES.CURRENT)
                {
                    arg[0] = arg[1] = 0xAA;
                    return 0xB1;
                }

                if (data[1] == (byte)Messages.PARAMETER_TYPES.STEPSIZE)
                {
                    arg[0] = arg[1] = 0xAA;
                    return 0xB2;
                }

                if (data[1] == (byte)Messages.PARAMETER_TYPES.ENDIR)
                {
                    arg[0] = arg[1] = 0xAA;
                    return 0xB3;
                }

                if (data[1] == (byte)Messages.PARAMETER_TYPES.MOTORDIR)
                {
                    arg[0] = arg[1] = 0xAA;
                    return 0xB4;
                }
            }


            if (data[0] == (byte)Messages.MESSAGE_IDS.SETPARAM)
            {
                if (data[1] == (byte)Messages.PARAMETER_TYPES.KP)
                {
                    arg[0] = data[2];
                    arg[1] = data[3];
                    return 0xA0;
                }
            
                if (data[1] == (byte)Messages.PARAMETER_TYPES.KI)
                {
                    arg[0] = data[2];
                    arg[1] = data[3];
                    return 0xA1;
                }
            
                if (data[1] == (byte)Messages.PARAMETER_TYPES.KD)
                {
                    arg[0] = data[2];
                    arg[1] = data[3];
                    return 0xA2;
                }
            
                if (data[1] == (byte)Messages.PARAMETER_TYPES.CURRENT)
                {
                    // The displayed current value is divided by 6.5 so have to 
                    // multiply it again by 6.5 before writing it 
                    UInt16 cur = (UInt16)((data[2] << 8) | data[3]);
                    cur = (UInt16)Math.Ceiling(cur * 6.5);
                    arg[0] = (byte)(cur >> 8);
                    arg[1] = (byte)(cur & 0x00FF);
                    return 0xA3;
                }

                if (data[1] == (byte)Messages.PARAMETER_TYPES.STEPSIZE)
                {
                    arg[0] = data[2];
                    arg[1] = data[3];
                    return 0xA4;
                }

                if (data[1] == (byte)Messages.PARAMETER_TYPES.ENDIR)
                {
                    arg[0] = 0x00;

                    if (data[3] > 0)
                        arg[1] = 0x55;
                    else
                        arg[1] = 0xAA;

                    return 0xA5;
                }

                if (data[1] == (byte)Messages.PARAMETER_TYPES.MOTORDIR)
                {
                    arg[0] = 0x00;

                    if (data[3] > 0)
                        arg[1] = 0x11;
                    else
                        arg[1] = 0x22;

                    return 0xA6;
                }
            }

            arg[0] = arg[1] = 0;
            return 0;
        }


        private byte CalculateCheckSum(byte[] data)
        {
            return (byte)((data[2] + data[3] + data[4] + data[5]) & 0xFF);
        }
    }
}
