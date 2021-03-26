using System;
using System.Collections.Generic;

class KeyMap
{
    // use a keymap dict to map keys that require the shift key
    public static Dictionary<char, char> keymap = new Dictionary<char, char>
    {
        {'`','~'},
        {'[','{'},
        {']','}'},
        {',','<'},
        {'.','>'},
        {'/','?'},
        {';',':'},
        {'\'','\"'},
        {'\\','|'},
        {'1','!'},
        {'2','@'},
        {'3','#'},
        {'4','$'},
        {'5','%'},
        {'6','^'},
        {'7','&'},
        {'8','*'},
        {'9','('},
        {'0',')'},
        {'-','_'},
        {'=','+'}
    };
}

