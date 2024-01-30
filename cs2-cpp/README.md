# cs2-cpp

The ESP model of Counter-Strike, External, Made with C++ and ImGUI.

Made by Wane.

**IT IS UNDER CONSTRUCTION!! ITS STATE IS SO POOR, YOU'LL DEFINITELY DISAPPOINT ABOUT IT. I DON'T RECOMMEND USING NOW!**

## Usage

Copy whole file in `cs2-cpp` into your directory, and use by **defining `Esper` Class**.
The ESP should work right after you define it.
```c++
Esper esper(ProcessName, ModuleName, WindowName); // L"cs2.exe", L"client.dll", L"Counter-Strike 2"
```

## Troubleshooting

Error Codes:
|Error Code|Description|
|-------------|-------------|
|0x3D000001|Couldn't make the process snapshot, maybe computer issue?|
|0x3D000002|Couldn't find the process. did you correctly put `ProcessName`?|
|0x3D000003|Couldn't make the module snapshot, maybe computer issue?|
|0x3D000004|Couldn't find the module. did you correcly put `ModuleName`?|
|0x3D000005|aborted|
|0x3D000006|ReadProcessMemory Error|
|0x3D000007|aborted|
|0x3D000008|WorldToScreen Error|
|0x3D000009|Couldn't get buffer from swap chain, usually DirectX11 Error|
|0x3D00000A|Render Target View is null, usually DirectX11 Error|
|0x3D00000B|Couldn't make render thread|
|0x3D00000C|Couldn't find the window. did you correctly put `WindowName`?|
|0x3D00000D|Couldn't get Window Rect|

You can handle by yourself using `catch`, like this:
```c++
catch (int expn) {
  if (expn == 0x3D00000C) return 1337;
  else return 69;
}
```