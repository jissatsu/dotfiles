using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Threading.Tasks;

class Network
{
    private static string[] hosts = new string[] {
        "https://api.ipify.org?format=json",
        "https://api.myip.com/"
    };

    public static string GetIP()
    {
        /* string ip = String.Empty;

        foreach( var host in hosts ) {
            if( EndPointReachable( host ) ) {
                using ( var wc = new WebClient() ) {
                    try {
                        using ( var stream = wc.OpenRead( host ) )
                            return true;
                    }
                    catch (WebException) {
                        return false;
                    }
                }
            }
        } */
        return "";
    }

    public static bool IsAvailable()
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
}