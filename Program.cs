using System;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Timers;

namespace ConsoleApp1
{
    class Program {

        private static Timer atimer;
        public static void Main(string[] args)
        {
            SetTimer( 2000, OnTimeEvent );
            Console.ReadLine();
            atimer.Stop();
            atimer.Dispose();
        }

        private static void SetTimer(int sec, ElapsedEventHandler hndl) {
            int ttime = 1000;
            if( sec > 0 ) {
                ttime = sec;
            }
            atimer = new Timer(ttime);
            atimer.Elapsed += hndl;
            atimer.AutoReset = true;
            atimer.Enabled = true;
        }

        private static void OnTimeEvent(Object source, ElapsedEventArgs e) {
            Console.WriteLine("The Elapsed event was raised at {0:HH:mm:ss.fff}",
                          e.SignalTime);
        }
    }
}
