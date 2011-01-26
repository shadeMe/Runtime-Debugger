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
	static bool											GetScriptDataForForm(UInt32 FormID, Script** ResultScript, ScriptEventList** ResultEventResult);


	[DllImport("user32.dll")]
	static bool											LockWindowUpdate(IntPtr hWndLock);
	[DllImport("user32.dll")]
	static IntPtr										WindowFromPoint(Point Point);
};