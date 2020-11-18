using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Newtonsoft.Json;

namespace TrueStepTerminal
{
    public class Settings
    {
        public class ApplicationSettings
        {
            public class ConnectionSettings
            {
                public string Type { get; set; }
                public string Port { get; set; }
                public int Baud { get; set; }
            }

            public ConnectionSettings Connection;

            public ApplicationSettings()
            {
                Connection = new ConnectionSettings();
            }
        }

        ApplicationSettings AppSettings = new ApplicationSettings();

        public ApplicationSettings.ConnectionSettings Connection
        {
            get
            {
                return AppSettings.Connection;
            }
            set
            {
                AppSettings.Connection = value;
            }
        }

        public bool LoadFromFile()
        {
            string strSettings;
            try
            {
                strSettings = File.ReadAllText(@"settings.json");
                AppSettings = JsonConvert.DeserializeObject<ApplicationSettings>(strSettings);
            }
            catch
            {
                // Load failed, use default settings
                AppSettings.Connection.Port = "COM1";
                AppSettings.Connection.Baud = 38400;
                return false;
            }

            return true;
        }

        public bool WriteToFile()
        {
            try
            {
                string strSettings = JsonConvert.SerializeObject(AppSettings);
                File.WriteAllText(@"settings.json", strSettings);
            }
            catch
            {
                return false;
            }
            return true;
        }
    }
}
