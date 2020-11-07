using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TrueStepTerminal
{
    /// <summary>
    /// TrueStep native serial protocol implementation
    /// </summary>
    public class Serial:SerialProtocolBase
    {
        enum PARSE_STATES
        {
            IDLE,
            PREAMBLE,
            LENGTH,
            MESSAGEID,
            PAYLOAD,
            CHECKSUM
        };


        PARSE_STATES parseState;
        Queue<byte> parseBuffer = new Queue<byte>();
        int pBytes;
        int msgLength;
        int seq;

        void ResetParser()
        {
            parseState = PARSE_STATES.IDLE;
            parseBuffer.Clear();
        }

        public override void Parse(byte data)
        {
            switch(parseState)
            {
                case PARSE_STATES.IDLE:
                    if (data == 0xFE)
                        parseState = PARSE_STATES.PREAMBLE;
                    break;

                case PARSE_STATES.PREAMBLE:
                    parseBuffer.Enqueue(data);
                    pBytes = data & 0x0F;
                    msgLength = pBytes + 3;
                    parseState = PARSE_STATES.LENGTH;
                    break;

                case PARSE_STATES.LENGTH:
                    parseBuffer.Enqueue(data);
                    if (pBytes > 0)
                        parseState = PARSE_STATES.MESSAGEID;
                    else
                        parseState = PARSE_STATES.PAYLOAD;
                    break;

                case PARSE_STATES.MESSAGEID:
                    parseBuffer.Enqueue(data);
                    pBytes--;

                    if (pBytes < 1)
                        parseState = PARSE_STATES.PAYLOAD;
                    break;

                case PARSE_STATES.PAYLOAD:
                    parseBuffer.Enqueue(data);
                    parseState = PARSE_STATES.IDLE;

                    byte[] packetBytes = parseBuffer.ToArray();

                    byte crc_calc = CalculateCRC8(packetBytes, 0, msgLength - 1);

                    if (crc_calc == data)
                    {
                        NotifyNewPacketReceived(packetBytes);
                    }
                    else
                    {
                        // Some error
                    }

                    ResetParser();
                    break;

                default:
                    break;
            }
        }


        public override byte[] GeneratePacket(byte[] data)
        {
            byte[] newPacket = new byte[data.Length + 3];

            newPacket[0] = 0xFE;
            newPacket[1] = (byte)((data.Length - 1) & 0x0F);
            newPacket[1] |= (byte)((seq++ << 4) & 0xF0);

            for (int i = 0; i < data.Length; i++)
                newPacket[i + 2] = data[i];

            newPacket[data.Length + 2] = CalculateCRC8(newPacket, 1, data.Length + 2);

            return newPacket;
        }


        byte CalculateCRC8(byte[] data, int offset, int length)
        {
            byte crc = 0xff;
            int i, j;

            for(i = offset; i < length; i++)
            {
                crc ^= data[i];
                for (j = 0; j < 8; j++)
                {
                    if ((crc & 0x80) != 0)
                        crc = (byte)((crc << 1) ^ 0x31);
                    else
                        crc <<= 1;
                }
            }

            return crc;
        }
        
    }
}
