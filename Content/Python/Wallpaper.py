#img = r"C:\GitHub\SoundVisualization\Saved\Screenshots\WindowsEditor\HighresScreenshot00000.png"
#img = r"C:\Sample\SoundVisualization\Saved\Screenshots\WindowsEditor\HighresScreenshot00000.png"
#img = r"C:\GitHub\MetaSion\Screenshots\ScreenCapture.png"
#img = r"C:\Project\MetaSion\Screenshots\ScreenCapture.png"
img = r"C:\Project\MetaSion\Screenshots\ScreenCapture.png"


import subprocess
import ctypes
subprocess.call(ctypes.windll.user32.SystemParametersInfoW(20, 0, img, 0))
