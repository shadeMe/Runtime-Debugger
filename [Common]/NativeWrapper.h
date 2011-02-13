#pragma once

#include "Includes.h"

class Script;
struct ScriptEventList;

class NativeWrapper
{
public:
	[DllImport("RuntimeDebugger.dll")] 
	static void											_D_PRINT(const char* Message);
	[DllImport("RuntimeDebugger.dll")] 
	static bool											GetScriptDataForForm(bool* Result, UInt32 FormID, Script** ResultScript, ScriptEventList** ResultEventList);
														// ### function result always returns false when crossing the DLL boundary
	[DllImport("RuntimeDebugger.dll")] 
	static bool											ToggleDebugBreakInvocationState();

	[DllImport("user32.dll")]
	static bool											LockWindowUpdate(IntPtr hWndLock);
	[DllImport("user32.dll")]
	static IntPtr										WindowFromPoint(Point Point);
	[DllImport("user32.dll")]
	static bool											ReleaseCapture(void);
	[DllImport("kernel32.dll")]
	static void											ExitProcess(UInt32 ExitCode);
};