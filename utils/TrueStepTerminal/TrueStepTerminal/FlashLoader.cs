using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace TrueStepTerminal
{
    /// <summary>
    /// Directly program the flash memory.
    /// 
    /// Still work in progress, uses STBLLIB.dll, Files.dll and STUARTBLLIB.dll
    /// </summary>
    public class FlashLoader
    {
        const string _dllLocation = "STBLLIB.dll";
        [DllImport(_dllLocation, EntryPoint = "COM_Open")]
        public static extern byte COM_Open();

        [DllImport(_dllLocation, EntryPoint = "COM_Close")]
        public static extern byte COM_Close();

        [DllImport(_dllLocation, EntryPoint = "SetCOMSettings", CallingConvention = CallingConvention.Cdecl)]
        public static extern byte SetCOMSettings(int numPort, long speedInBaud, int nbBit, int parity, float nbStopBit);

        public bool Connect()
        {
            Console.WriteLine(SetCOMSettings(5, 115200, 8, 2, 1));

            Console.WriteLine(COM_Open());
            //return COM_Open() == 0; 
            return false;
        }

        public bool Close()
        {
            Console.WriteLine(COM_Close());
            //return COM_Close() == 0;
            return false;
        }
    }
}
