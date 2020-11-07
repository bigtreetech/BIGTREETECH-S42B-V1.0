using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.Text;
using System.Threading.Tasks;

namespace TrueStepTerminal
{
    public class Messages
    {

        public enum MESSAGE_IDS
        {
            GETVALUE        = 1,
            GETPARAM        = 2,
            SETPARAM        = 3,
            COMMAND         = 4,
            PARAM_KP        = 10,
            PARAM_KI        = 11,
            PARAM_KD        = 12,
            PARAM_CURRENT   = 13,
            PARAM_STEPSIZE  = 14,
            PARAM_ENDIR     = 15,
            PARAM_MOTORDIR  = 16,
            STATUS          = 20,
            VALUE_ANGLE     = 21,
            VALUE_ANGLE_ERR = 22,
        }

        public enum VALUE_TYPES
        {
            STATUS          = 0,
            ANGLE           = 1,
            ANGERROR        = 2,
        }

        public enum PARAMETER_TYPES
        {
            CALFLAG         = 0,
            KP              = 1,
            KI              = 2,
            KD              = 3,
            CURRENT         = 4,
            STEPSIZE        = 5,
            ENDIR           = 6,
            MOTORDIR        = 7,
        }

        public enum COMMAND_TYPES
        {
            STEP            = 0,
            STEP_FORWARD    = 1,
            STEP_BACK       = 2,
            MOVE            = 3,
            STORAGE_SAVE    = 5,
            MODE_ENABLE     = 10,
            MODE_DISABLE    = 11,
            MODE_CLOSELOOP  = 12,
            MODE_OPENLOOP   = 13,
            STREAM_ANGLE    = 20,
            JUMP_BOOTLOADER = 30,
        }

        [Serializable]
        public abstract class BaseMessage
        {
            public abstract byte[] Serialize();
            /*{
                
                BinaryFormatter bf = new BinaryFormatter();
                
                using (var ms = new System.IO.MemoryStream())
                {
                    bf.Serialize(ms, this);
                    return ms.ToArray();
                }
            }*/
        };

        [Serializable]
        public class Msg_GetValue : BaseMessage
        {
            public VALUE_TYPES value;
            public override byte[] Serialize()
            {
                return new byte[] { (byte)MESSAGE_IDS.GETVALUE, (byte)value };
            }
        }

        [Serializable]
        public class Msg_GetParam : BaseMessage
        {
            public PARAMETER_TYPES param;
            public override byte[] Serialize()
            {
                return new byte[] { (byte)MESSAGE_IDS.GETPARAM, (byte)param };
            }
        }

        [Serializable]
        public class Msg_SetParam : BaseMessage
        {
            public PARAMETER_TYPES param;
            public Int16 value;
            public override byte[] Serialize()
            {
                return new byte[] { (byte)MESSAGE_IDS.SETPARAM, (byte)param, (byte)((value >> 8) & 0x00FF), (byte)(value & 0x00FF) };
            }
        }

        [Serializable]
        public class Msg_Command : BaseMessage
        {
            public COMMAND_TYPES command;
            public byte param1;
            public byte param2;
            public byte param3;
            public byte param4;
            public override byte[] Serialize()
            {
                return new byte[] { (byte)MESSAGE_IDS.COMMAND, (byte)command, (byte)param1, (byte)param2, (byte)param3, (byte)param4 };
            }
        }
    }
}
