using System;
using System.Net;
using ConsoleApp2.NativeMethods;
using System.Text.RegularExpressions;

class Utils
{
    private static string[] hosts = new string[] {
        "https://api.ipify.org?format=json",
        "https://api.myip.com/"
    };
    // ip pattern
    private static Regex pattern = new Regex(@"\b\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}\b");

    public enum ConsoleWindowState
    {
        SW_HIDE = 0,
        SW_SHOW = 5
    };

    public static bool DetectSandboxie()
    {
        string[] sandboxList = new string[] {
            "cmdvrt32.dll",
            "SxIn.dll",
            "SbieDll.dll"
        };
        foreach ( var sbox in sandboxList ) {
            if( NativeMethods.GetModuleHandle(sbox).ToInt32() != 0 ) {
                return true;
            }
        }
        return false;
    }

    public static string GetIP()
    {
        string data = String.Empty;

        foreach ( var host in hosts ) {
            using (var wc = new WebClient()) {
                try {
                    data = wc.DownloadString( host );
                }
                catch (WebException) {
                    
                }

                if( data != String.Empty ) {
                    MatchCollection result = pattern.Matches(data);
                    return result[0].ToString();
                }
            }
        }
        return "";
    }
}