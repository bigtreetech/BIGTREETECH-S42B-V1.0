using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace TrueStepTerminal
{
    public partial class frmMain : Form
    {
        delegate void UpdateParametersCallBack(string name, string value);
        delegate void UpdateValuesCallBack(string name, double value);


        Settings AppSettings = new Settings();
        SerialPort sPort;
        SerialProtocolBase serialMessages;
        Byte[] buffer = new byte[100];
        Parameters driverParameters;
        Timer tmrAutoTune = new Timer();
        int tmrAutoTuneTicks;


        public frmMain()
        {
            InitializeComponent();
        }


        private void frmMain_Load(object sender, EventArgs e)
        {
            AppSettings.LoadFromFile();

            sPort = new SerialPort();
            sPort.PortName = AppSettings.Connection.Port;
            sPort.BaudRate = AppSettings.Connection.Baud;
            sPort.DataReceived += SPort_DataReceived;

            driverParameters = new Parameters();

            //Bind list to dataGrid
            dataGridViewParameters.DataSource = driverParameters.Items;
            dataGridViewParameters.Columns[0].ReadOnly = true;
            dataGridViewParameters.SelectionMode = DataGridViewSelectionMode.FullRowSelect;

            tmrAutoTune.Interval = 1000;
            tmrAutoTune.Tick += tmrAutoTune_Tick;

            // Update GUI Settings
            cmbPortName.Text = AppSettings.Connection.Port;
            cmbPortBaud.Text = AppSettings.Connection.Baud.ToString();
            cmbPortBaud.Items.AddRange(new string[] { "9600", "19200", "38400", "57600", "115200" });

            // Create tooltips
            toolTips.SetToolTip(chkBTTProtocol, "Use original BTT protocol. Only works with original firmware!");
            toolTips.SetToolTip(chkWriteToFlash, "Write parameters to flash memory as opposed to RAM");
        }


        private void frmMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (sPort.IsOpen)
                sPort.Close();


            // Save previous session settings
            AppSettings.WriteToFile();
        }


        private void tmrAutoTune_Tick(object sender, EventArgs e)
        {
            if (tmrAutoTuneTicks%2 == 0)
                SoftMove(1024, 1);
            else
                SoftMove(1024, 0);

            if (tmrAutoTuneTicks < 3)
                tmrAutoTuneTicks++;
            else
            {
                tmrAutoTuneTicks = 0;
                tmrAutoTune.Stop();
            }
        }


        private void UpdateParameters(string name, string value)
        {
            if (this.InvokeRequired)
            {
                var d = new UpdateParametersCallBack(UpdateParameters);
                this.Invoke(d, name, value);
            }
            else
            {
                // If entry already exist then only update value
                if (driverParameters.Items.Any(o => o.Name == name))
                    driverParameters.Items.First(o => o.Name == name).Value = value;
                else
                    // Add new entry 
                    driverParameters.Items.Add(new Parameters.ParameterItem(name, value));
                
                dataGridViewParameters.Refresh();
            }
        }


        private void UpdateValues(string name, double value)
        {
            if (this.InvokeRequired)
            {
                var d = new UpdateValuesCallBack(UpdateValues);
                this.Invoke(d, name, value);
            }
            else
            {
                if (name == "angle")
                {
                    // Logging needs to happen fast and GUI updates slows it down
                    if (chkTuningEnable.Checked)
                        LogToFile(value);
                    else
                        lblAngleValue.Text = value.ToString("F");
                    
                }
                if (name == "angleErr")
                    lblAngleError.Text = value.ToString("F");
            }
        }


        private void LogToFile(double val)
        {
            using (System.IO.StreamWriter file =
            new System.IO.StreamWriter(@"TuneLog.txt", true))
            {
                file.WriteLine(val.ToString());
            }
        }


        private void SerialMessages_PacketReceived(object sender, byte[] packet)
        {
            switch(packet[1])
            {
                case (int)Messages.MESSAGE_IDS.PARAM_KP:
                    Int16 kp = BitConverter.ToInt16(packet, 2);
                    UpdateParameters("Kp", kp.ToString());
                    break;

                case (int)Messages.MESSAGE_IDS.PARAM_KI:
                    Int16 ki = BitConverter.ToInt16(packet, 2);
                    UpdateParameters("Ki", ki.ToString());
                    break;

                case (int)Messages.MESSAGE_IDS.PARAM_KD:
                    Int16 kd = BitConverter.ToInt16(packet, 2);
                    UpdateParameters("Kd", kd.ToString());
                    break;

                case (int)Messages.MESSAGE_IDS.PARAM_CURRENT:
                    byte current = packet[2];
                    UpdateParameters("Current", Math.Ceiling(current * 6.5).ToString());
                    break;

                case (int)Messages.MESSAGE_IDS.PARAM_STEPSIZE:
                    byte stepSize = packet[2];
                    UpdateParameters("Step Size", stepSize.ToString());
                    break;

                case (int)Messages.MESSAGE_IDS.PARAM_ENDIR:
                    byte endir = packet[2];
                    UpdateParameters("EN Pin", endir.ToString());
                    break;

                case (int)Messages.MESSAGE_IDS.PARAM_MOTORDIR:
                    byte motordir = packet[2];
                    UpdateParameters("Motor Direction", motordir.ToString());
                    break;

                case (int)Messages.MESSAGE_IDS.VALUE_ANGLE:
                    float angle = BitConverter.ToSingle(packet, 2);
                    UpdateValues("angle", angle);
                    break;

                case (int)Messages.MESSAGE_IDS.VALUE_ANGLE_ERR:
                    float angleErr = BitConverter.ToSingle(packet, 2);
                    UpdateValues("angleErr", angleErr);
                    break;
            }

        }


        private void SerialMessages_PacketError(object sender, string errMessage)
        {
            MessageBox.Show(errMessage, "Packet Error");
        }


        private void SPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int bytesToRead = sPort.BytesToRead;
            byte[] receivedBytes = new byte[bytesToRead];

            sPort.Read(receivedBytes, 0, bytesToRead);

            for (int i = 0; i < bytesToRead; i++)
                serialMessages.Parse(receivedBytes[i]);
        }


        private void btnConnect_Click(object sender, EventArgs e)
        {
            int baud;

            try
            {
                if (sPort.IsOpen)
                {
                    sPort.Close();
                    btnConnect.Text = "Connect";
                    chkBTTProtocol.Enabled = true;
                }
                else
                {
                    // Update connection settings
                    AppSettings.Connection.Port = cmbPortName.Text;
                    if (int.TryParse(cmbPortBaud.Text, out baud))
                        AppSettings.Connection.Baud = baud;
                    else
                        AppSettings.Connection.Baud = 38400;        // default incase there is an issue

                    // Select which serial protocol to use
                    if (chkBTTProtocol.Checked)
                        serialMessages = new SerialBTT();
                    else
                        serialMessages = new Serial();
                    
                    serialMessages.PacketReceived += SerialMessages_PacketReceived;
                    serialMessages.PacketError += SerialMessages_PacketError;

                    sPort.PortName = AppSettings.Connection.Port;
                    sPort.BaudRate = AppSettings.Connection.Baud;
                    sPort.Open();
                    btnConnect.Text = "Disconnect";
                    chkBTTProtocol.Enabled = false;
                }
            }
            catch
            {
                MessageBox.Show("Could not connect!","Connection Problem");
            }
        }


        private void cmbPortName_DropDown(object sender, EventArgs e)
        {
            cmbPortName.Items.Clear();
            cmbPortName.Items.AddRange(SerialPort.GetPortNames());
        }


        private void btnControlStep_Click(object sender, EventArgs e)
        {
            // Check if serial protocol object exist
            if (serialMessages == null)
                return;

            Messages.Msg_Command cmdStep = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.STEP };

            cmdStep.param1 = chkChangeDirection.Checked == true ? (byte)1 : (byte)0;

            byte[] msgStep = serialMessages.GeneratePacket(cmdStep.Serialize());

            if (sPort.IsOpen)
                sPort.Write(msgStep, 0, msgStep.Length);

            // Hierdie was vir die Fine Step, sal dit later weer gebruik
            /* 
            int loops = int.Parse(cmbStepLoop.Text);
            UInt16 stepSize = UInt16.Parse(cmbStepSize.Text); //1800;
            UInt16 current = UInt16.Parse(cmbStepCurrent.Text); //500;

            Messages.Msg_Command cmd;
            if (cmbStepDirection.Text == "CCW")
                cmd = new Messages.Msg_Command 
                { 
                    command = Messages.COMMAND_TYPES.STEP_FORWARD, 
                    param1 = (byte)(stepSize >> 8), 
                    param2 = (byte)(stepSize & 0x00FF),
                    param3 = (byte)(current >> 8),
                    param4 = (byte)(current & 0x00FF)
                };
            else
                cmd = new Messages.Msg_Command 
                { 
                    command = Messages.COMMAND_TYPES.STEP_BACK, 
                    param1 = (byte)(stepSize >> 8), 
                    param2 = (byte)(stepSize & 0x00FF),
                    param3 = (byte)(current >> 8),
                    param4 = (byte)(current & 0x00FF)
                };

            byte[] msg = serialMessages.GeneratePacket(cmd.Serialize());

            for (int i = 0; i < loops; i++)
            {
                if (sPort.IsOpen)
                    sPort.Write(msg, 0, msg.Length);

                System.Threading.Thread.Sleep(10);
            }
            */
        }


        private void btnControlGetAngle_Click(object sender, EventArgs e)
        {
            // Check if serial protocol object exist
            if (serialMessages == null)
                return;

            Messages.Msg_GetValue getAng = new Messages.Msg_GetValue { value = Messages.VALUE_TYPES.ANGLE };
            Messages.Msg_GetValue getAngErr = new Messages.Msg_GetValue { value = Messages.VALUE_TYPES.ANGERROR };
            

            byte[] msgAng = serialMessages.GeneratePacket(getAng.Serialize());
            byte[] msgAngErr = serialMessages.GeneratePacket(getAngErr.Serialize());
            

            if (sPort.IsOpen)
            {
                sPort.Write(msgAng, 0, msgAng.Length);
                sPort.Write(msgAngErr, 0, msgAngErr.Length);
            }
        }


        private void btnReadParameters_Click(object sender, EventArgs e)
        {
            // Check if serial protocol object exist
            if (serialMessages == null)
                return;

            Messages.Msg_GetParam getParamKp = new Messages.Msg_GetParam { param = Messages.PARAMETER_TYPES.KP };
            Messages.Msg_GetParam getParamKi = new Messages.Msg_GetParam { param = Messages.PARAMETER_TYPES.KI };
            Messages.Msg_GetParam getParamKd = new Messages.Msg_GetParam { param = Messages.PARAMETER_TYPES.KD };
            Messages.Msg_GetParam getParamCurrent = new Messages.Msg_GetParam { param = Messages.PARAMETER_TYPES.CURRENT };
            Messages.Msg_GetParam getParamStepSize = new Messages.Msg_GetParam { param = Messages.PARAMETER_TYPES.STEPSIZE };
            Messages.Msg_GetParam getParamENDir = new Messages.Msg_GetParam { param = Messages.PARAMETER_TYPES.ENDIR };
            Messages.Msg_GetParam getParamMotorDir = new Messages.Msg_GetParam { param = Messages.PARAMETER_TYPES.MOTORDIR };
    
            byte[] msgKp = serialMessages.GeneratePacket(getParamKp.Serialize());
            byte[] msgKi = serialMessages.GeneratePacket(getParamKi.Serialize());
            byte[] msgKd = serialMessages.GeneratePacket(getParamKd.Serialize());
            byte[] msgCurrent = serialMessages.GeneratePacket(getParamCurrent.Serialize());
            byte[] msgStepSize = serialMessages.GeneratePacket(getParamStepSize.Serialize());
            byte[] msgENDir = serialMessages.GeneratePacket(getParamENDir.Serialize());
            byte[] msgMotorDir = serialMessages.GeneratePacket(getParamMotorDir.Serialize());
            
            if (sPort.IsOpen)
            {
                // Short delays here are for compatibility with BTT serial protocol, which is rather slow
                sPort.Write(msgKp, 0, msgKp.Length);
                System.Threading.Thread.Sleep(50);

                sPort.Write(msgKi, 0, msgKi.Length);
                System.Threading.Thread.Sleep(50);

                sPort.Write(msgKd, 0, msgKd.Length);
                System.Threading.Thread.Sleep(50);

                sPort.Write(msgCurrent, 0, msgCurrent.Length);
                System.Threading.Thread.Sleep(50);

                sPort.Write(msgStepSize, 0, msgStepSize.Length);
                System.Threading.Thread.Sleep(50);

                sPort.Write(msgENDir, 0, msgENDir.Length);
                System.Threading.Thread.Sleep(50);

                sPort.Write(msgMotorDir, 0, msgMotorDir.Length);
                System.Threading.Thread.Sleep(50);
            }
        }


        private void btnWriteParameters_Click(object sender, EventArgs e)
        {
            // Check if serial protocol object exist
            if (serialMessages == null)
                return;

            try
            {
                Int16 valKp = Int16.Parse(driverParameters.Items.First(o => o.paramName == "Kp").Value);
                Int16 valKi = Int16.Parse(driverParameters.Items.First(o => o.paramName == "Ki").Value);
                Int16 valKd = Int16.Parse(driverParameters.Items.First(o => o.paramName == "Kd").Value);
                Int16 valCurrent = (Int16)Math.Ceiling(Int16.Parse(driverParameters.Items.First(o => o.paramName == "Current").Value) / 6.5);
                Int16 valStepSize = Int16.Parse(driverParameters.Items.First(o => o.paramName == "Step Size").Value);
                Int16 valENDir = Int16.Parse(driverParameters.Items.First(o => o.paramName == "EN Pin").Value);
                Int16 valMotorDir = Int16.Parse(driverParameters.Items.First(o => o.paramName == "Motor Direction").Value);

                Messages.Msg_SetParam setParamKp = new Messages.Msg_SetParam { param = Messages.PARAMETER_TYPES.KP, value = valKp };
                Messages.Msg_SetParam setParamKi = new Messages.Msg_SetParam { param = Messages.PARAMETER_TYPES.KI, value = valKi };
                Messages.Msg_SetParam setParamKd = new Messages.Msg_SetParam { param = Messages.PARAMETER_TYPES.KD, value = valKd };
                Messages.Msg_SetParam setParamCurrent = new Messages.Msg_SetParam { param = Messages.PARAMETER_TYPES.CURRENT, value = valCurrent };
                Messages.Msg_SetParam setParamStepSize = new Messages.Msg_SetParam { param = Messages.PARAMETER_TYPES.STEPSIZE, value = valStepSize };
                Messages.Msg_SetParam setParamENDir = new Messages.Msg_SetParam { param = Messages.PARAMETER_TYPES.ENDIR, value = valENDir };
                Messages.Msg_SetParam setParamMotorDir = new Messages.Msg_SetParam { param = Messages.PARAMETER_TYPES.MOTORDIR, value = valMotorDir };

                byte[] msgKp = serialMessages.GeneratePacket(setParamKp.Serialize());
                byte[] msgKi = serialMessages.GeneratePacket(setParamKi.Serialize());
                byte[] msgKd = serialMessages.GeneratePacket(setParamKd.Serialize());
                byte[] msgCurrent = serialMessages.GeneratePacket(setParamCurrent.Serialize());
                byte[] msgStepSize = serialMessages.GeneratePacket(setParamStepSize.Serialize());
                byte[] msgENDir = serialMessages.GeneratePacket(setParamENDir.Serialize());
                byte[] msgMotorDir = serialMessages.GeneratePacket(setParamMotorDir.Serialize());


                if (sPort.IsOpen)
                {
                    // Short delays here are for compatibility with BTT serial protocol, which is rather slow
                    sPort.Write(msgKp, 0, msgKp.Length);
                    System.Threading.Thread.Sleep(100);

                    sPort.Write(msgKi, 0, msgKi.Length);
                    System.Threading.Thread.Sleep(100);

                    sPort.Write(msgKd, 0, msgKd.Length);
                    System.Threading.Thread.Sleep(100);

                    sPort.Write(msgCurrent, 0, msgCurrent.Length);
                    System.Threading.Thread.Sleep(100);

                    sPort.Write(msgStepSize, 0, msgStepSize.Length);
                    System.Threading.Thread.Sleep(100);

                    sPort.Write(msgENDir, 0, msgENDir.Length);
                    System.Threading.Thread.Sleep(100);

                    sPort.Write(msgMotorDir, 0, msgMotorDir.Length);
                    System.Threading.Thread.Sleep(100);

                    if (chkWriteToFlash.Enabled & chkWriteToFlash.Checked)
                    {
                        Messages.Msg_Command cmdFlashSave = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.STORAGE_SAVE };
                        byte[] msgFlashSave = serialMessages.GeneratePacket(cmdFlashSave.Serialize());
                        sPort.Write(msgFlashSave, 0, msgFlashSave.Length);
                    }
                }
            }
            catch
            {
                MessageBox.Show("Something went wrong while trying to write parameters!", "Writing Error");
            }
        }


        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }


        private void rdOpenLoop_CheckedChanged(object sender, EventArgs e)
        {
            // Check if serial protocol object exist
            if (serialMessages == null)
                return;

            if (rdOpenLoop.Checked)
            {
                Messages.Msg_Command cmdModeOpen = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.MODE_OPENLOOP };
                byte[] msgModeOpen = serialMessages.GeneratePacket(cmdModeOpen.Serialize());
                if (sPort.IsOpen)
                    sPort.Write(msgModeOpen, 0, msgModeOpen.Length);
            }
        }


        private void rdClosedLoop_CheckedChanged(object sender, EventArgs e)
        {
            // Check if serial protocol object exist
            if (serialMessages == null)
                return;

            if (rdClosedLoop.Checked)
            {
                Messages.Msg_Command cmdModeClose = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.MODE_CLOSELOOP };
                byte[] msgModeClose = serialMessages.GeneratePacket(cmdModeClose.Serialize());
                if (sPort.IsOpen)
                    sPort.Write(msgModeClose, 0, msgModeClose.Length);
            }
        }


        private void chkEnable_CheckedChanged(object sender, EventArgs e)
        {
            // Check if serial protocol object exist
            if (serialMessages == null)
                return;

            if (chkEnable.Checked)
            {
                //Verify the mode before enabling
                rdOpenLoop_CheckedChanged(this, e);
                rdClosedLoop_CheckedChanged(this, e);

                // Seem unreliable to write while enabled
                btnWriteParameters.Enabled = false;

                Messages.Msg_Command cmdModeEnable = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.MODE_ENABLE };
                byte[] msgModeEnable = serialMessages.GeneratePacket(cmdModeEnable.Serialize());
                if (sPort.IsOpen)
                    sPort.Write(msgModeEnable, 0, msgModeEnable.Length);
            }
            else
            {
                btnWriteParameters.Enabled = true;

                Messages.Msg_Command cmdModeDisable = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.MODE_DISABLE };
                byte[] msgModeDisable = serialMessages.GeneratePacket(cmdModeDisable.Serialize());
                if (sPort.IsOpen)
                    sPort.Write(msgModeDisable, 0, msgModeDisable.Length);
            }
        }


        private void SoftMove(int steps, int direction)
        {
            // Check if serial protocol object exist
            if (serialMessages == null)
                return;

            // Check if serial protocol object exist
            if (serialMessages == null)
                return;

            Messages.Msg_Command cmdMove = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.MOVE };
            cmdMove.param1 = (byte)(steps >> 8);
            cmdMove.param2 = (byte)(steps & 0x00FF);
            cmdMove.param3 = (byte)direction;
            
            byte[] msgMove = serialMessages.GeneratePacket(cmdMove.Serialize());
            
            if (sPort.IsOpen)
                sPort.Write(msgMove, 0, msgMove.Length);
        }

        private void btnMove_Click(object sender, EventArgs e)
        {
            SoftMove(1024, chkChangeDirection.Checked == true ? 1 : 0);
        }

        private void chkTuningEnable_CheckedChanged(object sender, EventArgs e)
        {
            Messages.Msg_Command cmdStreamAngle;

            if (chkTuningEnable.Checked)
            {
                cmdStreamAngle = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.STREAM_ANGLE, param1 = 1 };
            }
            else
            {
                cmdStreamAngle = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.STREAM_ANGLE, param1 = 0 };
            }

            byte[] msgStreamAng = serialMessages.GeneratePacket(cmdStreamAngle.Serialize());

            if (sPort.IsOpen)
                sPort.Write(msgStreamAng, 0, msgStreamAng.Length);
        }

        private void btnAutoTune_Click(object sender, EventArgs e)
        {
            // ! THIS FUNCTION IS INCOMPLETE !
            tmrAutoTune.Start();

        }

        private void btnFirmwareUpload_Click(object sender, EventArgs e)
        {
            //FlashLoader flashProg = new FlashLoader();
           
            // Step 1: Place closed loop drive into UART bootloader mode
            Messages.Msg_Command cmdJumpToBootloader;

            cmdJumpToBootloader = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.JUMP_BOOTLOADER };

            byte[] msgJumpToBootloader = serialMessages.GeneratePacket(cmdJumpToBootloader.Serialize());

            if (sPort.IsOpen)
                sPort.Write(msgJumpToBootloader, 0, msgJumpToBootloader.Length);
          
            // Close the port to make it available for the flashloader
            sPort.Close();
            while (sPort.IsOpen);

            System.Threading.Thread.Sleep(1000);

            
            // Step 2: Use the STMFlashloader program to flash the selected file
            string arguments = "-c " + 
                " --pn " + cmbPortName.Text.Replace("COM", "") +
                " --br 115200 --db 8 --pr EVEN --sb 1 --ec OFF --to 10000 --co ON -Dtr --Hi -Rts --Lo" +
                " -i STM32F0_5x_3x_64K" +
                " -u --fn " + txtFirmwareUploadPath.Text +
                " -Dtr --Lo -Rts --Hi --Lo";
      

            //System.Diagnostics.Process.Start(@"C:\Program Files (x86)\STMicroelectronics\Software\Flash Loader Demo\STMFlashLoader.exe", arguments);
            
            // Console.WriteLine(flashProg.Connect());
            // Console.WriteLine(flashProg.Close());

            
            System.Diagnostics.ProcessStartInfo processFlashLoader = new System.Diagnostics.ProcessStartInfo();
            processFlashLoader.FileName = @"C:\Program Files (x86)\STMicroelectronics\Software\Flash Loader Demo\STMFlashLoader.exe";
            processFlashLoader.Arguments = arguments;
            processFlashLoader.RedirectStandardOutput = true;
            processFlashLoader.UseShellExecute = false;

            System.Diagnostics.Process p = new System.Diagnostics.Process();
            p.OutputDataReceived += (sende, args) => Console.WriteLine("FlashLoader: {0}", args.Data);

            p.StartInfo = processFlashLoader; //System.Diagnostics.Process.Start(processFlashLoader);
            p.Start();
            p.BeginOutputReadLine();
            
        }

        private void btnFirmwareDownload_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFirmwareFile = new SaveFileDialog();
            saveFirmwareFile.Filter = "Bin files|*.bin|Hex files|*.hex|All files|*.*";

            if (saveFirmwareFile.ShowDialog() != DialogResult.OK)
                return;

            string saveFileName = saveFirmwareFile.FileName;

            // Step 1: Place closed loop drive into UART bootloader mode
            Messages.Msg_Command cmdJumpToBootloader;

            cmdJumpToBootloader = new Messages.Msg_Command { command = Messages.COMMAND_TYPES.JUMP_BOOTLOADER };

            byte[] msgJumpToBootloader = serialMessages.GeneratePacket(cmdJumpToBootloader.Serialize());

            if (sPort.IsOpen)
                sPort.Write(msgJumpToBootloader, 0, msgJumpToBootloader.Length);

            // Close the port to make it available for the flashloader
            sPort.Close();
            while (sPort.IsOpen) ;

            System.Threading.Thread.Sleep(1000);


            // Step 2: Use the STMFlashloader program to download to the selected file
            string arguments = "-c " +
                " --pn " + cmbPortName.Text.Replace("COM", "") +
                " --br 115200 --db 8 --pr EVEN --sb 1 --ec OFF --to 10000 --co ON -Dtr --Hi -Rts --Lo" +
                " -i STM32F0_5x_3x_64K" +
                " -u --fn " + saveFileName +
                " -Dtr --Lo -Rts --Hi --Lo";


            System.Diagnostics.ProcessStartInfo processFlashLoader = new System.Diagnostics.ProcessStartInfo();
            processFlashLoader.FileName = @"C:\Program Files (x86)\STMicroelectronics\Software\Flash Loader Demo\STMFlashLoader.exe";
            processFlashLoader.Arguments = arguments;
            processFlashLoader.UseShellExecute = true;

            System.Diagnostics.Process p = new System.Diagnostics.Process();
            p.StartInfo = processFlashLoader;
            p.Start();

        }


        private void chkBTTProtocol_CheckedChanged(object sender, EventArgs e)
        {
            if (chkBTTProtocol.Checked)
            {
                tabControls.Enabled = false;
                btnControlGetAngle.Enabled = false;
                chkWriteToFlash.Checked = true;
                chkWriteToFlash.Enabled = false;
            }
            else
            {
                tabControls.Enabled = true;
                btnControlGetAngle.Enabled = true;
                chkWriteToFlash.Checked = false;
                chkWriteToFlash.Enabled = true;
            }
        }

        private void btnBrowseFirmware_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFirmwareFile = new OpenFileDialog();
            openFirmwareFile.Filter = "Bin files|*.bin|Hex files|*.hex|All files|*.*";

            if (openFirmwareFile.ShowDialog() == DialogResult.OK)
            {
                txtFirmwareUploadPath.Text = openFirmwareFile.FileName;
            }
        }


    }
}
